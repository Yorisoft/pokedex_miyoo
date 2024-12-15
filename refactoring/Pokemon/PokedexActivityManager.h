#ifndef POKEACTIVITYMANAGER_H
#define POKEACTIVITYMANAGER_H

#include "PokedexActivity.h"
#include <stack>
#include<variant>
#include<map>
enum {
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_POKEDEX_LIST,
    APPSTATE_POKEDEX_MENU,
    APPSTATE_POKEMON_ENTRY
};

using VariantType = std::variant<int, std::string>;

class PokedexActivityManager {
private:
    static PokedexActivity* activity;
    
    static std::stack<PokedexActivity*> sceneStack;

    static std::map<std::string, VariantType> prop;

public:
    static void onEvent(SDL_Event* Event);

    static void onLoop();

    static void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    static void setProp(const char* key, VariantType val);

    static void setActiveState(int AppStateID);

    static PokedexActivity* getActiveState();

    static void push(int AppStateID);
    //
    static void back();
};

#endif
