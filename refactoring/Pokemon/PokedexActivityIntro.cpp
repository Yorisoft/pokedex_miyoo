#include "PokedexActivityIntro.h"
#include "PokedexActivityManager.h"

PokedexActivityIntro PokedexActivityIntro::instance;

PokedexActivityIntro::PokedexActivityIntro() {
    surf_logo = NULL;
}

void PokedexActivityIntro::onActivate() {
    // Load Simple Logo
    std::string file = "res/img/splash.png";
    surf_logo = PokeSurface::onLoadImg(file);

    StartTime = SDL_GetTicks();
}

void PokedexActivityIntro::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityIntro::onLoop() {
    if (StartTime + 3000 < SDL_GetTicks()) {
        PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
    }
}

void PokedexActivityIntro::onRender(SDL_Surface* surf_display) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));

    if (surf_logo) {
        PokeSurface::onDraw(surf_display, surf_logo, 0, 0);
    }
}

PokedexActivityIntro* PokedexActivityIntro::getInstance() {
    return &instance;
}