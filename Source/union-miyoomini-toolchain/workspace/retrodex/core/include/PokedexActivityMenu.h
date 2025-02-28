#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

	const int 
		ITEM_HEIGHT = (static_cast<int>(WINDOW_HEIGHT / 5));

	const std::string 
		SOUND_UP_DOWN_PATH = "res/assets/sound_effects/up_down.wav",
		FONT_PATH = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf",
		BACKGROUND_IMG_PATH = "res/assets/misc/menu_background.png",
		LIST_BACKGROUND_IMG_PATH_DEFAULT = "res/assets/misc/menu_item_background_default.png",
		LIST_BACKGROUND_IMG_PATH_SELECTED = "res/assets/misc/menu_item_background_selected.png";

    const SDL_Color 
		COLOR = { 248, 248, 248 },
		HIGHLIGHT_COLOR = { 255, 0, 0 };

    int selectedIndex, offset;
	bool needRedraw, needInit;

    std::vector<std::string> game;
    std::vector<std::vector<std::string>>* dbResults;
	
	std::vector<SDL_Surface*> cachedTextSurfaces;
	std::vector<SDL_Surface*> cachedHighlightTextSurfaces;

	SDL_Surface* backgroundSurface, *listEntrySurface_default, 
		*listEntrySurface_selected;
    SDL_Rect backgroundRect, listEntryRect, gameVersionRect;
    TTF_Font* fontSurface;
    Mix_Chunk* sound_up_down;

	bool initSDL();
	void printMenuInfo();
	void clearCacheSurfaces();
    bool renderListItems(SDL_Surface* surf_display, int i);

    void onButtonUp(SDL_Keycode, Uint16);
    void onButtonDown(SDL_Keycode, Uint16);
    void onButtonLeft(SDL_Keycode, Uint16);
    void onButtonRight(SDL_Keycode, Uint16);
    void onButtonA(SDL_Keycode, Uint16);
    void onButtonB(SDL_Keycode, Uint16);
    void onButtonR(SDL_Keycode, Uint16);
    void onButtonL(SDL_Keycode, Uint16);
    void onButtonSelect(SDL_Keycode, Uint16);
    void onButtonStart(SDL_Keycode, Uint16);

public:
    PokedexActivityMenu();
    ~PokedexActivityMenu();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();
    static PokedexActivityMenu* getInstance();
};

#endif
