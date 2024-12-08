#pragma once
#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokeActivity.h"

enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_MENU,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEMON_ENTRY
};

class PokedexActivityManager {
private:
    static PokeActivity* activity;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surface_display);

    static void setActiveState(int AppStateID);

    static PokeActivity* getActiveState();
};

#endif
