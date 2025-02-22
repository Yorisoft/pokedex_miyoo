#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() :
    dbResults(nullptr),
    fontSurface(nullptr),
    sEffect(nullptr),
    selectedIndex(0),
    offset(0),
    itemHeight(0)
{
}

PokedexActivityMenu::~PokedexActivityMenu() {
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";

    color = { 248, 248, 248 }, highlightColor = { 255, 0, 0 };
    
    itemHeight = static_cast<int>(WINDOW_HEIGHT / 5);

    fontPath = "res/font/pokemon-dppt/pokemon-dppt.ttf";

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (std::vector<std::string>& game : *dbResults) {
        for (auto& col : game) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    game = (*dbResults)[selectedIndex];

    fontSurface = TTF_OpenFont(fontPath.c_str(), 46);
    if (!fontSurface) {
        std::cerr << "PokedexActivityMenu::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    sEffect = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() {
    if (fontSurface) 
        TTF_CloseFont(fontSurface);
    fontSurface = nullptr;

    if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;

    delete dbResults;
    dbResults = nullptr;

    color = { }, highlightColor = { };

    game.clear();
    fontPath.clear();

    selectedIndex = 0, offset = 0, itemHeight = 0;
}

void PokedexActivityMenu::onLoop() {
    //Set Game version and regional pokedex ID for PokedexDB
    game = (*dbResults)[selectedIndex];
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivityMenu::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Background
    std::string backgroundImageFile = "res/assets/misc/menu_background.png";
    SDL_Surface* listBackgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
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

    // List Items
    for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++) {
        if (!renderListItems(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }
}

bool PokedexActivityMenu::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    std::string backgroundImageFile = "res/assets/misc/menu_item_background_";
    offset + i == selectedIndex ? backgroundImageFile.append("selected.png") : backgroundImageFile.append("default.png");
    SDL_Surface* listEntrySurface = PokeSurface::onLoadImg(backgroundImageFile);
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

    //List item title ( Game Title )
    SDL_Surface* versionTitleSurface = TTF_RenderUTF8_Blended(
        fontSurface,
        (*dbResults)[offset + i][2].c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (versionTitleSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: versionTitleSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    int leftBorder = 15;
    SDL_Rect gameVersionRect;
    gameVersionRect.x = leftBorder + (WINDOW_WIDTH/2) - (versionTitleSurface->w / 2);
    gameVersionRect.y = (i * itemHeight) + (listEntryRect.h / 2) - (versionTitleSurface->h / 2) - 10;
    gameVersionRect.w = versionTitleSurface->w;
    gameVersionRect.h = versionTitleSurface->h;
    PokeSurface::onDraw(surf_display, versionTitleSurface, &gameVersionRect);
    SDL_FreeSurface(versionTitleSurface);

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
        if (selectedIndex < offset) {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
}

void PokedexActivityMenu::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
}

void PokedexActivityMenu::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonA(SDL_Keycode sym, Uint16 mod) {
    PokedexDB::setVersionID(std::stoi(game[0]));
    PokedexDB::setGenerationID(std::stoi(game[5]));
    PokedexDB::setVersionGroupID(std::stoi(game[7]));

    PokedexActivityManager::push(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onButtonB(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivityMenu::onButtonR(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < dbResults->size() - 3) {
        selectedIndex += 3;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += 3;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
    else {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityMenu::onButtonL(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex >= 3) {
        selectedIndex -= 3;
        if (selectedIndex < offset) {
            offset -= 3; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0) {
                offset = 0;  // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
    else {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset = 0;  // Cap offset to zero
    }
}

void PokedexActivityMenu::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
