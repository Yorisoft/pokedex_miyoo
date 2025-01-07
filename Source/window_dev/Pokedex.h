#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include <SDL.h>
#include <SDL_image.h>                   	
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "PokedexActivityManager.h"
#include "PokedexActivityEvent.h"
#include "PokeSurface.h"

class Pokedex : public PokedexActivityEvent {
private:
	bool running;

    // Variables for FPS calculation
    Uint32 frameCount;
    Uint32 lastTime;
    float fps;

    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Surface* screenTest;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
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

    void calculateFPS(Uint32& frameCount, Uint32& lastTime, float& fps);
};

#endif
