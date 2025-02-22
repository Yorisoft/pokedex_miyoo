#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Moves.h"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves() :
dbResults(nullptr),
pokemon(nullptr),
selectedIndex(0),
offset(0),
color({ 64, 64, 64}),
highlightColor({ 255, 0, 0 }),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
}

void PokedexActivity_PokemonView_Moves::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    pokemon = new Pokemon();

    dbResults = PokedexDB::executeSQL(&SQL_getPokeMovesDetail);
    for (std::vector<std::string>&moves : *dbResults) {
        for (auto& col : moves) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    move = (*dbResults)[selectedIndex];

    std::string sEffectPath = "res/assets/sound_effects/left_right.wav"; 
    sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    sEffect_UpDown = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    // why cant I delete this here ? 
    //delete dbResults; 
    //dbResults = nullptr; 

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {
}

void PokedexActivity_PokemonView_Moves::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Moves Items
    std::string backgroundImageFile = "res/assets/misc/pokemon_fr_view_3.png";
    SDL_Surface* backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
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

    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++) {
        move = (*dbResults)[offset + i];

        if (!renderListItems(surf_display, renderer, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    //Render list item background/surface
    SDL_Surface* listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
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
    SDL_Rect listEntryRect = {
        static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8),
        65 + (i * (itemHeight + spacing)),
        static_cast<int>(surf_display->w * 0.45),
        itemHeight
    };
    if (offset + i == selectedIndex) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderDrawRect(renderer, &listEntryRect);
    }
    else {
        PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    }
    SDL_FreeSurface(listEntrySurface);

    //Render item type
    std::string type = "res/assets/pokemons/types/" + move[2] + ".png";
    SDL_Surface* typeSurface = PokeSurface::onLoadImg(type);
    if (typeSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: typeSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect typeRect = {
        listEntryRect.x,
        listEntryRect.y,
        static_cast<int>(typeSurface->w * 1.5),
        static_cast<int>(typeSurface->h * 1.5)
    };
    PokeSurface::onDrawScaled(surf_display, typeSurface, &typeRect);
    SDL_FreeSurface(typeSurface);

    //Render item title/name
    std::string name = move[1];
    SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(
        font,
        name.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (nameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: nameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect nameRect = {
        typeRect.x + typeRect.w + 10,
        typeRect.y + 5,
        nameSurface->w,
        nameSurface->h
    };
    PokeSurface::onDrawScaled(surf_display, nameSurface, &nameRect);
    SDL_FreeSurface(nameSurface);

    // Render method
    std::string method = "res/assets/encounters/" + move[9] + ".png";
    SDL_Surface* methodSurface = PokeSurface::onLoadImg(method);
    if (methodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: methodSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect methodRect = {
        listEntryRect.x,
        (listEntryRect.y + listEntryRect.h ) - methodSurface->h * static_cast<int>(1.5),
        static_cast<int>(methodSurface->w * 1.5),
        static_cast<int>(methodSurface->h * 1.5)
    };
    PokeSurface::onDrawScaled(surf_display, methodSurface, &methodRect);
    SDL_FreeSurface(methodSurface);

    // Render level
    if (move[9] == "level-up") {
        std::string level =  move[8];
        SDL_Surface* levelSurface = TTF_RenderUTF8_Blended(
            font,
            level.c_str(),
            offset + i == selectedIndex ? highlightColor : color
        );
        if (levelSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: levelSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect levelRect = {
            methodRect.x + methodRect.w,
            (listEntryRect.y + listEntryRect.h ) - levelSurface->h,
            static_cast<int>(levelSurface->w),
            static_cast<int>(levelSurface->h)
        };
        PokeSurface::onDrawScaled(surf_display, levelSurface, &levelRect);
        SDL_FreeSurface(levelSurface);
    }

    // Render PP
    std::string pp = move[3] + '/' + move[3];
    SDL_Surface* ppSurface = TTF_RenderUTF8_Blended(
        font,
        pp.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (ppSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: ppSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect ppRect = {
        (listEntryRect.x + listEntryRect.w ) - ppSurface->w,
        (listEntryRect.y + listEntryRect.h ) - ppSurface->h,
        ppSurface->w,
        ppSurface->h
    };
    PokeSurface::onDraw(surf_display, ppSurface, &ppRect);
    SDL_FreeSurface(ppSurface);

    if (offset + i == selectedIndex) {
        if (!renderItemDetails(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // render poke icon
    std::string pokemonIcon = PokedexDB::getPokemonIdentifier();
    pokemonIcon = "res/icons/" + pokemonIcon + ".png";

    SDL_Surface* pokeSurface = PokeSurface::onLoadImg(pokemonIcon);
    if (pokeSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect = {
        0,
        55,
        pokeSurface->w * 2,
        pokeSurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, pokeSurface, &pokeIconRect);
    SDL_FreeSurface(pokeSurface);
    
    // Render Name
    std::string pokeName = pokemon->getName();
    SDL_Surface* pokeNameSurface = TTF_RenderUTF8_Blended(
        font,
        pokeName.c_str(),
        color
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect = {
        (pokeIconRect.x + pokeIconRect.w),
        pokeIconRect.y + 10,
        pokeNameSurface->w,
        pokeNameSurface->h
    };
    PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    // render poke types
    std::vector<std::string> types = pokemon->getTypes();
    std::string typeA = "res/assets/pokemons/types/" + types[0] + ".png";
    SDL_Surface* typeASurface = PokeSurface::onLoadImg(typeA);

    if (typeASurface == NULL) {
        std::cout << "Unable to load surface: typeASurface Type 1" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect typesARect = {
        pokeIconRect.x + pokeIconRect.w,
        (pokeNameRect.y + pokeNameRect.h ) + 5,
        typeASurface->w * 2,
        typeASurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, typeASurface, &typesARect);
    SDL_FreeSurface(typeASurface);

    //List item types_2
    if (types[1] != "NULL") { 
        std::string typeB = "res/assets/pokemons/types/" + types[1] + ".png";
        SDL_Surface* typeBSurface = PokeSurface::onLoadImg(typeB);
        if (typeBSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: typeBSurface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect typeBRect = {
            typesARect.x + typesARect.w + 5,
            typesARect.y,
            typeBSurface->w * 2,
            typeBSurface->h * 2
        };
        PokeSurface::onDrawScaled(surf_display, typeBSurface, &typeBRect);
        SDL_FreeSurface(typeBSurface);
    }

    // Render Power
    SDL_Surface* pwrSurface = TTF_RenderUTF8_Blended(
        font,
        (move[5] == "NULL" ? "--" : move[5]).c_str(),
        color
    );
    if (pwrSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pwrSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pwrRect = {
        typesARect.x + 40,
        typesARect.y + 70,
        pwrSurface->w,
        pwrSurface->h
    };
    PokeSurface::onDraw(surf_display, pwrSurface, &pwrRect);
    SDL_FreeSurface(pwrSurface);

    //List item category/class
    std::string moveClass = "res/assets/pokemons/types/" + move[4] + ".png";
    SDL_Surface* classSurface = PokeSurface::onLoadImg(moveClass);
    if (classSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: classSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect classRect = {
        classRect.x = pwrRect.x + 80,
        classRect.y = pwrRect.y,
        classRect.w = classSurface->w * 2,
        classRect.h = classSurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, classSurface, &classRect);
    SDL_FreeSurface(classSurface);

    // Render Accuracy
    SDL_Surface* accrySurface = TTF_RenderUTF8_Blended(
        font,
        (move[6] == "NULL" ? "--" : move[6]).c_str(),
        color
    );
    if (accrySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: accrySurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect accryRect = {
        accryRect.x = pwrRect.x,
        accryRect.y = (pwrRect.y + pwrRect.h) + 10,
        accryRect.w = accrySurface->w,
        accryRect.h = accrySurface->h
    };
    PokeSurface::onDraw(surf_display, accrySurface, &accryRect);
    SDL_FreeSurface(accrySurface);

    // Render Effect
    std::string summary = cleanString(move[7]);
    SDL_Surface* summarySurface = TTF_RenderUTF8_Blended_Wrapped(
        font,
        summary.c_str(),
        color,
        295
    );
    if (summarySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: summarySurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect summaryRect = {
        summaryRect.x = 15,
        summaryRect.y = (WINDOW_HEIGHT / 2 ) + 50,
        summaryRect.w = summarySurface->w,
        summaryRect.h = summarySurface->h
    };
    PokeSurface::onDraw(surf_display, summarySurface, &summaryRect);
    SDL_FreeSurface(summarySurface);

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
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonR(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS;
            }
        }
    }
    else {
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; 
    }
}

void PokedexActivity_PokemonView_Moves::onButtonL(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex >= MAX_VISIBLE_ITEMS) {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset) {
            offset -= MAX_VISIBLE_ITEMS; 
            if (offset < 0) {
                offset = 0; 
            }
        }
    }
    else {
        selectedIndex = 0;
        offset = 0;
    }
}

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}