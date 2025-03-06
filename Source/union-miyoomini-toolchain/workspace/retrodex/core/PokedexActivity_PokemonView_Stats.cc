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
se_left_right(nullptr),
needRedraw(true)
{
};

PokedexActivity_PokemonView_Stats::~PokedexActivity_PokemonView_Stats(){
    // in order to play sounds asynchrounously with activity, 
    // we cant call Mix_FreeChunk immediately after playing.. should probably make seperate class for this. 
    if(se_left_right)
        Mix_FreeChunk(se_left_right);
    se_left_right = nullptr;

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
		se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
		if (!se_left_right) {
			std::cerr << "Warning: PokedexActivity_PokemonView_Stats::initSDL() Unable to load se_left_right mix! SDL Error:  " << + Mix_GetError();
		}

		fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 34);
		if (fontSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Stats::initSDL() Unable to render fontSurface! SDL Error:  ") + TTF_GetError());
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
			fontSurface,
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
			fontSurface,
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
				fontSurface,
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
				fontSurface,
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
				fontSurface,
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
					fontSurface,
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
		return false;
	}

	return true;

    std::cout << "PokedexActivity_PokemonView_Stats::initSDL END \n";
}

void PokedexActivity_PokemonView_Stats::clearCachedSurfaces(){
	for(SDL_Surface* surface : statsNameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : statsSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	statsNameSurface_cache.clear();
	statsSurface_cache.clear();

}
void PokedexActivity_PokemonView_Stats::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Stats::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
	printPokeInfo();

	statNames = PokedexDB::executeSQL(&SQL_getStatNames);
    
	clearCachedSurfaces();

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
	backgroundSurface = nullptr;

	if(iconSurface)
    	SDL_FreeSurface(iconSurface);
	iconSurface = nullptr;

	if(idSurface)
    	SDL_FreeSurface(idSurface);
	idSurface = nullptr;

	if(nameSurface)
		SDL_FreeSurface(nameSurface);
	nameSurface = nullptr;
	
	if(abilitySurface)
		SDL_FreeSurface(abilitySurface);
	abilitySurface = nullptr;

	if(h_abilitySurface)
		SDL_FreeSurface(h_abilitySurface);
	h_abilitySurface = nullptr;

	for(SDL_Surface* surface : statsNameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : statsSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	statsNameSurface_cache.clear();
	statsSurface_cache.clear();

	if(fontSurface)
		TTF_CloseFont(fontSurface);
	fontSurface = nullptr;

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

			// Render Pokemon sprites
			PokeSurface::onDrawScaled(surf_display, iconSurface, &iconRect);

			// Render Pokemon ID
			PokeSurface::onDraw(surf_display, idSurface, &idRect);

			// Render Pokemon Name
			PokeSurface::onDrawScaled(surf_display, nameSurface, &nameRect);

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

			if (!pokemon->getAbilities()->empty()) {
				PokeSurface::onDrawScaled(surf_display, abilitySurface, &abilityRect);

				if (pokemon->getAbilities()->size() > 1) {
					PokeSurface::onDrawScaled(surf_display, h_abilitySurface, &h_abilityRect);
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
    Mix_PlayChannel(1, se_left_right, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod){
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);

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
