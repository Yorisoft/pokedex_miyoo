#include "PokedexActivityManager.h"

#include "PokeActivityIntro.h"
#include "PokeActivityMenu.h"
#include "PokeActivityPokedexList.h"
#include "PokeActivityPokemonEntry.h"

PokeActivity* PokedexActivityManager::activity = 0;

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
        activity = PokeAvtivityIntro::GetInstance();
        break;
    case 2:
        activity = PokeAvtivityMenu::GetInstance();
        break;
    case 3:
        activity = PokeAvtivityPokedexList::GetInstance();
        break;
    case 4:
        activity = PokeAvtivityPokemonEntry::GetInstance();
        break;
    }

    if (activity) {
        activity->onActivate();
    }
}

PokeActivity* PokedexActivityManager::getActiveState() {
    return activity;
}