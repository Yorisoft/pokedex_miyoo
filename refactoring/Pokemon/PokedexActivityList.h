#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> pokemon;

    SDL_Surface* pokeIDSurface, *pokeNameSurface,
    *pokeIconSurface, *pokeType1Surface,
    *pokeType2Surface, *listEntrySurface,
    *listBackgroundSurface;

    TTF_Font* fontSurface;

    Mix_Chunk* sEffect, sEffect_OnB;

    SDL_Color color, highlightColor;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

private:
    PokedexActivityList();
    ~PokedexActivityList();

    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);
    SDL_Rect renderItemBackground(SDL_Surface* surf_display, int i);
    bool renderItemSprites(SDL_Surface* surf_display, int i);
    bool renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i);

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