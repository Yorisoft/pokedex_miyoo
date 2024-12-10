#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList() {
    surf_logo = NULL;
}

void PokedexActivityList::onActivate() {
    

    StartTime = SDL_GetTicks();
}

void PokedexActivityList::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityList::onLoop() {
    // Load Simple Logo
    std::string file = "res/icons/abra.png";

    surf_logo = PokeSurface::onLoadImg(file);
    //if (StartTime + 3000 < SDL_GetTicks()) {
    //    PokedexActivityManager::setActiveState(APPSTATE_MENU);
    //}
}

void PokedexActivityList::onRender(SDL_Surface* surf_display) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));

    if (surf_logo) {
        PokeSurface::onDrawScaled(surf_display, surf_logo, 0, 0, 3, 3);
    }
    SDL_FreeSurface(surf_logo);
}

PokedexActivityList* PokedexActivityList::getInstance() {
    return &instance;
}