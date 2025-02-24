#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() : 
    dbResults(nullptr),
    backgroundSurface(nullptr),
    listBackgroundSurface_default(nullptr),
    listBackgroundSurface_selected(nullptr),
    sEffect(nullptr),
    sEffect_OnStart(nullptr),
    sEffect_OnExit(nullptr),
    needRedraw(true),
    selectedIndex(0),
    offset(0),
    itemHeight(0)
{
}

PokedexActivityList::~PokedexActivityList() {
    //Cant delete in onDeactivate or auido will be cut short
    // deleting here is not best practice, this will only ever be called once, 
    // onActivate/Deactivate will be called many times.
    if(sEffect_OnExit)
        Mix_FreeChunk(sEffect_OnExit);
    sEffect_OnExit = nullptr;

}

void PokedexActivityList::initSDL(){
	// Sound Effects
    sEffect_OnStart = Mix_LoadWAV("res/assets/sound_effects/list_start.wav");
    if (!sEffect_OnStart) {
        std::cerr << "Failed to load sound sEffect_OnStart: " << Mix_GetError() << std::endl;
    }

    sEffect_OnExit = Mix_LoadWAV("res/assets/sound_effects/list_back.wav");
    if (!sEffect_OnExit) {
        std::cerr << "Failed to load sound sEffect_OnExit: " << Mix_GetError() << std::endl;
    }

    sEffect = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

	//Background Surface
	backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
	if (backgroundSurface == NULL) {
		std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	};
	backgroundRect.x = 0;
	backgroundRect.y = 0;
	backgroundRect.w = WINDOW_WIDTH;
	backgroundRect.h = WINDOW_HEIGHT;

	// List Item Background Surface
    listBackgroundSurface_default = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_DEFAULT);
    if (listBackgroundSurface_default == NULL) {
        std::cout << "Unable to load surface: listBackgroundSurface_default " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    listBackgroundSurface_selected = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_SELECTED);
    if (listBackgroundSurface_selected == NULL) {
        std::cout << "Unable to load surface: listBackgroundSurface_selected " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    listBackgroundRect.x = static_cast<int>(WINDOW_WIDTH - (WINDOW_WIDTH * 0.5));
    listBackgroundRect.w = static_cast<int>(WINDOW_WIDTH * 0.5);

	// Pokemon Icon && Types Surface
	pokemonIconSurface_cache.clear();
	pokemonTypeSurface_cache.clear();
	pokemonIDNameSurface_cache.clear();

	TTF_Font* temp_font = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
	if (temp_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	for(auto& poke : *dbResults){
    	std::string iconFile = "res/assets/pokemons/sprites/" + poke[1] + ".png";
		SDL_Surface* pokeIconSurface = PokeSurface::onLoadImg(iconFile);
		if (pokeIconSurface == NULL) {
			std::cout << "Unable to load surface: pokeIconSurface " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		pokemonIconSurface_cache.push_back(pokeIconSurface);

		std::vector<SDL_Surface*> temp;
    	iconFile = "res/assets/pokemons/types/" + poke[3] + ".png";
		SDL_Surface* pokeType1Surface = PokeSurface::onLoadImg(iconFile);
		if (pokeType1Surface == NULL) {
			std::cout << "Unable to load surface: pokeType1Surface " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		temp.push_back(pokeType1Surface);

		if (poke[4] != "NULL") {
			iconFile = "res/assets/pokemons/types/" + poke[4] + ".png";
			SDL_Surface* pokeType2Surface = PokeSurface::onLoadImg(iconFile);
			if (pokeType2Surface == NULL) {
				std::cout << "Unable to load surface: pokeType2Surface " << SDL_GetError() << std::endl;
				exit(EXIT_FAILURE);
			};
			temp.push_back(pokeType2Surface);
		}
		pokemonTypeSurface_cache.emplace_back(temp);

		// ID && NAME
		temp.clear();
		SDL_Surface* pokeIDSurface = TTF_RenderUTF8_Blended(
			temp_font,
			poke[0].c_str(),
			color
		);
		if (pokeIDSurface == NULL) {
			std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		temp.push_back(pokeIDSurface);

		std::string name = poke[2];
		/*for (int i = 0; i < name.size(); i++) {
			name[i] = std::toupper(name[i]);
		}*/
		SDL_Surface* pokeNameSurface = TTF_RenderUTF8_Blended(
			temp_font,
			name.c_str(),
			color
		);
		if (pokeNameSurface == NULL) {
			std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		temp.push_back(pokeNameSurface);

		// ID && NAME HIGHLIGHTED
		SDL_Surface* pokeIDSurface_selected = TTF_RenderUTF8_Blended(
			temp_font,
			poke[0].c_str(),
			highlightColor
		);
		if (pokeIDSurface_selected == NULL) {
			std::cout << "Unable to render text! SDL Error: pokeIDSurface_selected " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		temp.push_back(pokeIDSurface_selected);

		SDL_Surface* pokeNameSurface_selected = TTF_RenderUTF8_Blended(
			temp_font,
			name.c_str(),
			highlightColor
		);
		if (pokeNameSurface_selected == NULL) {
			std::cout << "Unable to render text! SDL Error: pokeNameSurface_selected " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		temp.push_back(pokeNameSurface_selected);

		pokemonIDNameSurface_cache.emplace_back(temp);
	}

}

void PokedexActivityList::onActivate() {
    std::cout << "PokedexActivityList::onActivate START \n";

    color = { 248, 248, 248 }, highlightColor = { 255, 0, 0 };

    itemHeight = static_cast<int>(WINDOW_HEIGHT * 0.6 / 5);

    needRedraw = true;

	// Pokemon List DB Results
    dbResults = PokedexDB::executeSQL(&SQL_getNameAndID);
    for (auto& pokemon : *dbResults) {
        for (auto& col : pokemon) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    pokemon = (*dbResults)[selectedIndex];

	initSDL();

    // Play the sound effect
    Mix_PlayChannel(-1, sEffect_OnStart, 0);

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate() {
	if(backgroundSurface)
    	SDL_FreeSurface(backgroundSurface);

	if(listBackgroundSurface_default)
		SDL_FreeSurface(listBackgroundSurface_default);

	if(listBackgroundSurface_selected)
		SDL_FreeSurface(listBackgroundSurface_selected);

	for(SDL_Surface* icon : pokemonIconSurface_cache)
		if(icon)
    		SDL_FreeSurface(icon);

	for(std::vector<SDL_Surface*> types : pokemonTypeSurface_cache)
		for(SDL_Surface* type : types)
			if(type)
				SDL_FreeSurface(type);

	for(std::vector<SDL_Surface*> ids_names : pokemonIDNameSurface_cache)
		for(SDL_Surface* id_name : ids_names)
			if(id_name)
				SDL_FreeSurface(id_name);

    if(sEffect_OnStart)
        Mix_FreeChunk(sEffect_OnStart);
    sEffect_OnStart = nullptr;

    if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;

	pokemonIconSurface_cache.clear();
	pokemonTypeSurface_cache.clear();
	pokemonIDNameSurface_cache.clear();

    color = {  }, highlightColor = {  };

    pokemon.clear();

    selectedIndex = 0, offset = 0, itemHeight = 0;
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

void PokedexActivityList::onFreeze() {
// do thing for now..
}

PokedexActivityList* PokedexActivityList::getInstance() {
    return &instance;
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
    listBackgroundRect.y = (i * itemHeight + 70);
    listBackgroundRect.h = itemHeight;

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

    pokeIDRect.x = rect->x + 50;
    pokeIDRect.y = (i * itemHeight + 70) + (rect->h / 2) - (pokemonIDNameSurface_cache[index][0]->h / 2) - 5; // -5 to stay above white line in background
    pokeIDRect.w = pokemonIDNameSurface_cache[index][0]->w;
    pokeIDRect.h = pokemonIDNameSurface_cache[index][0]->h;

    pokeNameRect.x = pokeIDRect.x + (rect->w / 2) - (pokemonIDNameSurface_cache[index][1]->w / 2);
    pokeNameRect.y = (i * itemHeight + 70) + (rect->h / 2) - (pokemonIDNameSurface_cache[index][1]->h / 2) - 5;// -5 to stay above white line in background
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

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, sEffect, 0);
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
        Mix_PlayChannel(-1, sEffect, 0);
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
    Mix_PlayChannel(-1, sEffect_OnExit, 0);
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
        Mix_PlayChannel(-1, sEffect, 0);
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
        Mix_PlayChannel(-1, sEffect, 0);
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
