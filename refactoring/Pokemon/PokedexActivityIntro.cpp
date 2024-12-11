#include "PokedexActivityIntro.h"
#include "PokedexActivityManager.h"

PokedexActivityIntro PokedexActivityIntro::instance;

PokedexActivityIntro::PokedexActivityIntro() {
    surf_logo = NULL;
    logoAlpha = 0;
}

void PokedexActivityIntro::onActivate() {
    // Load Simple Logo
    std::string file = "res/img/splash.png";
    surf_logo = PokeSurface::onLoadImg(file);

    StartTime = SDL_GetTicks();
    logoAlpha = 0;
}

void PokedexActivityIntro::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityIntro::onLoop() {
    // Get the current time in milliseconds
    Uint32 currentTime = SDL_GetTicks();

    // Calculate elapsed time since last FPS calculation
    Uint32 elapsedTime = currentTime - StartTime;

    // Update logoAlpha value. Should be 255 after 3 seconds. 
    if (elapsedTime <= 3000) {
        logoAlpha = (255 * elapsedTime) / 3000; // Frames per second
    }
    else {
        logoAlpha = 255;
    }

    if (StartTime + 4000 < SDL_GetTicks()) {
        PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
    }
}

void PokedexActivityIntro::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    //SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));

    if (surf_logo) {
        SDL_SetTextureAlphaMod(texture, logoAlpha); // Apply the current alpha for fade-in
        int W = WINDOW_WIDTH;
        int H = WINDOW_HEIGHT;
        PokeSurface::onDraw(surf_display, surf_logo, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

PokedexActivityIntro* PokedexActivityIntro::getInstance() {
    return &instance;
}