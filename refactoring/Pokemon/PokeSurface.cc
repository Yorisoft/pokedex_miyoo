#include "PokeSurface.h"

PokeSurface::PokeSurface() {
}

SDL_Surface* PokeSurface::onLoadImg(std::string& file) {
    SDL_Surface* tempSurface = NULL;
    SDL_Surface* optimizedSurface = NULL;

    std::cout << "starting: LoadImage" << std::endl;
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
    std::cout << "ending: LoadImage" << std::endl;

    return optimizedSurface;
}

SDL_Surface* PokeSurface::onLoadBMP(std::string& file) {
    SDL_Surface* tempSurface = NULL;
    SDL_Surface* optimizedSurface = NULL;

    std::cout << "starting: LoadImage" << std::endl;
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
    std::cout << "ending: LoadImage" << std::endl;

    return optimizedSurface;
}

bool PokeSurface::onDraw(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, int X, int Y, int W, int H) {
    if (destinationSurface == NULL || sourceSurface == NULL) {
        std::cout << "Missing Source or Desitination Surface! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Rect destinationRect;
    destinationRect.x = X;
    destinationRect.y = Y;
    destinationRect.w = W;
    destinationRect.h = H;

    SDL_BlitSurface(sourceSurface, NULL, destinationSurface, &destinationRect);

    return true;
}

bool PokeSurface::onDrawScaled(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, int X, int Y, int W, int H) {
    if (destinationSurface == NULL || sourceSurface == NULL) {
        return false;
    }

    SDL_Rect sourceRect;
    sourceRect.x = X;
    sourceRect.y = Y;
    sourceRect.w = sourceSurface->w * W;
    sourceRect.h = sourceSurface->h * H;

    SDL_BlitScaled(sourceSurface, NULL, destinationSurface, &sourceRect);

    return true;
}