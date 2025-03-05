#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() : 
    dbResults(nullptr),
    backgroundSurface(nullptr),
    listBackgroundSurface_default(nullptr),
    listBackgroundSurface_selected(nullptr),
    se_up_down(nullptr),
    se_on_start(nullptr),
    se_on_exit(nullptr),
    needRedraw(true),
    selectedIndex(0),
    offset(0)
{
}

PokedexActivityList::~PokedexActivityList() {
    //Cant delete in onDeactivate or auido will be cut short
    // deleting here is not best practice, this will only ever be called once, 
    // onActivate/Deactivate will be called many times.
    if(se_on_exit)
        Mix_FreeChunk(se_on_exit);
    se_on_exit = nullptr;

}

bool PokedexActivityList::initSDL(){
	try{
		// Sound Effects
		se_on_start = Mix_LoadWAV(SOUND_EFFECT_ON_START_PATH.c_str());
		if (!se_on_start) {
			std::cerr << "Failed to load sound se_on_start: " << Mix_GetError() << std::endl;
		}

		se_on_exit = Mix_LoadWAV(SOUND_EFFECT_ON_EXIT_PATH.c_str());
		if (!se_on_exit) {
			std::cerr << "Failed to load sound se_on_exit: " << Mix_GetError() << std::endl;
		}

		se_up_down = Mix_LoadWAV(SOUND_EFFECT_UP_DOWN.c_str());
		if (!se_up_down) {
			std::cerr << "Failed to load sound se_up_down: " << Mix_GetError() << std::endl;
		}

		fontSurface = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
		if (fontSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load fontSurface! SDL Error:  ") + SDL_GetError());
		}

		//Background Surface
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
		};
		backgroundRect.x = 0;
		backgroundRect.y = 0;
		backgroundRect.w = WINDOW_WIDTH;
		backgroundRect.h = WINDOW_HEIGHT;

		// List Item Background Surface
		listBackgroundSurface_default = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_DEFAULT);
		if (listBackgroundSurface_default == NULL) {
			throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load listBackgroundSurface_default! SDL Error:  ") + SDL_GetError());
		};

		listBackgroundSurface_selected = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_SELECTED);
		if (listBackgroundSurface_selected == NULL) {
			throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load listBackgroundSurface_selected! SDL Error:  ") + SDL_GetError());
		};

		// Pokemons
		for(auto& poke : *dbResults){
			std::string spritePath =  SPRITES_IMG_BASE_PATH + poke[1] + ".png";
			SDL_Surface* pokeIconSurface = PokeSurface::onLoadImg(spritePath);
			if (pokeIconSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeIconSurface! SDL Error:  ") + SDL_GetError());
			};
			pokemonIconSurface_cache.push_back(pokeIconSurface);

			std::vector<SDL_Surface*> temp;
			std::string typePath = TYPES_IMG_BASE_PATH + poke[3] + ".png";
			SDL_Surface* pokeType1Surface = PokeSurface::onLoadImg(typePath);
			if (pokeType1Surface == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeType1Surface! SDL Error:  ") + SDL_GetError());
			};
			temp.push_back(pokeType1Surface);

			if (poke[4] != "NULL") {
				typePath = TYPES_IMG_BASE_PATH + poke[4] + ".png";
				SDL_Surface* pokeType2Surface = PokeSurface::onLoadImg(typePath);
				if (pokeType2Surface == NULL) {
					throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeType2Surface! SDL Error:  ") + SDL_GetError());
				};
				temp.push_back(pokeType2Surface);
			}
			pokemonTypeSurface_cache.emplace_back(temp);

			// ID && NAME
			temp.clear();
			SDL_Surface* pokeIDSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				poke[0].c_str(),
				COLOR
			);
			if (pokeIDSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeIDSurface! SDL Error:  ") + SDL_GetError());
			};
			temp.push_back(pokeIDSurface);

			std::string name = poke[2];
			/*for (int i = 0; i < name.size(); i++) {
				name[i] = std::toupper(name[i]);
			}*/
			SDL_Surface* pokeNameSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				name.c_str(),
				COLOR
			);
			if (pokeNameSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeNameSurface! SDL Error:  ") + SDL_GetError());
			};
			temp.push_back(pokeNameSurface);

			// ID && NAME HIGHLIGHTED
			SDL_Surface* pokeIDSurface_selected = TTF_RenderUTF8_Blended(
				fontSurface,
				poke[0].c_str(),
				HIGHLIGHT_COLOR
			);
			if (pokeIDSurface_selected == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeIDSurface_selected! SDL Error:  ") + SDL_GetError());
			};
			temp.push_back(pokeIDSurface_selected);

			SDL_Surface* pokeNameSurface_selected = TTF_RenderUTF8_Blended(
				fontSurface,
				name.c_str(),
				HIGHLIGHT_COLOR
			);
			if (pokeNameSurface_selected == NULL) {
				throw std::runtime_error(std::string("PokedexActivityList::initSDL() Unable to load pokeNameSurface_selected! SDL Error:  ") + SDL_GetError());
			};
			temp.push_back(pokeNameSurface_selected);

			pokemonIDNameSurface_cache.emplace_back(temp);
		}
	} 
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}
	
	return true;
}

void PokedexActivityList::print_dbResults(){
    for (auto& row : *dbResults) {
        for (auto& col : row) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivityList::clearCacheSurfaces(){
	if(!pokemonIconSurface_cache.empty())
		for(SDL_Surface* surface : pokemonIconSurface_cache)
			if(surface){
				SDL_FreeSurface(surface);
				surface = nullptr;
			}

	if(!pokemonTypeSurface_cache.empty())
		for(std::vector<SDL_Surface*> surfaces : pokemonTypeSurface_cache)
			if(!surfaces.empty())
				for(SDL_Surface* surface : surfaces)
					if(surface){
						SDL_FreeSurface(surface);
						surface = nullptr;
					}

	if(!pokemonIDNameSurface_cache.empty())
		for(std::vector<SDL_Surface*> surfaces : pokemonIDNameSurface_cache)
			if(!surfaces.empty())
				for(SDL_Surface* surface : surfaces)
					if(surface){
						SDL_FreeSurface(surface);
						surface = nullptr;
					}

	pokemonIconSurface_cache.clear();
	pokemonTypeSurface_cache.clear();
	pokemonIDNameSurface_cache.clear();
}

void PokedexActivityList::onActivate() {
    std::cout << "PokedexActivityList::onActivate START \n";

	// Pokemon List DB Results
    dbResults = PokedexDB::executeSQL(&SQL_getNameAndID);
    pokemon = (*dbResults)[selectedIndex];
	print_dbResults();

	clearCacheSurfaces();
	if(!initSDL()){
		std::cerr << "PokedexActivityList::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}

    Mix_PlayChannel(-1, se_on_start, 0);

    needRedraw = true;

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate() {
	if(backgroundSurface)
    	SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

	if(listBackgroundSurface_default)
		SDL_FreeSurface(listBackgroundSurface_default);
	 	listBackgroundSurface_default = nullptr;

	if(listBackgroundSurface_selected)
		SDL_FreeSurface(listBackgroundSurface_selected);
	listBackgroundSurface_default = nullptr;

	for(SDL_Surface* icon : pokemonIconSurface_cache)
		if(icon){
    		SDL_FreeSurface(icon);
			icon = nullptr;
		}

	for(std::vector<SDL_Surface*> types : pokemonTypeSurface_cache)
		for(SDL_Surface* type : types)
			if(type){
				SDL_FreeSurface(type);
				type = nullptr;
			}

	for(std::vector<SDL_Surface*> ids_names : pokemonIDNameSurface_cache)
		for(SDL_Surface* id_name : ids_names)
			if(id_name){
				SDL_FreeSurface(id_name);
				id_name = nullptr;
			}

    if(se_on_start)
        Mix_FreeChunk(se_on_start);
    se_on_start = nullptr;

    if(se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;

	if(fontSurface)
		TTF_CloseFont(fontSurface);
	fontSurface = nullptr;

	pokemonIconSurface_cache.clear();
	pokemonTypeSurface_cache.clear();
	pokemonIDNameSurface_cache.clear();

    pokemon.clear();

	selectedIndex = 0, offset = 0;
}

void PokedexActivityList::onLoop() {
    pokemon = (*dbResults)[selectedIndex];
}

void PokedexActivityList::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivityList::onRender START \n";
	if(needRedraw){
		// Clear the display surface
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

		PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

		// Render List Items
		for (int i = 0; i < MAX_VISIBLE_ITEMS && static_cast<std::size_t>(offset + i) < dbResults->size(); i++) {
			// Render list items
			if (!renderListItems(surf_display, font, i)) {
				exit(EXIT_FAILURE);
			}
		}
		needRedraw = false;
	}
}

bool PokedexActivityList::renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i) {
    //List item background
    listBackgroundRect = renderItemBackground(surf_display, i);

    if (offset + i == selectedIndex) {
        //List item sprites/icons
        if (!renderItemSprites(surf_display, i)) {
            std::cout << "Unable to render item sprites! SDL Error: " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    //List pokemon id
    if (!renderItemEntry(surf_display, &listBackgroundRect, font, i)) {
        std::cout << "Error in renderItemEntry! SDL Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    return true;
}

SDL_Rect PokedexActivityList::renderItemBackground(SDL_Surface* surf_display, int i) {
    listBackgroundRect.h = ITEM_HEIGHT;
	double heightRatio = static_cast<double>(listBackgroundRect.h) / static_cast<double>(listBackgroundSurface_default->h);

	listBackgroundRect.x = static_cast<int>(WINDOW_WIDTH - (WINDOW_WIDTH * 0.5));
    listBackgroundRect.y = (i * ITEM_HEIGHT + 70);
    listBackgroundRect.w = static_cast<int>(heightRatio * listBackgroundSurface_default->w);

	if(offset + i == selectedIndex){
    	PokeSurface::onDrawScaled(surf_display, listBackgroundSurface_selected, &listBackgroundRect);
	} 
	else {
    	PokeSurface::onDrawScaled(surf_display, listBackgroundSurface_default, &listBackgroundRect);
	}

    return listBackgroundRect;
}

bool PokedexActivityList::renderItemSprites(SDL_Surface* surf_display, int i) {
	int index = offset + i;
    //pokemon Sprite
    pokeEntryRect.x = 50;
    pokeEntryRect.y = 100;
    pokeEntryRect.w = pokemonIconSurface_cache[index]->w * 2;
    pokeEntryRect.h = pokemonIconSurface_cache[index]->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokemonIconSurface_cache[index], &pokeEntryRect);

    //List item types_1
    pokeEntryType1Rect.x = 20;
    pokeEntryType1Rect.y = pokeEntryRect.h + 220;
    pokeEntryType1Rect.w = pokemonTypeSurface_cache[index][0]->w * 2;
    pokeEntryType1Rect.h = pokemonTypeSurface_cache[index][0]->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokemonTypeSurface_cache[index][0], &pokeEntryType1Rect);

    //List item types_2
    if (pokemon[4] != "NULL") {
        pokeEntryType2Rect.x = 20 + pokeEntryType1Rect.w;
        pokeEntryType2Rect.y = pokeEntryRect.h + 220;
        pokeEntryType2Rect.w = pokemonTypeSurface_cache[index][1]->w * 2;
        pokeEntryType2Rect.h = pokemonTypeSurface_cache[index][1]->h * 2;

        PokeSurface::onDrawScaled(surf_display, pokemonTypeSurface_cache[index][1], &pokeEntryType2Rect);
    }
    
    return true;
}

bool PokedexActivityList::renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i) {
	int index = offset + i;

    pokeIDRect.x = rect->x + 60;
    pokeIDRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokemonIDNameSurface_cache[index][0]->h / 2) - 5; // -5 to stay above white line in background
    pokeIDRect.w = pokemonIDNameSurface_cache[index][0]->w;
    pokeIDRect.h = pokemonIDNameSurface_cache[index][0]->h;

    pokeNameRect.x = pokeIDRect.x + (rect->w / 2) - (pokemonIDNameSurface_cache[index][1]->w / 2);
    pokeNameRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokemonIDNameSurface_cache[index][1]->h / 2) - 5;// -5 to stay above white line in background
    pokeNameRect.w = pokemonIDNameSurface_cache[index][1]->w;
    pokeNameRect.h = pokemonIDNameSurface_cache[index][1]->h;

	if(offset + i == selectedIndex){
    	PokeSurface::onDraw(surf_display, pokemonIDNameSurface_cache[index][2], &pokeIDRect);
    	PokeSurface::onDraw(surf_display, pokemonIDNameSurface_cache[index][3], &pokeNameRect);
	}
	else{
    	PokeSurface::onDraw(surf_display, pokemonIDNameSurface_cache[index][0], &pokeIDRect);
    	PokeSurface::onDraw(surf_display, pokemonIDNameSurface_cache[index][1], &pokeNameRect);
	}

    return true;
}

void PokedexActivityList::onFreeze() {
// do thing for now..
}

PokedexActivityList* PokedexActivityList::getInstance() {
    return &instance;
}

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonDown(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivityList::onButtonRight(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivityList::onButtonA(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB
    PokedexDB::setPokemonID(std::stoi(pokemon[0]));
    PokedexDB::setPokemonIdentifier(pokemon[1]);

    ////Call next activity
    PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivityList::onButtonB(SDL_Keycode sym, Uint16 mod) {
    // Play the sound effect
    Mix_PlayChannel(-1, se_on_exit, 0);
   // need to find a way to call Mix_CloseChunk() without stopping the audio immediately 
   
    ////Set pokemon identifier for PokedexDB
    PokedexActivityManager::back();
}

void PokedexActivityList::onButtonR(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately 
    }
    else {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityList::onButtonL(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex >= MAX_VISIBLE_ITEMS) {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset) {
            offset -= MAX_VISIBLE_ITEMS; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0) {
                offset = 0;  // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately 
    }
    else {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset = 0;  // Cap offset to zero
    }

}

void PokedexActivityList::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
