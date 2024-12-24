#ifndef __POKEDEXACTIVITY_POKEMONVIEW_MOVES__
#define __POKEDEXACTIVITY_POKEMONVIEW_MOVES__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Moves : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Moves instance;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> move;

    Pokemon* pokemon;

    TTF_Font* fontSurface;
    SDL_Surface* backgroundSurface, * listEntrySurface,
        * moveNameSurface, * pokeIDSurface,
        * pokeIconSurface, * moveTypeSurface,
        * pwrSurface, * acrySurface, * effectSurface;
    SDL_Color color;
    SDL_Color highlightColor;

    int selectedIndex, offset, itemHeight;

    std::string fontPath;

private:
    PokedexActivity_PokemonView_Moves();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool renderListItems(SDL_Surface* surf_display, int i);

    void onFreeze();

public:
    static PokedexActivity_PokemonView_Moves* getInstance();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);

    void onButtonDown(SDL_Keycode sym, Uint16 mod);

    void onButtonRight(SDL_Keycode sym, Uint16 mod);

    void onButtonLeft(SDL_Keycode sym, Uint16 mod);

    void onButtonA(SDL_Keycode sym, Uint16 mod);

    void onButtonB(SDL_Keycode sym, Uint16 mod);

};

#endif
