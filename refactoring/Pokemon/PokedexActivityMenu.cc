#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() {
    // surface
    surf_logo = NULL;
    fontSurface = NULL;
    IDSurface = NULL;
    listEntrySurface = NULL;

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

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 18); 
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
        surf_logo = NULL;
    }       
    TTF_CloseFont(fontSurface);
    TTF_Quit();
}

void PokedexActivityMenu::onLoop() {
    //PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));
    std::cout << "PokedexActivityMenu::onRender START \n";

    for (int i = 0; i < MAX_VISIBLE_ITEMS && selectedIndex + i < dbResults->size(); i++) {
        std::cout << "starting.." << i << std::endl;
        std::cout << "selectedIndex: " << selectedIndex << std::endl;
        std:: cout << "offset: " << offset << std::endl;

        std::vector<std::string> game = (*dbResults)[selectedIndex + i];
        int gameID = stoi(game[0]);
        SDL_Color backgroundColors = {
            static_cast<Uint8>(gameColorMap.at(gameID)[0]),
            static_cast<Uint8>(gameColorMap.at(gameID)[1]),
            static_cast<Uint8>(gameColorMap.at(gameID)[2])
        };
        listEntrySurface = SDL_CreateRGBSurface(
            0,
            WINDOW_WIDTH,
            itemHeight,
            DEPTH,
            0, 0, 0, 0
        );
        SDL_FillRect(
            listEntrySurface,
            NULL,
            SDL_MapRGBA(
                listEntrySurface->format,
                backgroundColors.r,
                backgroundColors.g,
                backgroundColors.b,
                128
            )
        );

        SDL_Rect listEntryRect;
        listEntryRect.x = 0;
        listEntryRect.y = (i * itemHeight);
        listEntryRect.w = listEntrySurface->w;
        listEntryRect.h = listEntrySurface->h;


        surf_logo = TTF_RenderText_Blended(
            fontSurface,
            game[2].c_str(),
            i == 0 ? highlightColor : color
        );
        if (surf_logo == NULL) {
            std::cout << "Unable to render text! SDL Error: pokeListIDSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };
        
        SDL_Rect gameVersionRect;
        gameVersionRect.x = 0;
        gameVersionRect.y = (listEntryRect.h / 2) - (surf_logo->h / 2);
        gameVersionRect.w = listEntrySurface->w;
        gameVersionRect.h = listEntrySurface->h;
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
 
        SDL_FreeSurface(listEntrySurface);
    std::cout << "PokedexActivityMenu::onRender END \n";
    }
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}

void PokedexActivityMenu::onKeyDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex >= offset) {
            offset++;
        }
    }
}

void PokedexActivityMenu::onKeyUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset =  selectedIndex;
        }
    }
}