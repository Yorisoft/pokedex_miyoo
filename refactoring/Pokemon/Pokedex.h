#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include <SDL.h>
#include <SDL_image.h>                   	
#include <SDL_ttf.h>

class Pokedex {
private:
	bool running;

    SDL_Window* window = NULL;
    SDL_Surface* screen = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

public:

    Pokedex();

    int onExecute();

    bool onInit();

    void onEvent(SDL_Event* event);

    void onLoop();

    void onRender();

    void onCleanup();
};

#endif
