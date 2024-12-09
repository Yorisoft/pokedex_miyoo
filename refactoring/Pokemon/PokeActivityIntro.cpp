#include "PokeActivityIntro.h"

#include "PokedexActivityManager.h"

PokeActivityIntro PokeActivityIntro::instance;

PokeActivityIntro::PokeActivityIntro() {
    surf_logo = nullptr;
}

void PokeActivityIntro::OnActivate() {
    // Load Simple Logo
    surf_logo = ActivitySurface::OnLoad("res/img/splash.png");

    StartTime = SDL_GetTicks();
}

void PokeActivityIntro::OnDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = nullptr;
    }
}

void PokeActivityIntro::OnLoop() {
    if (StartTime + 3000 < SDL_GetTicks()) {
        PokedexActivityManager::setActiveState(APPSTATE_MENU);
    }
}

void PokeActivityIntro::OnRender(SDL_Surface* surf_display) {
    if (surf_logo) {
        ActivitySurface::OnDraw(surf_display, surf_logo, 0, 0);
    }
}

PokeActivityIntro* PokeActivityIntro::GetInstance() {
    return &instance;
}