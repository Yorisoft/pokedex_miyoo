#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() {
    // surface
    surf_logo = NULL;
    fontSurface = NULL;
    IDSurface = NULL;
    listEntrySurface = NULL;
    listBackgroundSurface = NULL;
    // initialize variables
    dbResults = NULL;
    selectedIndex = 0, offset = 0, itemHeight = static_cast<int>(WINDOW_HEIGHT / 5);

    // color maps
    color = { 255, 255, 255 };
    highlightColor = { 255, 0, 0 };
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";
    int language_id = languageMap.at("es");
    PokedexDB::setLanguageVersion(language_id);

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (auto& game : *dbResults) {
        for (auto& col : game) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 20); 
    if (!fontSurface) {
        std::cerr << "PokedexActivityMenu::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }


    std::string gameVersion = "red";
    PokedexDB::setGameVersion(gameVersion);

    int regionVersion = regionMap.at(gameVersion);
    PokedexDB::setRegionVersion(regionVersion);
    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
    }       
    TTF_CloseFont(fontSurface);
    TTF_Quit();
}

void PokedexActivityMenu::onLoop() {
    //PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));
    //std::cout << "PokedexActivityMenu::onRender START \n";

    std::string backgroundImageFile = "res/icons/icon/menu_background.png";
    listBackgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, listBackgroundSurface,
       backgroundRect.x, 
       backgroundRect.y, 
       backgroundRect.w, 
       backgroundRect.h 
    );
    SDL_FreeSurface(listBackgroundSurface);

    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++) {
        //std::cout << "starting.." << i << std::endl;
        //std::cout << "selectedIndex: " << selectedIndex << std::endl;
        //std:: cout << "offset: " << offset << std::endl;

        std::vector<std::string> game = (*dbResults)[offset + i];

        backgroundImageFile = "res/icons/icon/menu_item_background_";
        offset + i == selectedIndex ? backgroundImageFile.append("selected.png") : backgroundImageFile.append("default.png");
        listEntrySurface = PokeSurface::onLoadImg(backgroundImageFile);

        SDL_Rect listEntryRect;
        //listEntryRect.x = surf_display->w - (surf_display->w * 0.8);
        listEntryRect.x = surf_display->w - (surf_display->w * 0.9);
        listEntryRect.y = (i * itemHeight);
        listEntryRect.w = surf_display->w * 0.9;
        listEntryRect.h = itemHeight;
        /*PokeSurface::onDrawScaled(surf_display, listEntrySurface,
            listEntryRect.x,
            listEntryRect.y,
            listEntryRect.w,
            listEntryRect.h
        );*/

        surf_logo = TTF_RenderText_Blended(
            fontSurface,
            game[2].c_str(),
            offset + i == selectedIndex ? highlightColor : color
        );
        SDL_SetSurfaceBlendMode(listEntrySurface, SDL_BLENDMODE_BLEND);
        if (surf_logo == NULL) {
            std::cout << "Unable to render text! SDL Error: pokeListIDSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };
        SDL_Rect gameVersionRect;
        gameVersionRect.x = (listEntryRect.w / 2) - (surf_logo->w / 2);
        gameVersionRect.y = (listEntryRect.h / 2) - (surf_logo->h / 2);
        gameVersionRect.w = surf_logo->w;
        gameVersionRect.h = surf_logo->h;
        PokeSurface::onDraw(listEntrySurface, surf_logo,
            gameVersionRect.x,
            gameVersionRect.y,
            gameVersionRect.w,
            gameVersionRect.h
        );
        SDL_FreeSurface(surf_logo);

        PokeSurface::onDrawScaled(surf_display, listEntrySurface,
            listEntryRect.x,
            listEntryRect.y,
            listEntryRect.w,
            listEntryRect.h
        );

        
    //std::cout << "PokedexActivityMenu::onRender END \n";
    }
    // At the end, free listEntrySurface if no longer needed
    if (listEntrySurface) {
        SDL_FreeSurface(listEntrySurface);
    }
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}

void PokedexActivityMenu::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
    }
}

void PokedexActivityMenu::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}

void PokedexActivityMenu::onButtonA(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}

