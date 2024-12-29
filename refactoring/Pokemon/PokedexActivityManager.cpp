#include "PokedexActivityManager.h"

#include "PokedexActivityIntro.h"
#include "PokedexActivityList.h"
#include "PokedexActivityMenu.h"
#include "PokedexActivity_PokemonView_Info.h"
#include "PokedexActivity_PokemonView_Stats.h"
#include "PokedexActivity_PokemonView_Moves.h"
#include "PokedexActivity_PokemonView_Location.h"
#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity* PokedexActivityManager::activity = 0;
std::stack<PokedexActivity*> PokedexActivityManager::sceneStack;

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

void PokedexActivityManager::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    if (activity) {
        activity->onRender(surf_display, renderer, texture);
    }
}

void PokedexActivityManager::setActiveState(int AppStateID) {
    // Change state to be the desired activity
    switch (AppStateID) {
    case 0:
        activity = 0;
        break;
    case 1:
        activity = PokedexActivityIntro::getInstance();
        break;
    case 2:
        activity = PokedexActivityMenu::getInstance();
        break;
    case 3:
        activity = PokedexActivityList::getInstance();
        break;
    case 4:
        activity = PokedexActivity_PokemonView_Info::getInstance();
        break;
    case 5:
        activity = PokedexActivity_PokemonView_Stats::getInstance();
        break;
    case 6:
        activity = PokedexActivity_PokemonView_Moves::getInstance();
        break;
    case 7:
        activity = PokedexActivity_PokemonView_Location::getInstance();
        break;
    case 8:
        activity = PokedexActivity_PokemonView_Evolution::getInstance();
        break;
    default:
        break;
    }

    // push back current activity
    if (activity && activity != 0) {
        sceneStack.push(activity);
        sceneStack.top()->onActivate();
    }
}

void PokedexActivityManager::push(int AppStateID) {
    std::cout << "PokedexActivityManager::push()  START" << std::endl;
    if (!sceneStack.empty()) {
        sceneStack.top()->onFreeze();
    }

    setActiveState(AppStateID);
    std::cout << "PokedexActivityManager::push()  END" << std::endl;
}

void PokedexActivityManager::back() {
    std::cout << "PokedexActivityManager::back()  START" << std::endl;
    PokedexActivity* current = sceneStack.top();
    // Call onDeactivate() before popping to ensure proper cleanup
    if (current) {
        current->onDeactivate();
        //current = nullptr;
    }
    sceneStack.pop();

    activity = sceneStack.empty() ? nullptr : sceneStack.top();
    if (!activity) {
        std::cout << "activity  empty. Nothing to go back to." << std::endl;
        return;
    }

    //current->onDeactivate();
    //delete current;

    if (!sceneStack.empty()) {
        std::cout << "!sceneStack.empty(). Calling activity->onActivate()." << std::endl;
        activity->onActivate();
    }
    std::cout << "PokedexActivityManager::back()  END" << std::endl;
}

PokedexActivity* PokedexActivityManager::getActiveState() {
    return activity;
}