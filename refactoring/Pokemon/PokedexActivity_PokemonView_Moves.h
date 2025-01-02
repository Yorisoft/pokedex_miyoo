#ifndef __POKEDEXACTIVITY_POKEMONVIEW_MOVES__
#define __POKEDEXACTIVITY_POKEMONVIEW_MOVES__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Moves : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Moves instance;

    Pokemon* pokemon;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> move;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    SDL_Surface* backgroundSurface, * listEntrySurface,
        * moveNameSurface, * moveTypeSurface, * movePPSurface,
        * moveMethodSurface, * pokeIconSurface,
        * pokeType1Surface, * pokeType2Surface,
        * pokeNameSurface, * moveClassSurface,
        * movePwrSurface, * moveAcrySurface, * moveEffectSurface;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;

    PokedexActivity_PokemonView_Moves();

    bool renderItemDetails(SDL_Surface* surf_display, int i);
    bool renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, int i);
    std::string cleanString(std::string str);

    
    void onButtonUp(SDL_Keycode, Uint16);
    void onButtonDown(SDL_Keycode, Uint16);
    void onButtonLeft(SDL_Keycode, Uint16);
    void onButtonRight(SDL_Keycode, Uint16);
    void onButtonA(SDL_Keycode, Uint16);
    void onButtonB(SDL_Keycode, Uint16);
    void onButtonR(SDL_Keycode, Uint16);
    void onButtonL(SDL_Keycode, Uint16);
    void onButtonSelect(SDL_Keycode, Uint16);
    void onButtonStart(SDL_Keycode, Uint16);

public:
    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);
    void onFreeze();

    static PokedexActivity_PokemonView_Moves* getInstance();

};

#endif
