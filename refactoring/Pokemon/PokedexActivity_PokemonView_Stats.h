#ifndef __POKEDEXACTIVITY_POKEMONVIEW_STATS__
#define __POKEDEXACTIVITY_POKEMONVIEW_STATS__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Stats : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Stats instance;

    std::vector<std::vector<std::string>>* dbResults;

    Pokemon* pokemon;

    TTF_Font* fontSurface;
    SDL_Surface* pokeNameSurface, * pokeIDSurface,
        * pokeIconSurface, * backgroundSurface,
        * abilitySurface1, * abilitySurface2, * abilitySurfaceHidden, 
        * statsSurface,
        * hpSurface, * atkSurface, * defSurface, * spAtkSurface, * spDefSurface, * spdSurface;

    std::string fontPath;

private:
    PokedexActivity_PokemonView_Stats();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);

    bool renderSprites(SDL_Surface* surf_display);

    bool renderNameID(SDL_Surface* surf_display, TTF_Font* font);

    bool renderStats(SDL_Surface* surf_display, TTF_Font* font);

    bool renderFlavorText(SDL_Surface* surf_display);

    void onFreeze();

public:
    static PokedexActivity_PokemonView_Stats* getInstance();

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
};

#endif