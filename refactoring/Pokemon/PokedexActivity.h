#ifndef POKEDEXACTIVITY_H
#define POKEDEXACTIVITY_H

#include "ActivityEvent.h"

class PokedexActivity : public ActivityEvent {
public:
    PokedexActivity();

    virtual void OnActivate() = 0;

    virtual void OnDeactivate() = 0;

    virtual void OnLoop() = 0;

    virtual void OnRender(SDL_Surface* surf_display) = 0;
};

#endif