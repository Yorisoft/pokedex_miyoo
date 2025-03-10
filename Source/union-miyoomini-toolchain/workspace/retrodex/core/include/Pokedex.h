#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include "PokeSurface.h"
#include "PokedexActivityEvent.h"
#include "PokedexActivityManager.h"
#include <AssetManager.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Pokedex : public PokedexActivityEvent
{
  private:
    // Variables for FPS calculation
    Uint32 frameCount;
    Uint32 lastTime;
    float fps;

    // Variables for frame delay calculation
    const float FPS = 60.0f;
    const Uint32 frameDelay = 1000 / FPS;

    bool running, needRedraw, assetsLoaded;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *screen, *fpsSurface;
    TTF_Font *font;
    Mix_Chunk *sEffect;
    SDL_Rect fpsRect;

  public:
    Pokedex();

    int onExecute();
    bool onInit();
    bool onSDLInit();
    void onEvent(SDL_Event *event);
    void onLoop();
    void onRender();
    void onExit();
    void onCleanup();
    void calculateFPS();
    void renderFPS();
};

#endif
