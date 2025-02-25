#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Stats.h"

PokedexActivity_PokemonView_Stats PokedexActivity_PokemonView_Stats::instance;

PokedexActivity_PokemonView_Stats::PokedexActivity_PokemonView_Stats() : 
pokemon(nullptr),
dbResults(nullptr),
backgroundSurface(nullptr),
iconSurface(nullptr),
idSurface(nullptr),
nameSurface(nullptr),
abilitySurface(nullptr),
h_abilitySurface(nullptr),
sEffect(nullptr),
needRedraw(true)
{
};

PokedexActivity_PokemonView_Stats::~PokedexActivity_PokemonView_Stats(){
    // in order to play sounds asynchrounously with activity, 
    // we cant call Mix_FreeChunk immediately after playing.. should probably make seperate class for this. 
    if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;

}

void PokedexActivity_PokemonView_Stats::printPokeInfo(){
    std::vector<unsigned short> stats = pokemon->getBasicStats();
    
    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Defense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';
}

bool PokedexActivity_PokemonView_Stats::initSDL(){
    std::cout << "PokedexActivity_PokemonView_Stats::initSDL END \n";

	try {
		std::string path = SOUND_EFFECT_PATH;
		sEffect = Mix_LoadWAV(path.c_str());
		if (!sEffect) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load sEffect mix! SDL Error:  ") + Mix_GetError());
		}

		TTF_Font* temp_font = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
		if (temp_font == NULL) {
			std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		// Background Surface
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to render backgroundSurface! SDL Error:  ") + SDL_GetError());
		};
		backgroundRect.x = 0;
		backgroundRect.y = 0;
		backgroundRect.w = WINDOW_WIDTH;
		backgroundRect.h = WINDOW_HEIGHT;

		// Pokemon Sprite
		std::string spritePath = 
			SPRITE_IMG_BASE_PATH + 
			PokedexDB::getPokemonIdentifier() +
			".png";
		iconSurface = PokeSurface::onLoadImg(spritePath);
		if (iconSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to render iconSurface! SDL Error: ") + SDL_GetError());
		};
		iconRect.x = 50;
		iconRect.y = 100;   
		iconRect.w = iconSurface->w * 2;
		iconRect.h = iconSurface->h * 2;

		// ID
		// make it a 3 digit
		std::stringstream formattedID;
		formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
		std::string pokeID = formattedID.str();

	 	idSurface = TTF_RenderUTF8_Blended(
			temp_font,
			pokeID.c_str(),
			{ 96, 96, 96 }
		);
		if (idSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load idSurface! SDL Error: ") + SDL_GetError());
		};
		idRect.x = 10;
		idRect.y = 60;
		idRect.w = idSurface->w;
		idRect.h = idSurface->h;

		// Pokemon Name
		nameSurface = TTF_RenderUTF8_Blended(
			temp_font,
			pokemon->getName().c_str(),
			{ 96, 96, 96 }
		);
		if (nameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load nameSurface! SDL Error: ") + SDL_GetError());
		};

		nameRect.x = idRect.x + idRect.w + 10;
		nameRect.y = idRect.y;
		nameRect.w = nameSurface->w;
		nameRect.h = nameSurface->h;

		// Pokemon Stats
		statsNameSurface_cache.clear();
		statsSurface_cache.clear();

		std::vector<unsigned short> stats = pokemon->getBasicStats();
		for (size_t i = 0; i < (*statNames).size(); ++i) {
			SDL_Surface* statNameSurface = TTF_RenderUTF8_Blended(
				temp_font,
				(*statNames)[i][2].c_str(),
				{ 248, 248, 248 }
			);
			if (statNameSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load statNameSurface! SDL Error: ") + SDL_GetError());
			};

			/////////////////////////////////////////////
			//std::string statIconPath = "res/assets/misc/" + statFileNames[i] + "_icon_HOME.png";
			//SDL_Surface* statIcon = PokeSurface::onLoadImg(statIconPath);
			//if(!statIcon){
			//    std::cout << "Unable to load statIcon! SDL Error: statIcon " << SDL_GetError() << std::endl;
			//    exit(EXIT_FAILURE);
			//}

			////int topBorder = 80, spacing = 14;
			//SDL_Rect statIconRect;
			//statIconRect.x = WINDOW_WIDTH/2 + 150;
			//statIconRect.y = static_cast<int>(topBorder + (i * (statIcon->h*.5 + spacing)));
			//statIconRect.w = static_cast<int>(statIcon->w * .5);
			//statIconRect.h = static_cast<int>(statIcon->w * .5);
			//
			//PokeSurface::onDrawScaled(surf_display, statIcon, &statIconRect);
			//SDL_FreeSurface(statIcon);
			////////////////////////////////////////////////

			SDL_Surface* statSurface = TTF_RenderUTF8_Blended(
				temp_font,
				std::to_string(stats[i]).c_str(),
				{ 96, 96, 96 }
			);
			if (statSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load statSurface! SDL Error: ") + SDL_GetError());
			};

			statsNameSurface_cache.push_back(statNameSurface);
			statsSurface_cache.push_back(statSurface);
		}

		// Pokemon Abilities
		std::vector<std::vector<std::string>>* abilities = pokemon->getAbilities();
		if (!abilities->empty()) {
			std::string ability = (*abilities)[0][0] + "    " + (*abilities)[0][1];
			abilitySurface = TTF_RenderUTF8_Blended_Wrapped(
				temp_font,
				ability.c_str(),
				{ 96, 96, 96 },
				520
			);
			if (abilitySurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load abilitySurface! SDL Error: ") + SDL_GetError());
			};

			int topBorder = 80, spacing = 14;
			abilityRect.x = 130;
			abilityRect.y = WINDOW_HEIGHT/2 + 80;
			abilityRect.w = abilitySurface->w;
			abilityRect.h = abilitySurface->h;

			if (abilities->size() > 1) { // has hidden ability
				ability = (*abilities)[1][0] + "    " + (*abilities)[1][1];
				h_abilitySurface = TTF_RenderUTF8_Blended_Wrapped(
					temp_font,
					ability.c_str(),
					{ 96, 96, 96 },
					620
				);
				if (h_abilitySurface == NULL) {
					throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to load h_abilitySurface! SDL Error: ") + SDL_GetError());
				};

				h_abilityRect.x = abilityRect.x;
				h_abilityRect.y = WINDOW_HEIGHT - 80;
				h_abilityRect.w = h_abilitySurface->w;
				h_abilityRect.h = h_abilitySurface->h;
			}
		}
	} 
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return true;

    std::cout << "PokedexActivity_PokemonView_Stats::initSDL END \n";
}

void PokedexActivity_PokemonView_Stats::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Stats::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
	printPokeInfo();

	statNames = PokedexDB::executeSQL(&SQL_getStatNames);
    
	if(!initSDL()){
		std::cout << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}

	needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Stats::onActivate END \n";
}

void PokedexActivity_PokemonView_Stats::onDeactivate() {
	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);

	if(iconSurface)
    	SDL_FreeSurface(iconSurface);

	if(idSurface)
    	SDL_FreeSurface(idSurface);

	if(nameSurface)
		SDL_FreeSurface(nameSurface);
	
	for(SDL_Surface* surface : statsNameSurface_cache)
		if(surface)
			SDL_FreeSurface(surface);

	for(SDL_Surface* surface : statsSurface_cache)
		if(surface)
			SDL_FreeSurface(surface);

	statsNameSurface_cache.clear();
	statsSurface_cache.clear();

	if(abilitySurface)
		SDL_FreeSurface(abilitySurface);

	if(h_abilitySurface)
		SDL_FreeSurface(h_abilitySurface);

    delete pokemon;
	pokemon = nullptr;
}

void PokedexActivity_PokemonView_Stats::onLoop() {
}

void PokedexActivity_PokemonView_Stats::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
	if(needRedraw){
		// Clear the display surface
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

		try{
			// Render background
			PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
			std::cout << "Rendered Background" << std::endl;

			// Render Pokemon sprites
			PokeSurface::onDrawScaled(surf_display, iconSurface, &iconRect);
			std::cout << "Rendered sprite" << std::endl;

			// Render Pokemon ID
			PokeSurface::onDraw(surf_display, idSurface, &idRect);
			std::cout << "Rendered id" << std::endl;


			// Render Pokemon Name
			PokeSurface::onDrawScaled(surf_display, nameSurface, &nameRect);
			std::cout << "Rendered name" << std::endl;

			// Pokemon Stats
			// When ready, Evasion && Accuracy can be included as part of stats
			// To do so use statNames 
			// Using the Basic stats for now. 
			for(size_t i = 0; i < 6; i++){

				int topBorder = 80, spacing = 14;
				statsNameRect.x = (WINDOW_WIDTH / 2) + 20;
				statsNameRect.y = ((statsNameSurface_cache[i]->h + 5) * i) + topBorder;
				statsNameRect.w = statsNameSurface_cache[i]->w;
				statsNameRect.h = statsNameSurface_cache[i]->h;

				statsRect.x = (WINDOW_WIDTH - statsSurface_cache[i]->w) - 10;
				statsRect.y = (38 * i) + 80;
				statsRect.w = statsSurface_cache[i]->w;
				statsRect.h = statsSurface_cache[i]->h;

				PokeSurface::onDrawScaled(surf_display, statsNameSurface_cache[i], &statsNameRect);

				PokeSurface::onDrawScaled(surf_display, statsSurface_cache[i], &statsRect);
			}
			std::cout << "Rendered stats" << std::endl;

			if (!pokemon->getAbilities()->empty()) {
				PokeSurface::onDrawScaled(surf_display, abilitySurface, &abilityRect);
				std::cout << "Rendered ability" << std::endl;

				if (pokemon->getAbilities()->size() > 1) {
					PokeSurface::onDrawScaled(surf_display, h_abilitySurface, &h_abilityRect);
					std::cout << "Rendered h_ability" << std::endl;
				}
			}
		} 
		catch(const std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
		needRedraw = false;
	}
}

void PokedexActivity_PokemonView_Stats::onFreeze(){
}

PokedexActivity_PokemonView_Stats* PokedexActivity_PokemonView_Stats::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Stats::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonLeft(SDL_Keycode sym, Uint16 mod){
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod){
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Stats::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonB(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Stats::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
