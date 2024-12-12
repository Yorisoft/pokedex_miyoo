#ifndef POKEDEXACTIVITY_H
#define POKEDEXACTIVITY_H

#include "PokedexActivityEvent.h"
#include <sstream>
#include <map>
#include <vector>
#include "PokedexDB.h"
#include "SQLstatements.h"
#include "Mappings.h"

class PokedexActivity : public PokedexActivityEvent {
public:
    PokedexActivity();

    virtual void onActivate() = 0;

    virtual void onDeactivate() = 0;

    virtual void onLoop() = 0;

    virtual void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) = 0;
};

#endif