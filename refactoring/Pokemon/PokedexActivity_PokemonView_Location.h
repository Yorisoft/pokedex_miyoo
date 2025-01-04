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

    Mix_Chunk* pokeCry, * sEffect;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;
    SDL_Surface* backgroundSurface,
        * pokeIconSurface, * pokeNameSurface, 
        * pokeType1Surface, * pokeType2Surface,
        * listEntrySurface, 
        * routeNameSurface, * routeMethodSurface, * routeRateSurface,
        * routeLvlSurface, * routeSmrySurface;

    void printPokeInfo();

    bool renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font);
    bool renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i);
    bool renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i);
    
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
    PokedexActivity_PokemonView_Location();
    ~PokedexActivity_PokemonView_Location();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();

    static PokedexActivity_PokemonView_Location* getInstance();

};

#endif
