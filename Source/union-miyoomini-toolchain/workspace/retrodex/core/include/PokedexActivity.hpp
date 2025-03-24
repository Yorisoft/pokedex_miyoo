#ifndef POKEDEXACTIVITY_H
#define POKEDEXACTIVITY_H

#include "PokeSurface.hpp"
#include "PokedexActivityEvent.hpp"
#include "PokedexDB.hpp"
#include "SQLStatementStore.hpp"
#include <iomanip>
#include <map>
#include <sstream>

class PokedexActivity : public PokedexActivityEvent
{
  public:
    PokedexActivity();

    virtual void onActivate() = 0;

    virtual void onDeactivate() = 0;

    virtual void onFreeze() = 0;

    // virtual void onResume() = 0;

    virtual void onLoop() = 0;

    virtual void onRender(SDL_Surface *surf_display, SDL_Renderer *renderer, SDL_Texture *texture,
                          TTF_Font *font, Mix_Chunk *sEffect) = 0;
};

#endif