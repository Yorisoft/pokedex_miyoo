#ifndef POKEDEXACTIVITY_H
#define POKEDEXACTIVITY_H

#include <sstream>
#include <map>
#include <iomanip>
#include "PokedexActivityEvent.h"
#include "PokeSurface.h"
#include "PokedexDB.h"
#include "SQLstatements.h"

class PokedexActivity : public PokedexActivityEvent {
public:
    PokedexActivity();

    virtual void onActivate() = 0;

    virtual void onDeactivate() = 0;

    virtual void onLoop() = 0;

    virtual void onFreeze() = 0;

    virtual void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) = 0;
};

#endif