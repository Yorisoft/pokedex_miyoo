#ifndef _POKESURFACE_H_
#define _POKESURFACE_H_

#include<iostream>
#include <SDL.h>
#include <SDL_image.h>                   	

class PokeSurface {
public:
    PokeSurface();

public:
    static SDL_Surface* onLoad(std::string& file);

    static bool onDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
};

#endif
