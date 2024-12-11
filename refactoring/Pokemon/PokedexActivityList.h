#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"
#include "SQLstatements.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    SDL_Surface* surf_logo;

    int StartTime;

    std::vector<std::vector<std::string>>* dbResults;
    //TTF_Font* listFont;
    SDL_Surface* pokeListLabelSurface, * pokemonListEntrySurface;
    std::ostringstream textStream;

    SDL_Color color;
    SDL_Color highlightColor;
    std::string fontPath;
    int pokemonListSurfaceHeight;


private:
    PokedexActivityList();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool pokedexList(SDL_Surface* targetSurface, const std::vector<std::vector<std::string>>& dbResults);

    bool pokemonEntry(SDL_Surface* targetSurface, const std::vector<std::string>& pokemon, int index);

public:
    static PokedexActivityList* getInstance();
};

   // Charcoal, Gunmetal
#endif