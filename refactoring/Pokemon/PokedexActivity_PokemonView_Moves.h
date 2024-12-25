#ifndef __POKEDEXACTIVITY_POKEMONVIEW_MOVES__
#define __POKEDEXACTIVITY_POKEMONVIEW_MOVES__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Moves : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Moves instance;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> move;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    SDL_Surface* backgroundSurface, * listEntrySurface,
        * moveNameSurface, * moveTypeSurface, * movePPSurface,
        * moveMethodSurface, * pokeIconSurface;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;

    PokedexActivity_PokemonView_Moves();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool renderItemDetails(SDL_Surface* surf_display, int i);

    bool renderListItems(SDL_Surface* surf_display, int i);

    void onFreeze();

    static PokedexActivity_PokemonView_Moves* getInstance();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);

    void onButtonDown(SDL_Keycode sym, Uint16 mod);

    void onButtonLeft(SDL_Keycode sym, Uint16 mod);

    void onButtonRight(SDL_Keycode sym, Uint16 mod);

    void onButtonA(SDL_Keycode sym, Uint16 mod);

    void onButtonB(SDL_Keycode sym, Uint16 mod);
};

#endif
