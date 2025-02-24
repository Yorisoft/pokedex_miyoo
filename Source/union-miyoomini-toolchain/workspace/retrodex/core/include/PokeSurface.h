#ifndef _POKESURFACE_H_
#define _POKESURFACE_H_

#include<iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 

class PokeSurface {
public:
    PokeSurface();

public:
    static SDL_Surface* onLoadImg(std::string& file);

    static SDL_Surface* onLoadImg(const std::string& file);

    static SDL_Surface* onLoadBMP(std::string& file);

    static SDL_Surface* onLoadBMP(const std::string& file);

    static bool onDraw(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect);
    
    static bool onDrawScaled(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect);
};

#endif
