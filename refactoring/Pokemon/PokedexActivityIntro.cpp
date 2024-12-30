#include "PokedexActivityIntro.h"
#include "PokedexActivityManager.h"

PokedexActivityIntro PokedexActivityIntro::instance;

PokedexActivityIntro::PokedexActivityIntro() {
    splashSurface = NULL;
    logoAlpha = 0;
}

void PokedexActivityIntro::onActivate() {
    // Load Simple Logo
    std::string file = "res/img/splash.png";
    splashSurface = PokeSurface::onLoadImg(file);

    StartTime = SDL_GetTicks();
    logoAlpha = 0;
}

void PokedexActivityIntro::onDeactivate() {
    if (splashSurface) {
        SDL_FreeSurface(splashSurface);
        splashSurface = NULL;
    }
}

void PokedexActivityIntro::onLoop() {
    // Get the current time in milliseconds
    Uint32 currentTime = SDL_GetTicks();

    // Calculate elapsed time since last FPS calculation
    Uint32 elapsedTime = currentTime - StartTime;

    // Update logoAlpha value. Should be 255 after 3 seconds. 
    if (elapsedTime <= 3000) {
        logoAlpha = (255 * elapsedTime) / 3000; 
    }
    else {
        logoAlpha = 255;
    }

    if (static_cast<Uint32>(StartTime + 4000) < SDL_GetTicks()) {
        // call next activity
        PokedexActivityManager::push(APPSTATE_POKEDEX_MENU);
    }
}

void PokedexActivityIntro::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    if (splashSurface) {
        SDL_SetTextureAlphaMod(texture, logoAlpha); // Apply the current alpha for fade-in
        SDL_Rect splashScreenRect = {
            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT
        };
        PokeSurface::onDraw(surf_display, splashSurface, &splashScreenRect);
    }
}

void PokedexActivityIntro::onFreeze() {
// do nothing for now..
}

PokedexActivityIntro* PokedexActivityIntro::getInstance() {
    return &instance;
}