#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() : 
gameNameSurface(nullptr),
fontSurface(nullptr),
listEntrySurface(nullptr),
listBackgroundSurface(nullptr),
dbResults(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT / 5))
{
    // color maps
    color = { 255, 255, 255 };
    highlightColor = { 255, 0, 0 };
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (auto& game : *dbResults) {
        for (auto& col : game) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    game = (*dbResults)[selectedIndex];

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 28); 
    if (!fontSurface) {
        std::cerr << "PokedexActivityMenu::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() {
    selectedIndex = 0, offset = 0;
    if (gameNameSurface) {
        SDL_FreeSurface(gameNameSurface);
    }       
    TTF_CloseFont(fontSurface);
}

void PokedexActivityMenu::onLoop() {

}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    //std::cout << "PokedexActivityMenu::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render List Items
    std::string backgroundImageFile = "res/icons/icon/menu_background.png";
    listBackgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (listBackgroundSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: listBackgroundSurface " << SDL_GetError() << std::endl;
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
}

bool PokedexActivityMenu::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    std::string backgroundImageFile = "res/icons/icon/menu_item_background_";
    offset + i == selectedIndex ? backgroundImageFile.append("selected.png") : backgroundImageFile.append("default.png");
    listEntrySurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (listEntrySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect listEntryRect;
    listEntryRect.x = 0;
    listEntryRect.y = (i * itemHeight);
    listEntryRect.w = surf_display->w;
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

    //List item title
    gameNameSurface = TTF_RenderText_Blended(
        fontSurface,
        (*dbResults)[offset + i][2].c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (gameNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: gameNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect gameVersionRect;
    gameVersionRect.x = listEntryRect.x + (listEntryRect.w / 2) - (gameNameSurface->w / 2);
    gameVersionRect.y = (i * itemHeight) + (listEntryRect.h / 2) - (gameNameSurface->h / 2);
    gameVersionRect.w = gameNameSurface->w;
    gameVersionRect.h = gameNameSurface->h;
    PokeSurface::onDraw(surf_display, gameNameSurface, &gameVersionRect);
    SDL_FreeSurface(gameNameSurface);

    return true;
}

void PokedexActivityMenu::onFreeze() {
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}

void PokedexActivityMenu::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        game = (*dbResults)[selectedIndex];
        if (selectedIndex < offset) {
            offset--;
        }
    }
}

void PokedexActivityMenu::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        game = (*dbResults)[selectedIndex];
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}

void PokedexActivityMenu::onButtonA(SDL_Keycode sym, Uint16 mod) {
    //Set Game version and regional pokedex ID for PokedexDB
    PokedexDB::setGameVersion(game[1]);
    PokedexDB::setRegionVersion(std::stoi(game[3]));
    PokedexDB::setGenerationID(std::stoi(game[5]));
    PokedexDB::setVersionGroupID(std::stoi(game[7]));

    PokedexActivityManager::push(APPSTATE_POKEDEX_LIST);
}

