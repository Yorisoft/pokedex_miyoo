#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokedexActivity.h"
#include<stack>

enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_POKEDEX_MENU,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEMON_ENTRY
};

class PokedexActivityManager {
private:
    static PokedexActivity* activity;
    
    static std::stack<PokedexActivity*> sceneStack;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    static void setActiveState(int AppStateID);

    static PokedexActivity* getActiveState();

    static void push(int AppStateID);

    static void back();
};

#endif
