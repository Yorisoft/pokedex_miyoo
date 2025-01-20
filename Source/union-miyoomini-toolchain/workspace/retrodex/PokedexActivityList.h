#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    int selectedIndex, offset, itemHeight;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> pokemon;

    Mix_Chunk* sEffect, *sEffect_OnStart, *sEffect_OnExit;
    SDL_Color color, highlightColor;

    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);
    SDL_Rect renderItemBackground(SDL_Surface* surf_display, int i);
    bool renderItemSprites(SDL_Surface* surf_display, int i);
    bool renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i);

private:
    PokedexActivityList();
    ~PokedexActivityList();

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
    static PokedexActivityList* getInstance();

};
#endif