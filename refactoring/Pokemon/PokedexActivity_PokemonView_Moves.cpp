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
dbResults(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
    fontPath = "res/font/Pokemon_GB.ttf";
    // color maps
    color = { 64, 64, 64};
    highlightColor = { 255, 0, 0 };

}

void PokedexActivity_PokemonView_Moves::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    dbResults = PokedexDB::executeSQL(&SQL_getPokeMovesDetail);
    for (std::vector<std::string>&moves : *dbResults) {
        for (auto& col : moves) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    move = (*dbResults)[selectedIndex];


    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {
}

void PokedexActivity_PokemonView_Moves::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
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
        if (!renderListItems(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface* surf_display, int i) {
    //Render list item background/surface
    listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        surf_display->w * 0.9,
        itemHeight,
        DEPTH,
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
        return 1;
    }

    // Fill the surface with transparent color
    //SDL_FillRect(listEntrySurface, NULL, SDL_MapRGBA(listEntrySurface->format, 150, 150, 150, 255));

    int spacing = 18; 
    SDL_Rect listEntryRect;
    listEntryRect.x = surf_display->w - (surf_display->w * 0.45) - 8;
    listEntryRect.y = 65 + (i * (itemHeight + spacing));
    listEntryRect.w = surf_display->w * 0.45;
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

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
    moveTypeRect.w = moveTypeSurface->w * 1.5;
    moveTypeRect.h = moveTypeSurface->h * 1.5;
    PokeSurface::onDrawScaled(surf_display, moveTypeSurface, &moveTypeRect);
    SDL_FreeSurface(moveTypeSurface);

    //Render item title/name
    // load font
    fontSurface = TTF_OpenFont(fontPath.c_str(), 14);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Moves::onRender: Failed to load font: " << TTF_GetError() << std::endl;
    }
    
    // make all upper case
    std::string moveName = move[1];
    for (int i = 0; i < moveName.size(); i++) {
        moveName[i] = std::toupper(moveName[i]);
    }
    moveNameSurface = TTF_RenderText_Blended(
        fontSurface,
        moveName.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (moveNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveNameRect;
    moveNameRect.x = 20 + moveTypeRect.x + moveTypeRect.w;
    moveNameRect.y = moveTypeRect.y + 10;
    moveNameRect.w = moveNameSurface->w;
    moveNameRect.h = moveNameSurface->h;
    PokeSurface::onDraw(surf_display, moveNameSurface, &moveNameRect);
    SDL_FreeSurface(moveNameSurface);

    // closing font
    TTF_CloseFont(fontSurface);

    // Render method
    // load font
    fontSurface = TTF_OpenFont(fontPath.c_str(), 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Moves::onRender: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::string methodString = move[9];
    if (methodString == "level-up") {
        methodString = "LV." + move[8];
        moveMethodSurface = TTF_RenderText_Blended(
            fontSurface,
            methodString.c_str(),
            offset + i == selectedIndex ? highlightColor : color
        );
    }
    else {
        moveMethodSurface = TTF_RenderText_Blended(
            fontSurface,
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

    // closing font
    TTF_CloseFont(fontSurface);


    // Render PP
    // load font
    fontSurface = TTF_OpenFont(fontPath.c_str(), 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Moves::onRender: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::string ppString = move[3] + '/' + move[3];
    movePPSurface = TTF_RenderText_Blended(
        fontSurface,
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

    // closing font
    TTF_CloseFont(fontSurface);

    // Render selected Item details
    if (offset + i == selectedIndex) {
        //redern item details
        if (!renderItemDetails(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface* surf_display, int i) {
    //get move details
    std::string pokeIconPath = PokedexDB::getPokemonIdentifier();
    pokeIconPath = "res/icons/" + pokeIconPath + ".png";

    pokeIconSurface = PokeSurface::onLoadImg(pokeIconPath);
    if (pokeIconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIconSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect;
    pokeIconRect.x = 10;
    pokeIconRect.y = 40;
    pokeIconRect.w = pokeIconSurface->w * 2;
    pokeIconRect.h = pokeIconSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);
    SDL_FreeSurface(pokeIconSurface);

    return true;
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

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB

    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod) {

    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB
    //move = (*dbResults)[selectedIndex];
    //PokedexDB::setPokemonIdentifier(move[1]);

    ////Call next activity
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);

}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod) {
}
