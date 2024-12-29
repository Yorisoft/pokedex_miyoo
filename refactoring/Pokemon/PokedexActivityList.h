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
    SDL_Color color, highlightColor;

    int selectedIndex, offset, itemHeight;
    std::string fontPath;

private:
    PokedexActivityList();
    ~PokedexActivityList();

    void onButtonUp(SDL_Keycode sym, Uint16 mod);
    void onButtonDown(SDL_Keycode sym, Uint16 mod);
    void onButtonA(SDL_Keycode sym, Uint16 mod);
    void onButtonB(SDL_Keycode sym, Uint16 mod);

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    void onFreeze();

    bool renderListItems(SDL_Surface* surf_display, int i);

    SDL_Rect renderItemBackground(SDL_Surface* surf_display, int i);

    bool renderItemSprites(SDL_Surface* surf_display, int i);

    bool renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, int i);

    static PokedexActivityList* getInstance();

};

#endif