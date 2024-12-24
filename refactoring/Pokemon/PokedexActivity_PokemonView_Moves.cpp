#include "PokedexActivity_PokemonView_Moves.h"
#include "PokedexActivityManager.h"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves() :
backgroundSurface(nullptr),
listEntrySurface(nullptr),
moveNameSurface(nullptr),
pokeIconSurface(nullptr),
moveTypeSurface(nullptr),
pokeIDSurface(nullptr),
fontSurface(nullptr),
dbResults(nullptr),
pokemon(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
    fontPath = "res/font/Pokemon_GB.ttf";
    // color maps
    color = { 255, 255, 255 };
    highlightColor = { 255, 0, 0 };

}

void PokedexActivity_PokemonView_Moves::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    dbResults = PokedexDB::executeSQL(&SQL_getPokeMoves);
    for (std::vector<std::string>&moves : *dbResults) {
        for (auto& col : moves) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Moves::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate() {
    selectedIndex = 0, offset = 0;
    if (moveNameSurface) {
        SDL_FreeSurface(moveNameSurface);
        moveNameSurface = NULL;
    }
    TTF_CloseFont(fontSurface);
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
    // At the end, free listEntrySurface if no longer needed
    //if (listEntrySurface) {
    //    SDL_FreeSurface(listEntrySurface);
    //}
}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface* surf_display, int i) {
    listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        surf_display->w * 0.9,
        itemHeight,
        32, 
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
        return 1;
    }

    // Fill the surface with transparent color
    Uint32 transparentColor = SDL_MapRGBA(listEntrySurface->format, 150, 150, 150, 100); // RGBA(0, 0, 0, 0)
    //SDL_FillRect(listEntrySurface, NULL, transparentColor);

    //List item title
    int spacing = 18; // Adjust this value for more or less space between items

    SDL_Rect listEntryRect;
    listEntryRect.x = surf_display->w - (surf_display->w * 0.45) - 8;
    listEntryRect.y = 65 + (i * (itemHeight + spacing));
    listEntryRect.w = surf_display->w * 0.45;
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

    moveNameSurface = TTF_RenderText_Blended(
        fontSurface,
        move[1].c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (moveNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: moveNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveNameRect;
    moveNameRect.x = listEntryRect.x + (listEntryRect.w / 2) - (moveNameSurface->w / 2);
    moveNameRect.y = (i * itemHeight)+(listEntryRect.h / 2) - (moveNameSurface->h / 2);
    moveNameRect.w = moveNameSurface->w;
    moveNameRect.h = moveNameSurface->h;
    PokeSurface::onDraw(surf_display, moveNameSurface, &moveNameRect);
    SDL_FreeSurface(moveNameSurface);

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

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod) {

    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB

    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB
    move = (*dbResults)[selectedIndex];
    PokedexDB::setPokemonIdentifier(move[1]);

    ////Call next activity
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);

}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod) {
}

