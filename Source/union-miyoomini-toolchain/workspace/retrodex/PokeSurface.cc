#include "PokeSurface.h"

PokeSurface::PokeSurface() {
}

SDL_Surface* PokeSurface::onLoadImg(std::string& file) {
    SDL_Surface* tempSurface = NULL;
    SDL_Surface* optimizedSurface = NULL;

    tempSurface = IMG_Load(file.c_str());
    if (tempSurface == NULL) {
        std::cout << "Unable to load image! SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    optimizedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(tempSurface);
    if (optimizedSurface == NULL) {
        std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    return optimizedSurface;
}

SDL_Surface* PokeSurface::onLoadBMP(std::string& file) {
    SDL_Surface* tempSurface = NULL;
    SDL_Surface* optimizedSurface = NULL;

    tempSurface = SDL_LoadBMP(file.c_str());
    if (tempSurface == NULL) {
        std::cout << "Unable to load image! SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    optimizedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(tempSurface);
    if (optimizedSurface == NULL) {
        std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    return optimizedSurface;
}

bool PokeSurface::onDraw(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect) {
    if (destinationSurface == NULL || sourceSurface == NULL) {
        std::cout << "Missing Source or Desitination Surface! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_BlitSurface(sourceSurface, NULL, destinationSurface, rect);

    return true;
}

bool PokeSurface::onDrawScaled(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect) {
    if (destinationSurface == NULL || sourceSurface == NULL) {
        return false;
    }
    SDL_BlitScaled(sourceSurface, NULL, destinationSurface, rect);

    return true;
}