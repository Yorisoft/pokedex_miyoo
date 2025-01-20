#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() : 
    dbResults(nullptr),
    sEffect(nullptr),
    sEffect_OnStart(nullptr),
    sEffect_OnExit(nullptr),
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

void PokedexActivityList::onActivate() {
    std::cout << "PokedexActivityList::onActivate START \n";

    color = { 248, 248, 248 }, highlightColor = { 255, 0, 0 };

    itemHeight = static_cast<int>(WINDOW_HEIGHT * 0.6 / 5);

    dbResults = PokedexDB::executeSQL(&SQL_getNameAndID);
    for (auto& pokemon : *dbResults) {
        for (auto& col : pokemon) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    pokemon = (*dbResults)[selectedIndex];

    sEffect_OnStart = Mix_LoadWAV("res/audio/sound_effects/list_start.wav");
    if (!sEffect_OnStart) {
        std::cerr << "Failed to load sound sEffect_OnStart: " << Mix_GetError() << std::endl;
    }
    // Play the sound effect
    Mix_PlayChannel(-1, sEffect_OnStart, 0);

    sEffect_OnExit = Mix_LoadWAV("res/audio/sound_effects/list_back.wav");
    if (!sEffect_OnExit) {
        std::cerr << "Failed to load sound sEffect_OnExit: " << Mix_GetError() << std::endl;
    }

    sEffect = Mix_LoadWAV("res/audio/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate() {
    if(sEffect_OnStart)
        Mix_FreeChunk(sEffect_OnStart);
    sEffect_OnStart = nullptr;

    if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;

    color = {  }, highlightColor = {  };

    // why cant I delete this here ? 
    //delete dbResults; 
    //dbResults = nullptr; 

    pokemon.clear();

    selectedIndex = 0, offset = 0, itemHeight = 0;
}

void PokedexActivityList::onLoop() {
}

void PokedexActivityList::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivityList::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render List Items
    std::string backgroundImageFile = "res/icons/icon/pokedexList_background.png";
    SDL_Surface* listBackgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (listBackgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: listBackgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, listBackgroundSurface, &backgroundRect);
    SDL_FreeSurface(listBackgroundSurface);

    // Render List Items
    for (int i = 0; i < MAX_VISIBLE_ITEMS && static_cast<std::size_t>(offset + i) < dbResults->size(); i++) {
        // Render list items
        if (!renderListItems(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
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
    SDL_Rect listEntryRect = renderItemBackground(surf_display, i);

    pokemon = (*dbResults)[offset + i];
    if (offset + i == selectedIndex) {
        //List item sprites/icons
        if (!renderItemSprites(surf_display, i)) {
            std::cout << "Unable to render item sprites! SDL Error: " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);

        }
    }
    //List pokemon id
    if (!renderItemEntry(surf_display, &listEntryRect, font, i)) {
        std::cout << "Error in renderItemEntry! SDL Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);

    }
    return true;
}

SDL_Rect PokedexActivityList::renderItemBackground(SDL_Surface* surf_display, int i) {
    std::string backgroundImageFile = "res/icons/icon/menu_item_background_";
    offset + i == selectedIndex ? backgroundImageFile.append("selected.png") : backgroundImageFile.append("default.png");
    SDL_Surface* listEntrySurface = PokeSurface::onLoadImg(backgroundImageFile);

    if (listEntrySurface == NULL) {
        std::cout << "Unable to load surface: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect listEntryRect;
    listEntryRect.x = static_cast<int>(surf_display->w - (surf_display->w * 0.5));
    listEntryRect.y = (i * itemHeight + 70);
    listEntryRect.w = static_cast<int>(surf_display->w * 0.5);
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);
    
    return listEntryRect;
}

bool PokedexActivityList::renderItemSprites(SDL_Surface* surf_display, int i) {
    std::string pokemonName = pokemon[1];
    std::string iconFile = "res/sprites/" + pokemonName + ".png";
    SDL_Surface*pokeIconSurface = PokeSurface::onLoadImg(iconFile);

    if (pokeIconSurface == NULL) {
        std::cout << "Unable to load surface: pokeIconSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect pokeEntryRect;
    pokeEntryRect.x = 50;
    pokeEntryRect.y = 100;
    pokeEntryRect.w = pokeIconSurface->w * 2;
    pokeEntryRect.h = pokeIconSurface->h * 2;
    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeEntryRect);
    SDL_FreeSurface(pokeIconSurface);

    //List item types_1
    std::string pokemonType1 = pokemon[3];
    iconFile = "res/types/" + pokemonType1 + ".png";
    SDL_Surface* pokeType1Surface = PokeSurface::onLoadImg(iconFile);

    if (pokeType1Surface == NULL) {
        std::cout << "Unable to load surface: pokeIconSurface Type 1" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect pokeEntryType1Rect;
    pokeEntryType1Rect.x = 20;
    pokeEntryType1Rect.y = pokeEntryRect.h + 220;
    pokeEntryType1Rect.w = pokeType1Surface->w * 2;
    pokeEntryType1Rect.h = pokeType1Surface->h * 2;
    PokeSurface::onDrawScaled(surf_display, pokeType1Surface, &pokeEntryType1Rect);
    SDL_FreeSurface(pokeType1Surface);

    //List item types_2
    if (pokemon[4] != "NULL") {
        std::string pokemonType2 = pokemon[4];
        iconFile = "res/types/" + pokemonType2 + ".png";
        SDL_Surface* pokeType2Surface = PokeSurface::onLoadImg(iconFile);

        if (pokeType2Surface == NULL) {
            std::cout << "Unable to load surface: pokeIconSurface Type 1" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };
        SDL_Rect pokeEntryType2Rect;
        pokeEntryType2Rect.x = 20 + pokeEntryType1Rect.w;
        pokeEntryType2Rect.y = pokeEntryRect.h + 220;
        pokeEntryType2Rect.w = pokeType2Surface->w * 2;
        pokeEntryType2Rect.h = pokeType2Surface->h * 2;
        PokeSurface::onDrawScaled(surf_display, pokeType2Surface, &pokeEntryType2Rect);
        SDL_FreeSurface(pokeType2Surface);
    }
    
    return true;
}

bool PokedexActivityList::renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i) {
    SDL_Surface* pokeIDSurface = TTF_RenderUTF8_Blended(
        font,
        pokemon[0].c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (pokeIDSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIDRect;
    pokeIDRect.x = rect->x + 50;
    pokeIDRect.y = (i * itemHeight + 70) + (rect->h / 2) - (pokeIDSurface->h / 2) - 5; // -5 to stay above white line in background
    pokeIDRect.w = pokeIDSurface->w;
    pokeIDRect.h = pokeIDSurface->h;
    PokeSurface::onDraw(surf_display, pokeIDSurface, &pokeIDRect);
    SDL_FreeSurface(pokeIDSurface);

    
    //List pokemon name
    std::string name = pokemon[2];
    /*for (int i = 0; i < name.size(); i++) {
        name[i] = std::toupper(name[i]);
    }*/

    SDL_Surface* pokeNameSurface = TTF_RenderUTF8_Blended(
        font,
        name.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeVersionRect;
    pokeVersionRect.x = pokeIDRect.x + (rect->w / 2) - (pokeNameSurface->w / 2);
    pokeVersionRect.y = (i * itemHeight + 70) + (rect->h / 2) - (pokeNameSurface->h / 2) - 5;// -5 to stay above white line in background
    pokeVersionRect.w = pokeNameSurface->w;
    pokeVersionRect.h = pokeNameSurface->h;
    PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeVersionRect);
    SDL_FreeSurface(pokeNameSurface);

    return true;
}

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod) {
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
    pokemon = (*dbResults)[selectedIndex];
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
