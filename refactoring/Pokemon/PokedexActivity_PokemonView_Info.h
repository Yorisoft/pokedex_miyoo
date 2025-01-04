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

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);

    bool renderSprites(SDL_Surface* surf_display);

    bool renderNameID(SDL_Surface* surf_display, TTF_Font* font);

    bool renderHW(SDL_Surface* surf_display, TTF_Font* font);

    bool renderFlavorText(SDL_Surface* surf_display, TTF_Font* font);

    void onFreeze();

public:
    static PokedexActivity_PokemonView_Info* getInstance();

     
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
