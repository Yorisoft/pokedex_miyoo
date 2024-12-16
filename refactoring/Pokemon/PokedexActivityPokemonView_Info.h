#ifndef __POKEDEXACTIVITY_POKEMONVIEW_INFO__
#define __POKEDEXACTIVITY_POKEMONVIEW_INFO__

#include "PokedexActivity.h"

class PokedexActivity_PokemonView_Info : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Info instance;

    std::vector<std::vector<std::string>>* dbResults;

    SDL_Surface* pokeIDSurface, * pokeNameSurface,
        * pokeIconSurface, * pokeType1Surface,
        * pokeType2Surface, * backgroundSurface;

private:
    PokedexActivity_PokemonView_Info();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    void onFreeze();

public:
    static PokedexActivity_PokemonView_Info* getInstance();
};

#endif
