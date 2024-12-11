#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokedexActivity.h"

enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEDEX_MENU,
    APPSTATE_POKEMON_ENTRY
};

class PokedexActivityManager {
private:
    static PokedexActivity* activity;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    static void setActiveState(int AppStateID);

    static PokedexActivity* getActiveState();
};

#endif
