#ifndef _POKESURFACE_H_
#define _POKESURFACE_H_

#include<iostream>
#include <SDL.h>
#include <SDL_image.h> 

class PokeSurface {
public:
    PokeSurface();

public:
    static SDL_Surface* onLoadImg(std::string& file);

    static SDL_Surface* onLoadBMP(std::string& file);

    static bool onDraw(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect);
    
    static bool onDrawScaled(SDL_Surface* destinationSurface, SDL_Surface* sourceSurface, SDL_Rect* rect);
};

#endif
