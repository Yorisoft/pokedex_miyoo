#ifndef __POKEACTIVITYINTO_H__
#define __POKEACTIVITYINTO_H__

#include "PokedexActivity.h"
#include "ActivitySurface.h"

class PokeActivityIntro : public PokedexActivity {
private:
    static PokeActivityIntro instance;

    SDL_Surface* surf_logo;

    int StartTime;

private:
    PokeActivityIntro();

public:
    void OnActivate();

    void OnDeactivate();

    void OnLoop();

    void OnRender(SDL_Surface* surf_display);

public:
    static PokeActivityIntro* GetInstance();
};

#endif
