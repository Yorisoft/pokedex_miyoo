#ifndef __POKEDEXACTIVITY_POKEMONVIEW_LOCATION__
#define __POKEDEXACTIVITY_POKEMONVIEW_LOCATION__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Location : public PokedexActivity {
private: 
    static PokedexActivity_PokemonView_Location instance;

    Pokemon* pokemon;
    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::vector<std::string>>* routes;
    std::vector<std::string> route;
    
    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;
    SDL_Surface* backgroundSurface,
        * pokeIconSurface, * pokeNameSurface, 
        * pokeType1Surface, * pokeType2Surface,
        * listEntrySurface, 
        * routeNameSurface, * routeMethodSurface, * routeRateSurface,
        * routeLvlSurface, * routeSmrySurface;

public:
    PokedexActivity_PokemonView_Location();

    void onActivate();

    void printPokeInfo();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool renderPokeInfo(SDL_Surface* surf_display);

    bool renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, int i);

    bool renderItemDetails(SDL_Surface* surf_display, int i);

    void onFreeze();

    static PokedexActivity_PokemonView_Location* getInstance();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);

    void onButtonDown(SDL_Keycode sym, Uint16 mod);

    void onButtonLeft(SDL_Keycode sym, Uint16 mod);

    void onButtonRight(SDL_Keycode sym, Uint16 mod);

    void onButtonA(SDL_Keycode sym, Uint16 mod);

    void onButtonB(SDL_Keycode sym, Uint16 mod);
};

#endif
