#ifndef __POKEDEXACTIVITYINTRO_H__
#define __POKEDEXACTIVITYINTRO_H__

#include "PokedexActivity.h"

class PokedexActivityIntro : public PokedexActivity {
private:
    static PokedexActivityIntro instance;

    static const std::string userConfigFile;
    SDL_Surface* splashSurface;

    int StartTime;        
    Uint8 logoAlpha;

private:
    PokedexActivityIntro();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    void onFreeze();

public:
    static PokedexActivityIntro* getInstance();
};

#endif
