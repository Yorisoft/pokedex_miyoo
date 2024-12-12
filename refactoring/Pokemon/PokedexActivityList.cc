#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() {
    surf_logo = NULL;

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

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityList::onLoop() {
}

void PokedexActivityList::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));
    
    if (!pokedexList(surf_display, *dbResults)) {
        std::cout << "PokedexActivity::onRender: Coult not render Pokedex List: " << std::endl;
    }
    if (surf_logo) {
        PokeSurface::onDrawScaled(surf_display, surf_logo, 0, 0, 3, 3);
    }
    SDL_FreeSurface(surf_logo);
}

PokedexActivityList* PokedexActivityList::getInstance() {
    return &instance;
}

bool PokedexActivityList::pokedexList(SDL_Surface* targetSurface, const std::vector<std::vector<std::string>>& dbResults) {
    for (int i = 0; i < MAX_VISIBLE_ITEMS &&  + i < dbResults.size(); i++) {
        pokemonEntry(targetSurface, dbResults[i], i);
    };
    return true;
}

bool PokedexActivityList::pokemonEntry(SDL_Surface* targetSurface, const std::vector<std::string>& pokemon, int index) {
        //std::vector<std::string> pokemon = results[selectedItem + i];
        //// Get the type to determine color
        //std::string pokemonType = pokemon[8];
        //int pokemonID = std::stoi(pokemon[0]);
        //int colorIndex = (pokemonID - 1) % 2;

        //SDL_Color rainbowColors = {
        //    static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][0]),
        //    static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][1]),
        //    static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][2])
        //};

        //pokemonListEntrySurface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, pokemonListSurfaceHeight, DEPTH, 0, 0, 0, 0);
        //SDL_FillRect(pokemonListEntrySurface, NULL, SDL_MapRGBA(pokemonListEntrySurface->format, rainbowColors.r, rainbowColors.g, rainbowColors.b, 128));

        //int labelHeight = pokemonListEntrySurface->h;
        //SDL_Rect pokeListEntry{};
        //pokeListEntry.x = 0;
        //pokeListEntry.y = maxListLabelHeight + (i * pokemonListSurfaceHeight);
        //pokeListEntry.w = pokemonListEntrySurface->w;
        //pokeListEntry.h = pokemonListEntrySurface->h;

        //listFont = OpenFont(&listFont, fontPath, 15);
        //RenderPokemonID(&pokemonListEntrySurface, &listFont, pokemon, i);
        //TTF_CloseFont(listFont);

        //listFont = OpenFont(&listFont, fontPath, 15);
        //RenderPokemonListPixelArt(&pokemonListEntrySurface, &listFont, pokemon, i, maxListIDWidth);
        //TTF_CloseFont(listFont);

        //listFont = OpenFont(&listFont, fontPath, 12);
        //RenderPokemonMinimalStats(&pokemonListEntrySurface, &listFont, pokemon, i);
        //TTF_CloseFont(listFont);

        //SDL_BlitSurface(pokemonListEntrySurface, NULL, *screen, &pokeListEntry);
        //SDL_FreeSurface(pokemonListEntrySurface);
    return true;
}