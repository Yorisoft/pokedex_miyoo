#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity_PokemonView_Evolution PokedexActivity_PokemonView_Evolution::instance;

PokedexActivity_PokemonView_Evolution::PokedexActivity_PokemonView_Evolution() :
pokemon(nullptr),
se_up_down(nullptr),
se_left_right(nullptr),
backgroundSurface(nullptr),
needRedraw(true),
selectedIndex(0),
offset(0)
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

			// METHOD
			/* std::string method = (*evoChain)[i][4]; */
			/* if (method == "NULL") { */
			/* 	method = "__"; */
			/* } */
			/* else if (method == "level-up" && (*evoChain)[i][5] != "NULL") { // BY LEVEL */
			/* 	method = "res/assets/pokemons/encounters/" + method + ".png"; */
			/* } */
			/* else if (method == "use-item") { // BY ITEM */
			/* 	method = "res/assets/items/" + (*evoChain)[i][6] + ".png"; */
			/* } */
			/* else if (method == "trade") { // BY ITEM */
			/* 	method = "res/assets/pokemons/encounters/" + method + ".png"; */
			/* } */
			/* else if ((*evoChain)[i][8] != "NULL") { // BY HAPPYNESS */
			/* 	method = "happiness"; */
			/* 	method = "res/assets/pokemons/encounters/" + method + ".png"; */
			/* } */
			/* //else if (method == "level-up" && (*evoChain)[i][7] != "NULL") { // BY TIME OF DAY */
			/* //    method = "res/assets/pokemons/encounters/" + method + ".png"; */
			/* //} */
			/* //if (method == "Level up" && (*evoChain)[i][7] != "NULL") { */
			/* //    method = "Time: " + (*evoChain)[i][7]; */
			/* //    if ((*evoChain)[i][8] != "NULL") { */
			/* //        method += " Happiness: " + (*evoChain)[i][8]; */
			/* //    } */
			/* //} */
			/* //else if (method == "Level up" && (*evoChain)[i][8] != "NULL") { */
			/* //    method = "Happiness: " + (*evoChain)[i][8]; */
			/* //} */

			/* SDL_Surface* methodSurface; */
			/* SDL_Surface* methodInfoSurface; */
			/* SDL_Rect methodRect; */
			/* if (method == "__") { */
			/* 	methodSurface = TTF_RenderUTF8_Blended( */
			/* 		fontSurface, */
			/* 		method.c_str(), */
			/* 		{ 96, 96, 96 } */
			/* 	); */
			/* } */
			/* else { */
			/* 	methodSurface = PokeSurface::onLoadImg(method); */
			/* } */
			/* if (methodSurface == NULL ) { */
			/* 	throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load methodSurface! SDL Error:  ") + SDL_GetError()); */
			/* }; */

			/* std::string methodInfo = (*evoChain)[i][5] != "NULL" ? (*evoChain)[i][5] : (*evoChain)[i][8]; */
			/* methodInfoSurface = TTF_RenderUTF8_Blended( */
			/* 	fontSurface, */
			/* 	methodInfo.c_str(), */
			/* 	{ 96, 96, 96 } */
			/* ); */
			/* if (methodInfoSurface == NULL ) { */
			/* 	std::cout << "Unable to render text! SDL Error: methodSurfaceInfo" << TTF_GetError() << std::endl; */
			/* 	exit(EXIT_FAILURE); */
			/* }; */

			pokeSprite_cache.push_back(pokeSurface);
			idSurface_cache.push_back(idSurface);
			nameSurface_cache.push_back(nameSurface);
			/* methodSurface_cache.push_back(methodSurface); */
			/* methodInfoSurface_cache.push_back(methodInfoSurface); */
		}
	}
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

void PokedexActivity_PokemonView_Evolution::clearCachedSurfaces(){
	for(SDL_Surface* surface : pokeSprite_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : idSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : nameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : methodSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : methodInfoSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	pokeSprite_cache.clear();
	idSurface_cache.clear();
	nameSurface_cache.clear();
	methodSurface_cache.clear();
	methodInfoSurface_cache.clear();
}

void PokedexActivity_PokemonView_Evolution::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate START \n";

    pokemon = new Pokemon();
	evoChain = pokemon->getEvolutionChain();
    printPokeInfo();

    evo = (*evoChain)[selectedIndex];

	needRedraw = true;

	clearCachedSurfaces();
	if(!initSDL()){
		std::cerr << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
	
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate END \n";
}

void PokedexActivity_PokemonView_Evolution::onDeactivate() {
	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);

	if(listEntrySurface_default)
    	SDL_FreeSurface(listEntrySurface_default);
	listEntrySurface_default = nullptr;

	if(listEntrySurface_selected)
    	SDL_FreeSurface(listEntrySurface_selected);
	listEntrySurface_selected = nullptr;

	for(SDL_Surface* surface : pokeSprite_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : idSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : nameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : methodSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : methodInfoSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	pokeSprite_cache.clear();
	idSurface_cache.clear();
	nameSurface_cache.clear();
	methodSurface_cache.clear();
	methodInfoSurface_cache.clear();

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;
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
	}
}

bool PokedexActivity_PokemonView_Evolution::renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i) {
	// ListEntry
	int spacing = 15; 
	listEntryRect = {
		static_cast<int>(WINDOW_WIDTH * 0.5),
		50 + (i * (ITEM_HEIGHT + spacing)),
		static_cast<int>(WINDOW_WIDTH * 0.5),
		ITEM_HEIGHT + 10
	};
	
	/* int spacing = 5; */ 
	/* listEntryRect.h = ITEM_HEIGHT; */
	/* double heightRatio = static_cast<double>(listEntryRect.h) / static_cast<double>(listEntrySurface_default->h); */
	/* listEntryRect.x = static_cast<int>(WINDOW_WIDTH * 0.5); */
	/* listEntryRect.y = 65 + (i * (ITEM_HEIGHT + spacing)); */
	/* listEntryRect.w = static_cast<int>(heightRatio * listEntrySurface_default->w); */
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

    if (offset + i == selectedIndex) {
        if (!renderPokeInfo(surf_display, font, i)) {
            std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

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
	std::string trigger = (*evoChain)[offset + i][4];
	SDL_Surface* triggerSurface;
	if (trigger == "NULL") {
		trigger = "__";
		triggerSurface = TTF_RenderUTF8_Blended(
			font,
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
	
	SDL_Rect triggerIconRect = {
		0,
		pokeRect.y + pokeRect.h,
		static_cast<int>(triggerSurface->w * 1.5),
		static_cast<int>(triggerSurface->h * 1.5)
	};
    PokeSurface::onDrawScaled(surf_display, triggerSurface, &triggerIconRect);
	SDL_FreeSurface(triggerSurface);
	evo_item_y = triggerIconRect.y + triggerIconRect.h;


	// LEVEL
	std::string level = (*evoChain)[offset + i][6];
	if (level != "NULL") {
		level = " + " + level;
			SDL_Surface* levelSurface = TTF_RenderUTF8_Blended(
			font,
			level.c_str(),
			{ 96, 96, 96 }
		);
		if (levelSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		int leftPadding = 5;
		SDL_Rect levelRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(levelSurface->w),
			static_cast<int>(levelSurface->h)
		};
    	PokeSurface::onDrawScaled(surf_display, levelSurface, &levelRect);
		SDL_FreeSurface(levelSurface);
	}


	// ITEMS
	// USE ITEM
	std::string useItem = (*evoChain)[offset + i][5];
	if (useItem != "NULL") {
			useItem = "res/assets/items/" + useItem + ".png";
			SDL_Surface* useItemSurface = PokeSurface::onLoadImg(useItem);
		if (useItemSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		int leftPadding = 5;
		SDL_Rect itemRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(useItemSurface->w * 1.5),
			static_cast<int>(useItemSurface->h * 1.5)
		};
    	PokeSurface::onDrawScaled(surf_display, useItemSurface, &itemRect);
		SDL_FreeSurface(useItemSurface);
	}

	// TRADE ITEM
	std::string heldItem = (*evoChain)[offset + i][9];
	if (heldItem != "NULL") {
			heldItem = "res/assets/items/" + heldItem + ".png";
			SDL_Surface* heldItemSurface = PokeSurface::onLoadImg(heldItem);
		if (heldItemSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		int leftPadding = 5;
		SDL_Rect itemRect = {
			triggerIconRect.x + triggerIconRect.w + leftPadding,
			triggerIconRect.y,
			static_cast<int>(heldItemSurface->w * 1.5),
			static_cast<int>(heldItemSurface->h * 1.5)
		};
    	PokeSurface::onDrawScaled(surf_display, heldItemSurface, &itemRect);
		SDL_FreeSurface(heldItemSurface);
	}
	
	// GENDER
	std::string genderID = (*evoChain)[offset + i][7];
	SDL_Surface* genderSurface;
	if (genderID != "NULL") {
			genderID = "res/assets/pokemons/gender/" + genderID + ".png";
			genderSurface = PokeSurface::onLoadImg(genderID);
		if (genderSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		int leftPadding = 5;
		SDL_Rect genderRect;
		genderRect.x = triggerIconRect.x;
		genderRect.y = evo_item_y;
		genderRect.h = static_cast<int>(genderSurface->w);
		genderRect.w = static_cast<int>(genderSurface->h);

    	PokeSurface::onDrawScaled(surf_display, genderSurface, &genderRect);
		SDL_FreeSurface(genderSurface);
		evo_item_y = genderRect.y + genderRect.h;
	}

	// TIME
	std::string time = (*evoChain)[offset + i][10];
	if (time != "NULL") {
			time = "res/assets/pokemons/encounters/" + time + ".png";
			SDL_Surface* timeIconSurface = PokeSurface::onLoadImg(time);
		if (timeIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect timeIcon;
		timeIcon.x = pokeRect.x + pokeRect.w;
		timeIcon.y = pokeRect.y + pokeRect.h;
		timeIcon.h = static_cast<int>(timeIconSurface->w * 1.5);
		timeIcon.w = static_cast<int>(timeIconSurface->h * 1.5);
		
    	PokeSurface::onDrawScaled(surf_display, timeIconSurface, &timeIcon);
		SDL_FreeSurface(timeIconSurface);
	}

	// HAPPINESS
	std::string happiness = (*evoChain)[offset + i][13];
	if (happiness != "NULL") {
		happiness = " + " + happiness;
		std::string happinessPath = "res/assets/pokemons/encounters/happiness.png";
		SDL_Surface* happinessIconSurface = PokeSurface::onLoadImg(happinessPath);
		if (happinessIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* happinessSurface = TTF_RenderUTF8_Blended(
			font,
			happiness.c_str(),
			{ 96, 96, 96 }
		);
		if (happinessSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect happinessIconRect;
		happinessIconRect.x = triggerIconRect.x;
		happinessIconRect.y = evo_item_y;
		happinessIconRect.w = static_cast<int>(happinessIconSurface->w * 1.5);
		happinessIconRect.h = static_cast<int>(happinessIconSurface->h * 1.5);

		SDL_Rect happinessRect;
		happinessRect.x = happinessIconRect.x + happinessIconRect.w;
		happinessRect.y = happinessIconRect.y;
		happinessRect.w = static_cast<int>(happinessSurface->w);
		happinessRect.h = static_cast<int>(happinessSurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, happinessIconSurface, &happinessIconRect);
		SDL_FreeSurface(happinessIconSurface);
		evo_item_y = happinessIconRect.y + happinessIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, happinessSurface, &happinessRect);
		SDL_FreeSurface(happinessSurface);
	}

	// BEAUTY
	std::string beauty = (*evoChain)[offset + i][14];
	if (beauty != "NULL") {
		beauty = " + " + beauty;
		std::string beautyPath = "res/assets/pokemons/encounters/beauty.png";
		SDL_Surface* beautyIconSurface = PokeSurface::onLoadImg(beautyPath);
		if (beautyIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* beautySurface = TTF_RenderUTF8_Blended(
			font,
			beauty.c_str(),
			{ 96, 96, 96 }
		);
		if (beautySurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect beautyIconRect;
		beautyIconRect.x = triggerIconRect.x;
		beautyIconRect.y = evo_item_y;
		beautyIconRect.w = static_cast<int>(beautyIconSurface->w * 2);
		beautyIconRect.h = static_cast<int>(beautyIconSurface->h * 2);

		SDL_Rect beautyRect;
		beautyRect.x = beautyIconRect.x + beautyIconRect.w;
		beautyRect.y = beautyIconRect.y;
		beautyRect.w = static_cast<int>(beautySurface->w);
		beautyRect.h = static_cast<int>(beautySurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, beautyIconSurface, &beautyIconRect);
		SDL_FreeSurface(beautyIconSurface);
		evo_item_y = beautyIconRect.y + beautyIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, beautySurface, &beautyRect);
		SDL_FreeSurface(beautySurface);
	}

	// AFFECTION
	std::string affection = (*evoChain)[offset + i][14];
	if (affection != "NULL") {
		affection = " + " + affection;
		std::string affectionPath = "res/assets/pokemons/encounters/affection.png";
		SDL_Surface* affectionIconSurface = PokeSurface::onLoadImg(affectionPath);
		if (affectionIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* affectionSurface = TTF_RenderUTF8_Blended(
			font,
			affection.c_str(),
			{ 96, 96, 96 }
		);
		if (affectionSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect affectionIconRect;
		affectionIconRect.x = triggerIconRect.x;
		affectionIconRect.y = evo_item_y;
		affectionIconRect.w = static_cast<int>(affectionIconSurface->w * 2);
		affectionIconRect.h = static_cast<int>(affectionIconSurface->h * 2);

		SDL_Rect affectionRect;
		affectionRect.x = affectionIconRect.x + affectionIconRect.w;
		affectionRect.y = affectionIconRect.y;
		affectionRect.w = static_cast<int>(affectionSurface->w);
		affectionRect.h = static_cast<int>(affectionSurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, affectionIconSurface, &affectionIconRect);
		SDL_FreeSurface(affectionIconSurface);
		evo_item_y = affectionIconRect.y + affectionIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, affectionSurface, &affectionRect);
		SDL_FreeSurface(affectionSurface);
	}

	// KNOWN MOVE 
	std::string knownMove = (*evoChain)[offset + i][11];
	if (knownMove != "NULL") {
		knownMove = " + " + knownMove;
		std::string knownMovePath = "res/assets/pokemons/encounters/tutor.png";
		SDL_Surface* knownMoveIconSurface = PokeSurface::onLoadImg(knownMovePath);
		if (knownMoveIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* knownMoveSurface = TTF_RenderUTF8_Blended(
			font,
			knownMove.c_str(),
			{ 96, 96, 96 }
		);
		if (knownMoveSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect knownMoveIconRect;
		knownMoveIconRect.x = triggerIconRect.x;
		knownMoveIconRect.y = evo_item_y;
		knownMoveIconRect.w = static_cast<int>(knownMoveIconSurface->w * 2);
		knownMoveIconRect.h = static_cast<int>(knownMoveIconSurface->h * 2);

		SDL_Rect knwonMoveRect;
		knwonMoveRect.x = knownMoveIconRect.x + knownMoveIconRect.w;
		knwonMoveRect.y = knownMoveIconRect.y;
		knwonMoveRect.w = static_cast<int>(knownMoveSurface->w);
		knwonMoveRect.h = static_cast<int>(knownMoveSurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, knownMoveIconSurface, &knownMoveIconRect);
		SDL_FreeSurface(knownMoveIconSurface);
		evo_item_y = knownMoveIconRect.y + knownMoveIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, knownMoveSurface, &knwonMoveRect);
		SDL_FreeSurface(knownMoveSurface);
	}


	// MOVE TYPE
	std::string moveType = (*evoChain)[offset + i][12];
	if (moveType != "NULL") {
		moveType = " + " + moveType;
		std::string moveTypePath = "res/assets/pokemons/encounters/tutor.png";
		SDL_Surface* moveTypeIconSurface = PokeSurface::onLoadImg(moveTypePath);
		if (moveTypeIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* moveTypeSurface = TTF_RenderUTF8_Blended(
			font,
			moveType.c_str(),
			{ 96, 96, 96 }
		);
		if (moveTypeSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect moveTypeIconRect;
		moveTypeIconRect.x = triggerIconRect.x;
		moveTypeIconRect.y = evo_item_y;
		moveTypeIconRect.w = static_cast<int>(moveTypeIconSurface->w * 2);
		moveTypeIconRect.h = static_cast<int>(moveTypeIconSurface->h * 2);

		SDL_Rect moveTypeRect;
		moveTypeRect.x = moveTypeIconRect.x + moveTypeIconRect.w;
		moveTypeRect.y = moveTypeIconRect.y;
		moveTypeRect.w = static_cast<int>(moveTypeSurface->w);
		moveTypeRect.h = static_cast<int>(moveTypeSurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, moveTypeIconSurface, &moveTypeIconRect);
		SDL_FreeSurface(moveTypeIconSurface);
		evo_item_y = moveTypeIconRect.y + moveTypeIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, moveTypeSurface, &moveTypeRect);
		SDL_FreeSurface(moveTypeSurface);
	}

	// LOCATION
	std::string location = (*evoChain)[offset + i][8];
	if (location != "NULL") {
		std::string locationPath = "res/assets/pokemons/encounters/location.png";
		SDL_Surface* locationIconSurface = PokeSurface::onLoadImg(locationPath);
		if (locationIconSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		SDL_Surface* locationSurface = TTF_RenderUTF8_Blended(
			font,
			location.c_str(),
			{ 96, 96, 96 }
		);
		if (locationSurface == NULL ) {
			std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Rect locationIconRect;
		locationIconRect.x = triggerIconRect.x;
		locationIconRect.y = evo_item_y;
		locationIconRect.w = static_cast<int>(locationIconSurface->w * 2);
		locationIconRect.h = static_cast<int>(locationIconSurface->h * 2);

		SDL_Rect locationRect;
		locationRect.x = locationIconRect.x + locationIconRect.w;
		locationRect.y = locationIconRect.y;
		locationRect.w = static_cast<int>(locationSurface->w);
		locationRect.h = static_cast<int>(locationSurface->h);
		
    	PokeSurface::onDrawScaled(surf_display, locationIconSurface, &locationIconRect);
		SDL_FreeSurface(locationIconSurface);
		evo_item_y = locationIconRect.y + locationIconRect.h;

    	PokeSurface::onDrawScaled(surf_display, locationSurface, &locationRect);
		SDL_FreeSurface(locationSurface);
	}


	/* else if (trigger == "level-up" && (*evoChain)[offset + i][5] != "NULL") { // BY LEVEL */
	/* 	trigger = "res/assets/pokemons/encounters/" + trigger + ".png"; */
	/* } */
	/* else if (trigger == "use-item") { // BY ITEM */
	/* 	trigger = "res/assets/items/" + (*evoChain)[offset + i][6] + ".png"; */
	/* } */
	/* else if (trigger == "trade") { // BY ITEM */
	/* 	trigger = "res/assets/pokemons/encounters/" + trigger + ".png"; */
	/* } */
	/* else if ((*evoChain)[offset + i][8] != "NULL") { // BY HAPPYNESS */
	/* 	trigger = "happiness"; */
	/* 	trigger = "res/assets/pokemons/encounters/" + trigger + ".png"; */
	/* } */
	//else if (method == "level-up" && (*evoChain)[offset + i][7] != "NULL") { // BY TIME OF DAY
	//    method = "res/assets/pokemons/encounters/" + method + ".png";
	//}
	//if (method == "Level up" && (*evoChain)[offset + i][7] != "NULL") {
	//    method = "Time: " + (*evoChain)[offset + i][7];
	//    if ((*evoChain)[offset + i][8] != "NULL") {
	//        method += " Happiness: " + (*evoChain)[offset + i][8];
	//    }
	//}
	//else if (method == "Level up" && (*evoChain)[offset + i][8] != "NULL") {
	//    method = "Happiness: " + (*evoChain)[offset + i][8];
	//}

    /* // */
    /* if ((*evoChain)[offset + i][5] != "NULL" || (*evoChain)[offset + i][8] != "NULL") { */
		/* methodInfoRect = { */
			/* methodRect.x + methodRect.w, */
			/* nameRect.y + nameRect.h + 10, */
			/* static_cast<int>(methodInfoSurface_cache[offset + i]->w * .9), */
			/* static_cast<int>(methodInfoSurface_cache[offset + i]->h * .9) */
		/* }; */

    /*     PokeSurface::onDrawScaled(surf_display, methodInfoSurface_cache[offset + i], &methodInfoRect); */
    /* } */

	/* // LEVEL */ 
	/* std::string method; */
	/* SDL_Surface* levelIconSurface = PokeSurface::onLoadImg(METHOD_IMG_BASE_PATH + "level-up.png"); */
	/* if (levelIconSurface == NULL) { */
		/* std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl; */
		/* exit(EXIT_FAILURE); */
	/* }; */
	/* SDL_Rect levelIconRect = { */
		/* 0, */
		/* pokeRect.y + pokeRect.h, */
		/* static_cast<int>(levelIconSurface->w * 1.5), */
		/* static_cast<int>(levelIconSurface->h * 1.5) */
	/* }; */
    /* PokeSurface::onDrawScaled(surf_display, levelIconSurface, &levelIconRect); */

	/* // TRADE */
	/* SDL_Surface* tradeIconSurface; */
	/* if((*evoChain)[offset + i][4] == "trade"){ */
		/* tradeIconSurface = PokeSurface::onLoadImg(METHOD_IMG_BASE_PATH + "trade.png"); */
		/* if (tradeIconSurface == NULL) { */
			/* std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl; */
			/* exit(EXIT_FAILURE); */
		/* }; */
	/* } */
	/* else { */
		/* tradeIconSurface = PokeSurface::onLoadImg(METHOD_IMG_BASE_PATH + "trade_na.png"); */
		/* if (tradeIconSurface == NULL) { */
			/* std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl; */
			/* exit(EXIT_FAILURE); */
		/* }; */
	/* } */
	/* SDL_Rect tradeIconRect = { */
		/* 0, */
		/* levelIconRect.y + levelIconRect.h, */
		/* static_cast<int>(tradeIconSurface->w * 1.5), */
		/* static_cast<int>(tradeIconSurface->h * 1.5) */
	/* }; */
	/* PokeSurface::onDrawScaled(surf_display, tradeIconSurface, &tradeIconRect); */

	/* // ITEM */
	/* SDL_Surface* itemIconSurface = PokeSurface::onLoadImg(METHOD_IMG_BASE_PATH + "item.png"); */
	/* if (itemIconSurface == NULL) { */
		/* std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl; */
		/* exit(EXIT_FAILURE); */
	/* }; */
	/* SDL_Rect itemIconRect = { */
		/* 0, */
		/* pokeRect.y + pokeRect.h, */
		/* static_cast<int>(itemIconSurface->w * 1.5), */
		/* static_cast<int>(itemIconSurface->h * 1.5) */
	/* }; */
    /* PokeSurface::onDrawScaled(surf_display, itemIconSurface, &itemIconRect); */




    return true;
}

PokedexActivity_PokemonView_Evolution* PokedexActivity_PokemonView_Evolution::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Evolution::onButtonUp(SDL_Keycode sym, Uint16 mod) {
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
    PokedexDB::setVersionID(std::stoi(evo[11]));
    PokedexDB::setVersionGroupID(std::stoi(evo[12]));
    PokedexDB::setGenerationID(std::stoi(evo[13]));

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
