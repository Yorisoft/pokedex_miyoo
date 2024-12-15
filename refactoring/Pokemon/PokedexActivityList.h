#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    SDL_Surface* surf_logo;
    SDL_Surface* pokeIconSurface;
    SDL_Surface* listEntrySurface;
    SDL_Surface* listBackgroundSurface;
    TTF_Font* fontSurface;
    SDL_Surface* IDSurface;

    int StartTime;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> pokemon;
    //TTF_Font* listFont;
    SDL_Surface* pokeListLabelSurface, * pokemonListEntrySurface;
    int selectedIndex, offset, itemHeight;
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

    void onFreeze();

    bool renderListItems(SDL_Surface* surf_display, int i);

public:
    static PokedexActivityList* getInstance();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);

    void onButtonDown(SDL_Keycode sym, Uint16 mod);

    void onButtonA(SDL_Keycode sym, Uint16 mod);

    void onButtonB(SDL_Keycode sym, Uint16 mod);
};

   // Charcoal, Gunmetal
#endif