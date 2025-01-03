#ifndef __POKEDEXACTIVITY_POKEMONVIEW_EVOLUTION__ 
#define __POKEDEXACTIVITY_POKEMONVIEW_EVOLUTION__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Evolution : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Evolution instance;
    std::vector<std::vector<std::string>>* evoChain;
    std::vector<std::string> evo;

    Pokemon* pokemon;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    SDL_Surface* backgroundSurface,
        * pokeSpriteSurface, * pokeIDSurface, * pokeNameSurface,
        * pokeMethodSurface, * listEntrySurface ;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;

    void printPokeInfo();
    bool renderPokeInfo(SDL_Surface* surf_display, int i);
    bool renderListItems(SDL_Surface* surf_display, int i);

public:
    PokedexActivity_PokemonView_Evolution();
    ~PokedexActivity_PokemonView_Evolution();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();

    static PokedexActivity_PokemonView_Evolution* getInstance();

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