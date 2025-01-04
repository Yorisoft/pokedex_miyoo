#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Moves.h"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves() :
backgroundSurface(nullptr),
fontSurface(nullptr),
listEntrySurface(nullptr),
moveNameSurface(nullptr),
moveTypeSurface(nullptr),
movePPSurface(nullptr),
moveMethodSurface(nullptr),
pokeIconSurface(nullptr),
pokeType1Surface(nullptr),
pokeType2Surface(nullptr),
pokeNameSurface(nullptr),
moveClassSurface(nullptr),
movePwrSurface(nullptr),
moveAcrySurface(nullptr),
moveEffectSurface(nullptr),
dbResults(nullptr),
pokemon(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
    fontPath = "res/font/pokemon-advanced-battle/pokemon-advanced-battle.ttf";
    // color maps
    color = { 64, 64, 64};
    highlightColor = { 255, 0, 0 };

}

void PokedexActivity_PokemonView_Moves::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";
    // For some reason.. pokemon needs to be created before executeSQL command...
    pokemon = new Pokemon();

    std::cout << "Create new pokemon. Making getPokeMoveDetails call to db" << std::endl;
    dbResults = PokedexDB::executeSQL(&SQL_getPokeMovesDetail);
    for (std::vector<std::string>&moves : *dbResults) {
        for (auto& col : moves) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    move = (*dbResults)[selectedIndex];

    std::string sEffectPath = "res/audio/sound_effects/left_right.wav"; // <- empty char is standin for form variant
    sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {
}

void PokedexActivity_PokemonView_Moves::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivity_PokemonView_Moves::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Moves Items
    std::string backgroundImageFile = "res/icons/icon/pokemon_fr_view_3.png";
    backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    // Render _PokemonView_Moves Items
    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++) {
        move = (*dbResults)[offset + i];

        // Render list items
        if (!renderListItems(surf_display, renderer, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    // Fill the surface with transparent color
    //SDL_FillRect(listEntrySurface, NULL, SDL_MapRGBA(listEntrySurface->format, 150, 150, 150, 255));

    //Render list item background/surface
    listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        static_cast<int>(surf_display->w * 0.9),
        itemHeight,
        DEPTH,
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int spacing = 18; 
    SDL_Rect listEntryRect;
    listEntryRect.x = static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8);
    listEntryRect.y = 65 + (i * (itemHeight + spacing));
    listEntryRect.w = static_cast<int>(surf_display->w * 0.45);
    listEntryRect.h = itemHeight;
    if (offset + i == selectedIndex) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderDrawRect(renderer, &listEntryRect);
    }
    else {
        PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    }
    SDL_FreeSurface(listEntrySurface);

    /////////////////////////////////////////////////////////////////////////////
    //Render item type
    std::string moveTypePath = "res/types/" + move[2] + ".png";
    moveTypeSurface = PokeSurface::onLoadImg(moveTypePath);
    if (moveTypeSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveTypeSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect moveTypeRect;
    moveTypeRect.x = listEntryRect.x;
    moveTypeRect.y = listEntryRect.y;
    moveTypeRect.w = static_cast<int>(moveTypeSurface->w * 1.5);
    moveTypeRect.h = static_cast<int>(moveTypeSurface->h * 1.5);
    PokeSurface::onDrawScaled(surf_display, moveTypeSurface, &moveTypeRect);
    SDL_FreeSurface(moveTypeSurface);


    /////////////////////////////////////////////////////////////////////////////
    //Render item title/name
    
    std::string moveName = move[1];
    // make all upper case
    //for (int i = 0; i < moveName.size(); i++) {
    //    moveName[i] = std::toupper(moveName[i]);
    //}
    moveNameSurface = TTF_RenderUTF8_Blended(
        font,
        moveName.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (moveNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveNameRect;
    moveNameRect.x = moveTypeRect.x + moveTypeRect.w + 10;
    moveNameRect.y = moveTypeRect.y + 5;
    moveNameRect.w = moveNameSurface->w;
    moveNameRect.h = moveNameSurface->h;
    PokeSurface::onDrawScaled(surf_display, moveNameSurface, &moveNameRect);
    SDL_FreeSurface(moveNameSurface);

    /////////////////////////////////////////////////////////////////////////////
    // Render method
    std::string methodString = move[9];
    if (methodString == "level-up") {
        methodString = "LV." + move[8];
        moveMethodSurface = TTF_RenderUTF8_Blended(
            font,
            methodString.c_str(),
            offset + i == selectedIndex ? highlightColor : color
        );
    }
    else {
        moveMethodSurface = TTF_RenderUTF8_Blended(
            font,
            methodString.c_str(),
            offset + i == selectedIndex ? highlightColor : color
        );
    }
    if (moveMethodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveMethodSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveMethodRect;
    moveMethodRect.x = listEntryRect.x;
    moveMethodRect.y = (listEntryRect.y + listEntryRect.h ) - moveMethodSurface->h;
    moveMethodRect.w = moveMethodSurface->w;
    moveMethodRect.h = moveMethodSurface->h;
    PokeSurface::onDraw(surf_display, moveMethodSurface, &moveMethodRect);
    SDL_FreeSurface(moveMethodSurface);

    /////////////////////////////////////////////////////////////////////////////
    // Render PP
    // load font
    std::string ppString = move[3] + '/' + move[3];
    movePPSurface = TTF_RenderUTF8_Blended(
        font,
        ppString.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (movePPSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: movePPSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect movePPRect;
    movePPRect.x = (listEntryRect.x + listEntryRect.w ) - movePPSurface->w;
    movePPRect.y = (listEntryRect.y + listEntryRect.h ) - movePPSurface->h;
    movePPRect.w = movePPSurface->w;
    movePPRect.h = movePPSurface->h;
    PokeSurface::onDraw(surf_display, movePPSurface, &movePPRect);
    SDL_FreeSurface(movePPSurface);

    /////////////////////////////////////////////////////////////////////////////
    // Render selected Item details
    if (offset + i == selectedIndex) {
        //redern item details
        if (!renderItemDetails(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // render poke icon
    std::string iconPath = PokedexDB::getPokemonIdentifier();
    iconPath = "res/icons/" + iconPath + ".png";

    pokeIconSurface = PokeSurface::onLoadImg(iconPath);
    if (pokeIconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIconSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect;
    pokeIconRect.x = 0;
    pokeIconRect.y = 55;
    pokeIconRect.w = pokeIconSurface->w * 2;
    pokeIconRect.h = pokeIconSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);
    SDL_FreeSurface(pokeIconSurface);
    
    /////////////////////////////////////////////////////////////////////////////

    // Render Name
    std::string pokeName = pokemon->getName();
    pokeNameSurface = TTF_RenderUTF8_Blended(
        font,
        pokeName.c_str(),
        color
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect;
    pokeNameRect.x = (pokeIconRect.x + pokeIconRect.w);
    pokeNameRect.y = pokeIconRect.y + 10;
    pokeNameRect.w = pokeNameSurface->w;
    pokeNameRect.h = pokeNameSurface->h;
    PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    ////////////////////////////////////////////////////////////////////////////


    // render poke types
    std::vector<std::string> pokemonTypes = pokemon->getTypes();
    iconPath = "res/types/" + pokemonTypes[0] + ".png";
    pokeType1Surface = PokeSurface::onLoadImg(iconPath);

    if (pokeType1Surface == NULL) {
        std::cout << "Unable to load surface: pokeType1Surface Type 1" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect pokeType1Rect;
    pokeType1Rect.x = pokeIconRect.x + pokeIconRect.w;
    pokeType1Rect.y = (pokeNameRect.y + pokeNameRect.h ) + 5;
    pokeType1Rect.w = pokeType1Surface->w * 2;
    pokeType1Rect.h = pokeType1Surface->h * 2;
    PokeSurface::onDrawScaled(surf_display, pokeType1Surface, &pokeType1Rect);
    SDL_FreeSurface(pokeType1Surface);

    //List item types_2
    if (pokemonTypes[1] != "NULL") { 
        iconPath = "res/types/" + pokemonTypes[1] + ".png";
        pokeType2Surface = PokeSurface::onLoadImg(iconPath);
        if (pokeType2Surface == NULL) {
            std::cout << "Unable to render text! SDL Error: pokeType2Surface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect pokeType2Rect;
        pokeType2Rect.x = pokeType1Rect.x + pokeType1Rect.w + 5;
        pokeType2Rect.y = pokeType1Rect.y;
        pokeType2Rect.w = pokeType2Surface->w * 2;
        pokeType2Rect.h = pokeType2Surface->h * 2;

        PokeSurface::onDrawScaled(surf_display, pokeType2Surface, &pokeType2Rect);
        SDL_FreeSurface(pokeType2Surface);
    }

    ///////////////////////////////////////////////////////////////////////

    // Render Power
    
    movePwrSurface = TTF_RenderUTF8_Blended(
        font,
        (move[5] == "NULL" ? "--" : move[5]).c_str(),
        color
    );
    if (movePwrSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: movePwrSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect movePwrRect;
    movePwrRect.x = pokeType1Rect.x + 40;
    movePwrRect.y = pokeType1Rect.y + 70;
    movePwrRect.w = movePwrSurface->w;
    movePwrRect.h = movePwrSurface->h;
    PokeSurface::onDraw(surf_display, movePwrSurface, &movePwrRect);
    SDL_FreeSurface(movePwrSurface);

    ////////////////////////////////////////////////////////////////////////////
    //List item category/class
    iconPath = "res/types/" + move[4] + ".png";
    moveClassSurface = PokeSurface::onLoadImg(iconPath);
    if (moveClassSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveClassSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveClassRect;
    moveClassRect.x = movePwrRect.x + 80;
    moveClassRect.y = movePwrRect.y;
    moveClassRect.w = moveClassSurface->w * 2;
    moveClassRect.h = moveClassSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, moveClassSurface, &moveClassRect);
    SDL_FreeSurface(moveClassSurface);

    //////////////////////////////////////////////////////////////////////////////////
    // Render Accuracy
    moveAcrySurface = TTF_RenderUTF8_Blended(
        font,
        (move[6] == "NULL" ? "--" : move[6]).c_str(),
        color
    );
    if (moveAcrySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveAcrySurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveAcryRect;
    moveAcryRect.x = movePwrRect.x;
    moveAcryRect.y = (movePwrRect.y + movePwrRect.h) + 10;
    moveAcryRect.w = moveAcrySurface->w;
    moveAcryRect.h = moveAcrySurface->h;
    PokeSurface::onDraw(surf_display, moveAcrySurface, &moveAcryRect);
    SDL_FreeSurface(moveAcrySurface);

    ////////////////////////////////////////////////////////////////////////////
    // Render Effect
    std::string effect = cleanString(move[7]);
    moveEffectSurface = TTF_RenderUTF8_Blended_Wrapped(
        font,
        effect.c_str(),
        color,
        320
    );
    if (moveEffectSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveEffectSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveEffectRect;
    moveEffectRect.x = 15;
    moveEffectRect.y = (WINDOW_HEIGHT / 2 ) + 50;
    moveEffectRect.w = moveEffectSurface->w;
    moveEffectRect.h = moveEffectSurface->h;
    PokeSurface::onDraw(surf_display, moveEffectSurface, &moveEffectRect);
    SDL_FreeSurface(moveEffectSurface);

    ////////////////////////////////////////////////////////////////////////////

    return true;
}

std::string PokedexActivity_PokemonView_Moves::cleanString(std::string str) {
	std::string replacement = " "; 
	std::string cleanStr = str;
	size_t pos = 0;
	while ((pos = cleanStr.find("\n", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = cleanStr.find("\r", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = cleanStr.find("\f", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	return cleanStr;

}

void PokedexActivity_PokemonView_Moves::onFreeze() {
    // do thing for now..
}

PokedexActivity_PokemonView_Moves* PokedexActivity_PokemonView_Moves::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Moves::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
    }
}

void PokedexActivity_PokemonView_Moves::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}

void PokedexActivity_PokemonView_Moves::onButtonR(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
    }
    else {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivity_PokemonView_Moves::onButtonL(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex >= MAX_VISIBLE_ITEMS) {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset) {
            offset -= MAX_VISIBLE_ITEMS; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0) {
                offset = 0;  // Cap offset to zero
            }
        }
    }
    else {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset = 0;  // Cap offset to zero
    }
}

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);
    ////Set pokemon identifier for PokedexDB
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);

}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB
    //move = (*dbResults)[selectedIndex];
    //PokedexDB::setPokemonIdentifier(move[1]);

    ////Call next activity
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);

}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
