#include "PokedexActivityManager.h"

#include "PokeActivityIntro.h"
#include "PokeActivityMenu.h"
#include "PokeActivityPokedexList.h"
#include "PokeActivityPokemonEntry.h"

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
    if (activity) {
        activity->onDeativate();
    }

    // Change state to be the desired activity
    switch (AppStateID) {
    case 0:
        activity = 0;
        break;
    case 1:
        activity = PokeActivityIntro::GetInstance();
        break;
    case 2:
        activity = PokeActivityMenu::GetInstance();
        break;
    case 3:
        activity = PokeActivityPokedexList::GetInstance();
        break;
    case 4:
        activity = PokeActivityPokemonEntry::GetInstance();
        break;
    }

    if (activity) {
        activity->onActivate();
    }
}

PokedexActivity* PokedexActivityManager::getActiveState() {
    return activity;
}