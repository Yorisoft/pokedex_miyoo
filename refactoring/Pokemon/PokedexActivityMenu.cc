#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() {
    surf_logo = NULL;
}

void PokedexActivityMenu::onActivate() {
}

void PokedexActivityMenu::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityMenu::onLoop() {
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    //SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));

    if (surf_logo) {
        int W = WINDOW_WIDTH;
        int H = WINDOW_HEIGHT;
        PokeSurface::onDraw(surf_display, surf_logo, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}