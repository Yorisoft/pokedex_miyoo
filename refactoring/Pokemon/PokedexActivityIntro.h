#ifndef __POKEDEXACTIVITYINTO_H__
#define __POKEDEXACTIVITYINTO_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"

class PokedexActivityIntro : public PokedexActivity {
private:
    static PokedexActivityIntro instance;

    SDL_Surface* surf_logo;

    int StartTime;        
    Uint8 logoAlpha;

private:
    PokedexActivityIntro();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display);

public:
    static PokedexActivityIntro* getInstance();
};

#endif
