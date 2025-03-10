#ifndef __POKEDEXACTIVITYINTRO_H__
#define __POKEDEXACTIVITYINTRO_H__

#include "PokedexActivity.h"
#include "AssetManager.h"

class PokedexActivityIntro : public PokedexActivity {
private:
    static PokedexActivityIntro instance;

    int StartTime;        
    Uint8 logoAlpha;

    const std::string 
		SPLASH_ART_PATH = "res/assets/splash/splash.png",
		FONT_PATH = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf";

    const SDL_Color 
		COLOR = { 248, 248, 248 }, 
		HIGHLIGHT_COLOR = { 255, 0, 0 };


    static const std::string userConfigFile;
    std::map<std::string, int> userSettingMap;

    SDL_Surface* splashSurface, *fileSurface;
	AssetManager* assetManager;
	
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
