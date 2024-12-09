#pragma once
#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokedexActivity.h"

enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_MENU,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEMON_ENTRY
};

class PokedexActivityManager {
private:
    static PokedexActivity* activity;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surface_display);

    static void setActiveState(int AppStateID);

    static PokedexActivity* getActiveState();
};

#endif
