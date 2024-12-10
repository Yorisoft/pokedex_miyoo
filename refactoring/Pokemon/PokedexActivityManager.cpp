#include "PokedexActivityManager.h"

#include "PokedexActivityIntro.h"
#include "PokedexActivityList.h"
//#include "PokeActivityPokedexList.h"
//#include "PokeActivityPokemonEntry.h"

PokedexActivity* PokedexActivityManager::activity = 0;

void PokedexActivityManager::onEvent(SDL_Event* event) {
    if (activity) {
        activity->onEvent(event);
    }
}

void PokedexActivityManager::onLoop() {
    if (activity) {
        activity->onLoop();
    }
}

void PokedexActivityManager::onRender(SDL_Surface* surface_display) {
    if (activity) {
        activity->onRender(surface_display);
    }
}

void PokedexActivityManager::setActiveState(int AppStateID) {
    // push back current activity
    if (activity && activity != 0) {
        activity->onDeactivate();
    }

    // Change state to be the desired activity
    switch (AppStateID) {
    case 0:
        activity = 0;
        break;
    case 1:
        activity = PokedexActivityIntro::getInstance();
        break;
    case 2:
        activity = PokedexActivityList::getInstance();
        break;
   /* case 3:
        activity = PokeActivityPokedexList::getInstance();
        break;
    case 4:
        activity = PokeActivityPokemonEntry::getInstance();
        break;*/
    default:
        break;
    }

    if (activity) {
        activity->onActivate();
    }
}

PokedexActivity* PokedexActivityManager::getActiveState() {
    return activity;
}