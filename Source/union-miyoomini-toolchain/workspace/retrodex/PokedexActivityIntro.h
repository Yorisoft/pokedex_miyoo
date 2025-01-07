#ifndef __POKEDEXACTIVITYINTRO_H__
#define __POKEDEXACTIVITYINTRO_H__

#include "PokedexActivity.h"

class PokedexActivityIntro : public PokedexActivity {
private:
    static PokedexActivityIntro instance;

    SDL_Surface* splashSurface;

    int StartTime;        
    Uint8 logoAlpha;

    static const std::string userConfigFile;
    std::map<std::string, int> userSettingMap;

private:
    PokedexActivityIntro();
    void loadUserConfig(const std::string&);

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);

    void onFreeze();

public:
    static PokedexActivityIntro* getInstance();
};

#endif
