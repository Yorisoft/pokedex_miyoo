#ifndef POKEDEXACTIVITY_H
#define POKEDEXACTIVITY_H

#include "PokedexActivityEvent.h"

class PokedexActivity : public PokedexActivityEvent {
public:
    PokedexActivity();

    virtual void onActivate() = 0;

    virtual void onDeactivate() = 0;

    virtual void onLoop() = 0;

    virtual void onRender(SDL_Surface* surf_display) = 0;
};

#endif