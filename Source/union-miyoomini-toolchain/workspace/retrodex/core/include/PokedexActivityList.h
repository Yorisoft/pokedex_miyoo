#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

	const std::string 
		BACKGROUND_IMG_PATH = "res/assets/misc/pokedexList_background.png",
		LIST_BACKGROUND_IMG_PATH_DEFAULT = "res/assets/misc/menu_item_background_default.png",
		LIST_BACKGROUND_IMG_PATH_SELECTED = "res/assets/misc/menu_item_background_selected.png";

    int selectedIndex, offset, itemHeight;
	bool needRedraw;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> pokemon;

	std::vector<SDL_Surface*> pokemonIconSurface_cache;
	std::vector<std::vector<SDL_Surface*>> pokemonTypeSurface_cache;
	std::vector<std::vector<SDL_Surface*>> pokemonIDNameSurface_cache;

	SDL_Surface* backgroundSurface, *listBackgroundSurface_default, 
		*listBackgroundSurface_selected;
    SDL_Rect backgroundRect, listBackgroundRect,
			 pokeEntryRect, pokeEntryType1Rect, 
			 pokeEntryType2Rect, pokeIDRect, pokeNameRect;
    Mix_Chunk* sEffect, *sEffect_OnStart, *sEffect_OnExit;
    SDL_Color color, highlightColor;

    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);
    SDL_Rect renderItemBackground(SDL_Surface* surf_display, int i);
    bool renderItemSprites(SDL_Surface* surf_display, int i);
    bool renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i);

private:
    PokedexActivityList();
    ~PokedexActivityList();

	void initSDL();

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
    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();
    static PokedexActivityList* getInstance();

};
#endif