#ifndef __POKEDEXACTIVITY_POKEMONVIEW_INFO__
#define __POKEDEXACTIVITY_POKEMONVIEW_INFO__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Info : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Info instance;

    Pokemon* pokemon;

    std::vector<std::vector<std::string>>* dbResults;

    std::string fontPath;

    SDL_Surface* pokeIDSurface, * pokeNameSurface,
        * pokeIconSurface, * pokeType1Surface,
        * pokeType2Surface, * backgroundSurface,
        * pokeHeightSurface, * pokeWeightSurface,
        * flavorTextSurface, * pokeGenusSurface;
    TTF_Font* fontSurface;

private:
    PokedexActivity_PokemonView_Info();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool renderSprites(SDL_Surface* surf_display);

    bool renderNameID(SDL_Surface* surf_display);

    bool renderHW(SDL_Surface* surf_display);

    bool renderFlavorText(SDL_Surface* surf_display);

    void onFreeze();

public:
    static PokedexActivity_PokemonView_Info* getInstance();

    void onButtonB(SDL_Keycode sym, Uint16 mod);

    void onButtonRight(SDL_Keycode sym, Uint16 mod);

    void onButtonLeft(SDL_Keycode sym, Uint16 mod);
};

#endif
