#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity_PokemonView_Evolution PokedexActivity_PokemonView_Evolution::instance;

PokedexActivity_PokemonView_Evolution::PokedexActivity_PokemonView_Evolution() :
needRedraw(true),
selectedIndex(0),
offset(0),
pokemon(nullptr),
backgroundSurface(nullptr),
listEntrySurface_default(nullptr),
listEntrySurface_selected(nullptr),
se_up_down(nullptr),
se_left_right(nullptr)
{
}

PokedexActivity_PokemonView_Evolution::~PokedexActivity_PokemonView_Evolution() {
	if(se_up_down)
		Mix_FreeChunk(se_up_down);

	if(se_left_right)
		Mix_FreeChunk(se_left_right);

}

void PokedexActivity_PokemonView_Evolution::printPokeInfo() {
    std::vector<unsigned short> stats = pokemon->getBasicStats();
    
    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';
    std::cout << "Evolution Chain ID: " << pokemon->getEvolutionChainID() << '\n';
    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Deffense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';

    for (std::vector<std::string> r : *pokemon->getEvolutionChain()) {
        for (auto& c : r) {
            std::cout << c << '|';
        }
        std::cout << std::endl;
    }

}

bool PokedexActivity_PokemonView_Evolution::initSDL(){
	try{
		se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
		if (!se_left_right) {
			std::cerr << "Failed to load sound se_up_down: " << Mix_GetError() << std::endl;
		}

		se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
		if (!se_left_right) {
			std::cerr << "Failed to load sound se_left_right: " << Mix_GetError() << std::endl;
		}

		fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 34);
		if (fontSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load fontSurface! SDL Error:  ") + SDL_GetError());
		}

		//Render background
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH.c_str());
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
		};

		backgroundRect = {
			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT
		};

		// ListEntry
		listEntrySurface_default = PokeSurface::onLoadImg(ENTRY_BACKGROUND_IMAGE_PATH_DEFAULT);
		if (listEntrySurface_default == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load listEntrySurface_default! SDL Error:  ") + SDL_GetError());
		};

		listEntrySurface_selected = PokeSurface::onLoadImg(ENTRY_BACKGROUND_IMAGE_PATH_SELECTED);
		if (listEntrySurface_selected == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load listEntrySurface_selected! SDL Error:  ") + SDL_GetError());
		};

		for (int i = 0; i < evoChain->size(); i++) {
			// Pokemon Sprite
			std::string poke = (*evoChain)[i][2];
			std::string path = SPRITE_IMG_BASE_PATH + poke + ".png";

			SDL_Surface* pokeSurface = PokeSurface::onLoadImg(path);
			if (pokeSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load pokeSurface! SDL Error:  ") + SDL_GetError());
			};

			//
			// make it a 3 digit
			std::stringstream formattedID;
			formattedID << std::setw(3) << std::setfill('0') << (*evoChain)[i][1];
			std::string id = formattedID.str();

			SDL_Surface* idSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				id.c_str(),
				{ 96, 96, 96 }
			);
			if (idSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load idSurface! SDL Error:  ") + SDL_GetError());
			};

			std::string name = (*evoChain)[i][3];
			SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				name.c_str(),
				{ 96, 96, 96 }
			);
			if (nameSurface == NULL ) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load idSurface! SDL Error:  ") + SDL_GetError());
			};

			//// Render poke method( level or item)
			std::vector<SDL_Surface*> temp;

			// TRIGGER
			std::string trigger = (*evoChain)[offset + i][4];
			SDL_Surface* triggerSurface = nullptr;
			if (trigger == "NULL") {
				trigger = "__";
				triggerSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					trigger.c_str(),
					{ 96, 96, 96 }
				);
			}
			else { 
				trigger = "res/assets/pokemons/encounters/" + trigger + ".png";
				triggerSurface = PokeSurface::onLoadImg(trigger);
			}

			if (triggerSurface == NULL ) {
				std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			};
			temp.push_back(triggerSurface);

			// LEVEL
			SDL_Surface* levelSurface = nullptr;
			std::string level = (*evoChain)[offset + i][6];
			if (level != "NULL") {
				level = " + " + level;
				levelSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					level.c_str(),
					{ 96, 96, 96 }
				);
				if (levelSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(levelSurface);

			// ITEMS
			// USE ITEM
			SDL_Surface* useItemSurface = nullptr;
			std::string useItem = (*evoChain)[offset + i][5];
			if (useItem != "NULL") {
				useItem = "res/assets/items/" + useItem + ".png";
				useItemSurface = PokeSurface::onLoadImg(useItem);
				if (useItemSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(useItemSurface);

			// TRADE ITEM
			SDL_Surface* heldItemSurface = nullptr;
			std::string heldItem = (*evoChain)[offset + i][9];
			if (heldItem != "NULL") {
				heldItem = "res/assets/items/" + heldItem + ".png";
				heldItemSurface = PokeSurface::onLoadImg(heldItem);
				if (heldItemSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(heldItemSurface);
			
			// GENDER
			std::string genderID = (*evoChain)[offset + i][7];
			SDL_Surface* genderSurface = nullptr;
			if (genderID != "NULL") {
				genderID = "res/assets/pokemons/gender/" + genderID + ".png";
				genderSurface = PokeSurface::onLoadImg(genderID);
				if (genderSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(genderSurface);

			// TIME
			SDL_Surface* timeIconSurface = nullptr;
			std::string time = (*evoChain)[offset + i][10];
			if (time != "NULL") {
				time = "res/assets/pokemons/encounters/" + time + ".png";
				timeIconSurface = PokeSurface::onLoadImg(time);
				if (timeIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(timeIconSurface);

			// HAPPINESS
			SDL_Surface* happinessIconSurface = nullptr;
			SDL_Surface* happinessSurface = nullptr;
			std::string happiness = (*evoChain)[offset + i][13];
			if (happiness != "NULL") {
				happiness = " + " + happiness;
				std::string happinessPath = "res/assets/pokemons/encounters/happiness.png";
				happinessIconSurface = PokeSurface::onLoadImg(happinessPath);
				if (happinessIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};

				happinessSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					happiness.c_str(),
					{ 96, 96, 96 }
				);
				if (happinessSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(happinessIconSurface);
			temp.push_back(happinessSurface);

			// BEAUTY
			SDL_Surface* beautyIconSurface = nullptr;
			SDL_Surface* beautySurface = nullptr;
			std::string beauty = (*evoChain)[offset + i][14];
			if (beauty != "NULL") {
				beauty = " + " + beauty;
				std::string beautyPath = "res/assets/pokemons/encounters/beauty.png";
				beautyIconSurface = PokeSurface::onLoadImg(beautyPath);
				if (beautyIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				beautySurface = TTF_RenderUTF8_Blended(
					fontSurface,
					beauty.c_str(),
					{ 96, 96, 96 }
				);
				if (beautySurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(beautyIconSurface);
			temp.push_back(beautySurface);

			// AFFECTION
			SDL_Surface* affectionIconSurface = nullptr;
			SDL_Surface* affectionSurface = nullptr;
			std::string affection = (*evoChain)[offset + i][14];
			if (affection != "NULL") {
				affection = " + " + affection;
				std::string affectionPath = "res/assets/pokemons/encounters/affection.png";
				affectionIconSurface = PokeSurface::onLoadImg(affectionPath);
				if (affectionIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				affectionSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					affection.c_str(),
					{ 96, 96, 96 }
				);
				if (affectionSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(affectionIconSurface);
			temp.push_back(affectionSurface);

			// KNOWN MOVE 
			SDL_Surface* knownMoveIconSurface = nullptr;
			SDL_Surface* knownMoveSurface = nullptr;
			std::string knownMove = (*evoChain)[offset + i][11];
			if (knownMove != "NULL") {
				knownMove = " + " + knownMove;
				std::string knownMovePath = "res/assets/pokemons/encounters/tutor.png";
				knownMoveIconSurface = PokeSurface::onLoadImg(knownMovePath);
				if (knownMoveIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				knownMoveSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					knownMove.c_str(),
					{ 96, 96, 96 }
				);
				if (knownMoveSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(knownMoveIconSurface);
			temp.push_back(knownMoveSurface);


			// MOVE TYPE
			std::string moveType = (*evoChain)[offset + i][12];
			SDL_Surface* moveTypeIconSurface = nullptr;
			SDL_Surface* moveTypeSurface = nullptr;
			if (moveType != "NULL") {
				moveType = " + " + moveType;
				std::string moveTypePath = "res/assets/pokemons/encounters/tutor.png";
				moveTypeIconSurface = PokeSurface::onLoadImg(moveTypePath);
				if (moveTypeIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				moveTypeSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					moveType.c_str(),
					{ 96, 96, 96 }
				);
				if (moveTypeSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(moveTypeIconSurface);
			temp.push_back(moveTypeSurface);

			// LOCATION
			SDL_Surface* locationIconSurface = nullptr;
			SDL_Surface* locationSurface = nullptr;
			std::string location = (*evoChain)[offset + i][8];
			if (location != "NULL") {
				std::string locationPath = "res/assets/pokemons/encounters/location.png";
				SDL_Surface* locationIconSurface = PokeSurface::onLoadImg(locationPath);
				if (locationIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				SDL_Surface* locationSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					location.c_str(),
					{ 96, 96, 96 }
				);
				if (locationSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(locationIconSurface);
			temp.push_back(locationSurface);

			// RELATIVE STATS
			std::string relativeStats = (*evoChain)[offset + i][16];
			SDL_Surface* atkIconSurface = nullptr;
			SDL_Surface* defIconSurface = nullptr;
			SDL_Surface* relativeStatSurface = nullptr;
			if (relativeStats != "NULL") {
				std::string atkIconPath = "res/assets/pokemons/encounters/stats_atk.png";
				atkIconSurface = PokeSurface::onLoadImg(atkIconPath);
				if (atkIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
				std::string defIconPath = "res/assets/pokemons/encounters/stats_def.png";
				defIconSurface = PokeSurface::onLoadImg(defIconPath);
				if (defIconSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};

				if(relativeStats == "1")
					relativeStats = "Attack > Deffense";
				if(relativeStats == "-1")
					relativeStats = "Attack < Deffense";
				if(relativeStats == "0")
					relativeStats = "Attack = Deffense";

				relativeStatSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					relativeStats.c_str(),
					{ 96, 96, 96 }
				);
				if (relativeStatSurface == NULL ) {
					std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
					exit(EXIT_FAILURE);
				};
			}
			temp.push_back(atkIconSurface);
			temp.push_back(defIconSurface);
			temp.push_back(relativeStatSurface);

			pokeSprite_cache.push_back(pokeSurface);
			idSurface_cache.push_back(idSurface);
			nameSurface_cache.push_back(nameSurface);
			evoConditionSurface_cache.push_back(temp);
		}
	}
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

void PokedexActivity_PokemonView_Evolution::clearCachedSurfaces(){
    std::cout << "PokedexActivity_PokemonView_Evolution::clearCachedSurfaces START \n";
	
    std::cout << "PokedexActivity_PokemonView_Evolution::clearing pokeSprite_cache \n";
	for(SDL_Surface* surface : pokeSprite_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
    std::cout << "PokedexActivity_PokemonView_Evolution::cleared pokeSprite_cache \n";

	for(SDL_Surface* surface : idSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
    std::cout << "PokedexActivity_PokemonView_Evolution::cleared idSurface_cache \n";

	for(SDL_Surface* surface : nameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}	
    std::cout << "PokedexActivity_PokemonView_Evolution::cleared nameSurface_cache \n";

    std::cout << "PokedexActivity_PokemonView_Evolution::clearing evoConditionSurfaces_cache \n";
	for(std::vector<SDL_Surface*> surfaces : evoConditionSurface_cache)
		for(SDL_Surface* surface : surfaces)
			if(surface){
				SDL_FreeSurface(surface);
				surface = nullptr;
			}

    std::cout << "PokedexActivity_PokemonView_Evolution::cleared evoConditionSurfaces_cache \n";
	pokeSprite_cache.clear();
	idSurface_cache.clear();
	nameSurface_cache.clear();
	evoConditionSurface_cache.clear();

    std::cout << "PokedexActivity_PokemonView_Evolution::clearCachedSurfaces END \n";
}

void PokedexActivity_PokemonView_Evolution::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate START \n";

    pokemon = new Pokemon();
	evoChain = pokemon->getEvolutionChain();
    printPokeInfo();

    evo = (*evoChain)[selectedIndex];

	needRedraw = true;

	if(!initSDL()){
		std::cerr << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
	
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate END \n";
}

void PokedexActivity_PokemonView_Evolution::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Evolution::onDeactive START \n";
	
	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

	if(listEntrySurface_default)
    	SDL_FreeSurface(listEntrySurface_default);
	listEntrySurface_default = nullptr;

	if(listEntrySurface_selected)
    	SDL_FreeSurface(listEntrySurface_selected);
	listEntrySurface_selected = nullptr;

	clearCachedSurfaces();

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Evolution::onDeactive END \n";
}

void PokedexActivity_PokemonView_Evolution::onLoop() {}

void PokedexActivity_PokemonView_Evolution::onFreeze() {}

void PokedexActivity_PokemonView_Evolution::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
	if(needRedraw){
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

		PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

		for (int i = 0; i < 3 && static_cast<std::size_t>(offset + i) < evoChain->size(); i++) {
        if (i > 0 && (*evoChain)[offset + i][1] == (*evoChain)[offset + i - 1][1]){ // < -- skip forms. for now..
            if (selectedIndex == evoChain->size() - 1) {
                selectedIndex--;
                if (selectedIndex - offset >= 3) {
                    offset--;
                }
            }
            continue;
        }

			// Render list items
			if (!renderListItems(surf_display, font, i)) {
				exit(EXIT_FAILURE);
			}
		}
		needRedraw = false;
	}
}

bool PokedexActivity_PokemonView_Evolution::renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i) {
    if (offset + i == selectedIndex) {
        if (!renderPokeInfo(surf_display, font, i)) {
            std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

	// ListEntry
	int spacing = 15; 
	listEntryRect = {
		static_cast<int>(WINDOW_WIDTH * 0.5),
		50 + (i * (ITEM_HEIGHT + spacing)),
		static_cast<int>(WINDOW_WIDTH * 0.5),
		ITEM_HEIGHT + 10
	};
	
	if(offset + i == selectedIndex){
		PokeSurface::onDrawScaled(surf_display, listEntrySurface_selected, &listEntryRect);
	}
	else {
		PokeSurface::onDrawScaled(surf_display, listEntrySurface_default, &listEntryRect);
	}

    // render poke sprite
	pokeRect = {
		static_cast<int>((listEntryRect.x + listEntryRect.w ) - pokeSprite_cache[offset + i]->w * 1.3),
		listEntryRect.y + 10,
		pokeSprite_cache[offset + i]->w,
		pokeSprite_cache[offset + i]->h
	};
    PokeSurface::onDrawScaled(surf_display, pokeSprite_cache[offset + i], &pokeRect);

    // Render Item ID
    idRect = {
        listEntryRect.x + 60,
        listEntryRect.y + 25,
        static_cast<int>(idSurface_cache[i + offset]->w * .8),
        static_cast<int>(idSurface_cache[i + offset]->h * .8)
    };
    PokeSurface::onDrawScaled(surf_display, idSurface_cache[i + offset], &idRect);

    //// Render poke name( level or item)
    nameRect = {
        listEntryRect.x + 10,
        idRect.y + idRect.h + 20,
        static_cast<int>(nameSurface_cache[i + offset]->w * .9),
        static_cast<int>(nameSurface_cache[i + offset]->h * .9)
    };
    PokeSurface::onDrawScaled(surf_display, nameSurface_cache[i + offset], &nameRect);

    return true;
}

bool PokedexActivity_PokemonView_Evolution::renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // render poke sprite
    pokeRect= {
        50,
        90,
        pokeSprite_cache[offset + i]->w * 2,
        pokeSprite_cache[offset + i]->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, pokeSprite_cache[offset + i], &pokeRect);
    
    //// Render poke method( level or item)
	// TRIGGER
	int evo_item_y = 0;
	SDL_Rect triggerIconRect = {
		0,
		pokeRect.y + pokeRect.h,
		static_cast<int>(evoConditionSurface_cache[selectedIndex][0]->w * 1.5),
		static_cast<int>(evoConditionSurface_cache[selectedIndex][0]->h * 1.5)
	};
    PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][0], &triggerIconRect);
	evo_item_y = triggerIconRect.y + triggerIconRect.h;

	// LEVEL
	if (evoConditionSurface_cache[selectedIndex][1]) {
		int leftPadding = 5;
		SDL_Rect levelRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(evoConditionSurface_cache[selectedIndex][1]->w),
			static_cast<int>(evoConditionSurface_cache[selectedIndex][1]->h)
		};
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][1], &levelRect);
	}

	// ITEMS
	// USE ITEM
	if (evoConditionSurface_cache[selectedIndex][2]) {
		int leftPadding = 5;
		SDL_Rect itemRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(evoConditionSurface_cache[selectedIndex][2]->w * 1.5),
			static_cast<int>(evoConditionSurface_cache[selectedIndex][2]->h * 1.5)
		};
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][2], &itemRect);
	}

	// TRADE ITEM
	if (evoConditionSurface_cache[selectedIndex][3]) {
		int leftPadding = 5;
		SDL_Rect itemRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(evoConditionSurface_cache[selectedIndex][3]->w * 1.5),
			static_cast<int>(evoConditionSurface_cache[selectedIndex][3]->h * 1.5)
		};
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][3], &itemRect);
	}
	
	// GENDER
	if (evoConditionSurface_cache[selectedIndex][4]) {
		int leftPadding = 5;
		SDL_Rect genderRect;
		genderRect.x = triggerIconRect.x;
		genderRect.y = evo_item_y;
		genderRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][4]->w);
		genderRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][4]->h);

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][4], &genderRect);
		evo_item_y = genderRect.y + genderRect.h;
	}

	// TIME
	if (evoConditionSurface_cache[selectedIndex][5]) {
		SDL_Rect timeIcon;
		timeIcon.x = pokeRect.x + pokeRect.w;
		timeIcon.y = pokeRect.y + pokeRect.h;
		timeIcon.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][5]->w * 1.5);
		timeIcon.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][5]->h * 1.5);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][5], &timeIcon);
	}

	// HAPPINESS
	if (evoConditionSurface_cache[selectedIndex][6]) {
		SDL_Rect happinessIconRect;
		happinessIconRect.x = triggerIconRect.x;
		happinessIconRect.y = evo_item_y;
		happinessIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][6]->w * 1.5);
		happinessIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][6]->h * 1.5);

		SDL_Rect happinessRect;
		happinessRect.x = happinessIconRect.x + happinessIconRect.w;
		happinessRect.y = happinessIconRect.y;
		happinessRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][7]->w);
		happinessRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][7]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][6], &happinessIconRect);
		evo_item_y = happinessIconRect.y + happinessIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][7], &happinessRect);
	}

	// BEAUTY
	if (evoConditionSurface_cache[selectedIndex][8]) {
		SDL_Rect beautyIconRect;
		beautyIconRect.x = triggerIconRect.x;
		beautyIconRect.y = evo_item_y;
		beautyIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][8]->w * 2);
		beautyIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][8]->h * 2);

		SDL_Rect beautyRect;
		beautyRect.x = beautyIconRect.x + beautyIconRect.w;
		beautyRect.y = beautyIconRect.y;
		beautyRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][9]->w);
		beautyRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][9]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][8], &beautyIconRect);
		evo_item_y = beautyIconRect.y + beautyIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][9], &beautyRect);
	}

	// AFFECTION
	if (evoConditionSurface_cache[selectedIndex][10]) {
		SDL_Rect affectionIconRect;
		affectionIconRect.x = triggerIconRect.x;
		affectionIconRect.y = evo_item_y;
		affectionIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][10]->w * 2);
		affectionIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][10]->h * 2);

		SDL_Rect affectionRect;
		affectionRect.x = affectionIconRect.x + affectionIconRect.w;
		affectionRect.y = affectionIconRect.y;
		affectionRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][11]->w);
		affectionRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][11]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][10], &affectionIconRect);
		evo_item_y = affectionIconRect.y + affectionIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][11], &affectionRect);
	}

	// KNOWN MOVE 
	if (evoConditionSurface_cache[selectedIndex][12]) {
		SDL_Rect knownMoveIconRect;
		knownMoveIconRect.x = triggerIconRect.x;
		knownMoveIconRect.y = evo_item_y;
		knownMoveIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][12]->w * 2);
		knownMoveIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][12]->h * 2);

		SDL_Rect knwonMoveRect;
		knwonMoveRect.x = knownMoveIconRect.x + knownMoveIconRect.w;
		knwonMoveRect.y = knownMoveIconRect.y;
		knwonMoveRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][13]->w);
		knwonMoveRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][13]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][12], &knownMoveIconRect);
		evo_item_y = knownMoveIconRect.y + knownMoveIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][13], &knwonMoveRect);
	}

	// MOVE TYPE
	if (evoConditionSurface_cache[selectedIndex][14]) {
		SDL_Rect moveTypeIconRect;
		moveTypeIconRect.x = triggerIconRect.x;
		moveTypeIconRect.y = evo_item_y;
		moveTypeIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][14]->w * 2);
		moveTypeIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][14]->h * 2);

		SDL_Rect moveTypeRect;
		moveTypeRect.x = moveTypeIconRect.x + moveTypeIconRect.w;
		moveTypeRect.y = moveTypeIconRect.y;
		moveTypeRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][15]->w);
		moveTypeRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][15]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][14], &moveTypeIconRect);
		evo_item_y = moveTypeIconRect.y + moveTypeIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][15], &moveTypeRect);
	}

	// LOCATION
	if (evoConditionSurface_cache[selectedIndex][16]) {
		SDL_Rect locationIconRect;
		locationIconRect.x = triggerIconRect.x;
		locationIconRect.y = evo_item_y;
		locationIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][16]->w * 2);
		locationIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][16]->h * 2);

		SDL_Rect locationRect;
		locationRect.x = locationIconRect.x + locationIconRect.w;
		locationRect.y = locationIconRect.y;
		locationRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][17]->w);
		locationRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][17]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][16], &locationIconRect);
		evo_item_y = locationIconRect.y + locationIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][17], &locationRect);
	}

	// RELATIVE STATS
	if (evoConditionSurface_cache[selectedIndex][18]) {
		SDL_Rect atkIconRect;
		atkIconRect.x = triggerIconRect.x;
		atkIconRect.y = evo_item_y;
		atkIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][18]->w * 0.7);
		atkIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][18]->h * 0.7);

		SDL_Rect defIconRect;
		defIconRect.x = atkIconRect.x + atkIconRect.w + 60;
		defIconRect.y = evo_item_y;
		defIconRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][19]->w * 0.7);
		defIconRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][19]->h * 0.7);

		SDL_Rect relativeStatRect;
		relativeStatRect.x = triggerIconRect.x;
		relativeStatRect.y = atkIconRect.y + atkIconRect.h;
		relativeStatRect.w = static_cast<int>(evoConditionSurface_cache[selectedIndex][20]->w);
		relativeStatRect.h = static_cast<int>(evoConditionSurface_cache[selectedIndex][20]->h);
		
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][18], &atkIconRect);
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][19], &defIconRect);
    	PokeSurface::onDrawScaled(surf_display, evoConditionSurface_cache[selectedIndex][20], &relativeStatRect);

		evo_item_y = relativeStatRect.y + relativeStatRect.h;
	}

    return true;
}

PokedexActivity_PokemonView_Evolution* PokedexActivity_PokemonView_Evolution::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Evolution::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;
	
    if (selectedIndex > 0) {
        selectedIndex--;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex < offset) {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonDown(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;
	
    if (selectedIndex < evoChain->size() - 1) {
        selectedIndex++;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex - offset >= 3) {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Evolution::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonA(SDL_Keycode sym, Uint16 mod) {
    std::cout << evo[2] << '\n';
    PokedexDB::setPokemonID(std::stoi(evo[1]));
    PokedexDB::setPokemonIdentifier(evo[2]);
    PokedexDB::setVersionID(std::stoi(evo[22]));
    PokedexDB::setVersionGroupID(std::stoi(evo[23]));
    PokedexDB::setGenerationID(std::stoi(evo[24]));

    ////Call next activity
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Evolution::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Evolution::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
