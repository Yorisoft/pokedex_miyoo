#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> game;

    SDL_Surface* gameNameSurface, *listEntrySurface,
        *listBackgroundSurface;

    TTF_Font* fontSurface;

    SDL_Color color;
    SDL_Color highlightColor;
    
    std::string fontPath;
    int selectedIndex, offset, itemHeight;

    bool renderListItems(SDL_Surface* surf_display, int i);

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
    PokedexActivityMenu();
    ~PokedexActivityMenu();

    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    void onFreeze();

    static PokedexActivityMenu* getInstance();
};

#endif
