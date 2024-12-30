#ifndef __POKEDEXACTIVITY_SETTINGS__ 
#define __POKEDEXACTIVITY_SETTINGS__ 

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_Settings : public PokedexActivity {
private:
    static PokedexActivity_Settings instance;
    std::map<int, std::string> language_map;


    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    SDL_Surface* backgroundSurface;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;

    bool renderListItems(SDL_Surface* surf_display, int i);
    void printOptionsInfo();

public:
    PokedexActivity_Settings();
    ~PokedexActivity_Settings();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);
    void onFreeze();

    static PokedexActivity_Settings* getInstance();

    void onButtonUp(SDL_Keycode, Uint16);
    void onButtonDown(SDL_Keycode, Uint16);
    void onButtonLeft(SDL_Keycode, Uint16);
    void onButtonRight(SDL_Keycode, Uint16);
    void onButtonA(SDL_Keycode, Uint16);
    void onButtonB(SDL_Keycode, Uint16);
    void onButtonSelect(SDL_Keycode, Uint16);
    void onButtonStart(SDL_Keycode, Uint16);
};

#endif
