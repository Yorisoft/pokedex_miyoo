#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

    SDL_Surface* surf_logo;
    SDL_Surface* listEntrySurface;
    SDL_Surface* listBackgroundSurface;
    TTF_Font* fontSurface;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> game;
    int selectedIndex, offset, itemHeight;

    SDL_Color color;
    SDL_Color highlightColor;

    PokedexActivityMenu();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    bool renderListItems(SDL_Surface* surf_display, int i);

    void onFreeze();

    static PokedexActivityMenu* getInstance();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);

    void onButtonDown(SDL_Keycode sym, Uint16 mod);

    void onButtonA(SDL_Keycode sym, Uint16 mod);

};

#endif
