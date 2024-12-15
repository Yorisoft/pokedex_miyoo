#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() {
    surf_logo = NULL;
    pokeIconSurface = NULL;
    fontSurface = NULL;
    IDSurface = NULL;
    listEntrySurface = NULL;
    listBackgroundSurface = NULL;
    selectedIndex = 0, offset = 0, itemHeight = static_cast<int>(WINDOW_HEIGHT / 5);

    //listFont = nullptr;
    pokeListLabelSurface = nullptr, pokemonListEntrySurface = nullptr;
    textStream;
    color = { 255, 255, 255 };
    highlightColor = { 255, 0, 0 };
    fontPath = "res/font/Pokemon_GB.ttf";
    pokemonListSurfaceHeight = static_cast<int>(WINDOW_HEIGHT / 5);

    dbResults = NULL;
}

void PokedexActivityList::onActivate() {
    std::cout << "PokedexActivityList::onActivate START \n";

    StartTime = SDL_GetTicks();
    dbResults = PokedexDB::executeSQL(&SQL_getNameAndID);
    for (auto& pokemon : *dbResults) {
        for (auto& col : pokemon) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 28);
    if (!fontSurface) {
        std::cerr << "PokedexActivityList::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }
    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate() {
    selectedIndex = 0, offset = 0;
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }      
    TTF_CloseFont(fontSurface);
    //TTF_Quit();
}

void PokedexActivityList::onLoop() {
}

void PokedexActivityList::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    //std::cout << "PokedexActivityList::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render List Items
    std::string backgroundImageFile = "res/icons/icon/pokedexList_background.png";
    listBackgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
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
    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++) {
        // Render list items
        if (!renderListItems(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }
    // At the end, free listEntrySurface if no longer needed
    if (listEntrySurface) {
        SDL_FreeSurface(listEntrySurface);
    }
}

void PokedexActivityList::onFreeze() {
// do thing for now..
}
PokedexActivityList* PokedexActivityList::getInstance() {
    return &instance;
}

bool PokedexActivityList::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    std::string backgroundImageFile = "res/icons/icon/menu_item_background_";
    offset + i == selectedIndex ? backgroundImageFile.append("selected.png") : backgroundImageFile.append("default.png");
    listEntrySurface = PokeSurface::onLoadImg(backgroundImageFile);

    if (listEntrySurface == NULL) {
        std::cout << "Unable to load surface: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect listEntryRect;
    listEntryRect.x = surf_display->w - (surf_display->w * 0.5);
    listEntryRect.y = (i * itemHeight);
    listEntryRect.w = surf_display->w * 0.5;
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);

    if (offset + i == selectedIndex) {
        //List item ico n   
        pokemon = (*dbResults)[offset + i];
        std::string pokemonName = pokemon[2];
        std::string iconFile = "res/icons/" + pokemonName + ".png";
        pokeIconSurface = PokeSurface::onLoadImg(iconFile);

        if (pokeIconSurface == NULL) {
            std::cout << "Unable to load surface: pokeIconSurface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };
        SDL_Rect pokeEntryRect;
        pokeEntryRect.x = 30;
        pokeEntryRect.y = 120;
        pokeEntryRect.w = pokeIconSurface->w * 4;
        pokeEntryRect.h = pokeIconSurface->h * 4;
        PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeEntryRect);


    }
        //List pokemon name
    surf_logo = TTF_RenderText_Blended(
        fontSurface,
        pokemon[3].c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (surf_logo == NULL) {
        std::cout << "Unable to render text! SDL Error: surf_logo " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeVersionRect;
    pokeVersionRect.x = listEntryRect.x + (listEntryRect.w / 2) - (surf_logo->w / 2);
    pokeVersionRect.y = (i * itemHeight) + (listEntryRect.h / 2) - (surf_logo->h / 2);
    pokeVersionRect.w = surf_logo->w;
    pokeVersionRect.h = surf_logo->h;
    PokeSurface::onDraw(surf_display, surf_logo, &pokeVersionRect);
    SDL_FreeSurface(surf_logo);

    return true;
}

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
    }
}

void PokedexActivityList::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}

void PokedexActivityList::onButtonA(SDL_Keycode sym, Uint16 mod) {
    ////Set Game version and regional pokedex ID for PokedexDB
    //PokedexDB::setGameVersion(game[1]);
    //PokedexDB::setRegionVersion(std::stoi(game[3]));

    //PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityList::onButtonB(SDL_Keycode sym, Uint16 mod) {
    //Set Game version and regional pokedex ID for PokedexDB

    PokedexActivityManager::back();
    //PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
}
