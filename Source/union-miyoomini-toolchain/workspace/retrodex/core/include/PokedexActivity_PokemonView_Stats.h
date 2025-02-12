#ifndef __POKEDEXACTIVITY_POKEMONVIEW_STATS__
#define __POKEDEXACTIVITY_POKEMONVIEW_STATS__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Stats : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Stats instance;

    std::string fontPath;

    std::vector<std::vector<std::string>>* dbResults;

    Pokemon* pokemon;

    Mix_Chunk* sEffect;

private:
    PokedexActivity_PokemonView_Stats();
    bool renderSprites(SDL_Surface* surf_display);
    bool renderNameID(SDL_Surface* surf_display, TTF_Font* font);
    bool renderStats(SDL_Surface* surf_display, TTF_Font* font);
    bool renderFlavorText(SDL_Surface* surf_display);

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
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();

    static PokedexActivity_PokemonView_Stats* getInstance();
};
#endif