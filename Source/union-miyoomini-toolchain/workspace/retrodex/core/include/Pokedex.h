#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>                   	
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "PokedexActivityManager.h"
#include "PokedexActivityEvent.h"
#include "PokeSurface.h"

class Pokedex : public PokedexActivityEvent {
private:
    // Variables for FPS calculation
    Uint32 frameCount;
    Uint32 lastTime;
    float fps;

	bool running;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface* screen;
    TTF_Font* font;
    Mix_Chunk* sEffect;

public:
    Pokedex();

    int onExecute();
    bool onInit();
    bool onSDLInit();
    void onEvent(SDL_Event* event);
    void onLoop();
    void onRender();
    void onExit();
    void onCleanup();
    void calculateFPS();
	void renderFPS();
};

#endif
