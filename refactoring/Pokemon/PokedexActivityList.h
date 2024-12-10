#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    SDL_Surface* surf_logo;

    int StartTime;

private:
    PokedexActivityList();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display);

public:
    static PokedexActivityList* getInstance();
};

#endif