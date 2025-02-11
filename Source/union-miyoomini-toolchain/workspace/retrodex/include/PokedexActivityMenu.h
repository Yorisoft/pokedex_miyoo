#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

    std::vector<std::string> game;
    std::vector<std::vector<std::string>>* dbResults;

    SDL_Color color, highlightColor;
    TTF_Font* fontSurface;
    Mix_Chunk* sEffect;

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
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();
    static PokedexActivityMenu* getInstance();
};

#endif
