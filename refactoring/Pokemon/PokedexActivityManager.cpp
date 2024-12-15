#include "PokedexActivityManager.h"

#include "PokedexActivityIntro.h"
#include "PokedexActivityList.h"
#include "PokedexActivityMenu.h"
//#include "PokeActivityPokemonEntry.h"

PokedexActivity* PokedexActivityManager::activity = 0;

std::stack<PokedexActivity*> PokedexActivityManager::sceneStack;

std::map<std::string, VariantType> PokedexActivityManager::prop;

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
    //PokedexActivity* current = activity;
    // // push back current activity
    //if (current && current != 0) {
    //    sceneStack.push(current);
    //}

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
    case 3:
        activity = PokedexActivityMenu::getInstance();
        break;
    /*case 4:
        activity = PokeActivityPokemonEntry::getInstance();
        break;*/
    default:
        break;
    }

    // push back current activity
    if (activity && activity != 0) {
        sceneStack.push(activity);
        sceneStack.top()->onActivate();
    }
}

void PokedexActivityManager::setProp(const char* key, VariantType val) {
    if (key != nullptr) {
        prop[key] = val;
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
//

void PokedexActivityManager::back() {
    std::cout << "PokedexActivityManager::back()  START" << std::endl;
    PokedexActivity* current = sceneStack.top();
    sceneStack.pop();

    activity = sceneStack.empty() ? nullptr : sceneStack.top();
    if (!activity) {
        std::cout << "activity  empty. Nothing to go back to." << std::endl;
        return;
    }

    current->onDeactivate();

    if (!sceneStack.empty()) {
        std::cout << "!sceneStack.empty(). Calling activity->onActivate()." << std::endl;
        activity->onActivate();
    }
    std::cout << "PokedexActivityManager::back()  END" << std::endl;
}

PokedexActivity* PokedexActivityManager::getActiveState() {
    return activity;
}