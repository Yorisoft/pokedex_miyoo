#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokedexActivity.h"
#include<stack>

enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_POKEDEX_MENU,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEMON_VIEW_INFO,
    APPSTATE_POKEMON_VIEW_STATS,
    APPSTATE_POKEMON_VIEW_MOVES,
    APPSTATE_POKEMON_VIEW_LOCATION,
    APPSTATE_POKEMON_VIEW_EVOLUTION,
    APPSTATE_POKEDEX_SETTING   
};

class PokedexActivityManager {
private:
    static PokedexActivity* activity;
    
    static std::stack<PokedexActivity*> sceneStack;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);

    static void setActiveState(int AppStateID);

    static PokedexActivity* getActiveState();

    static void push(int AppStateID);

    static void back();

    static void replace(int AppStateID);
};

#endif
