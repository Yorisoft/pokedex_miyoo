#ifndef __POKEDEXACTIVITYLIST_H__
#define __POKEDEXACTIVITYLIST_H__

#include "PokedexActivity.h"

class PokedexActivityList : public PokedexActivity {
private:
    static PokedexActivityList instance;

    const int ITEM_HEIGHT = static_cast<int>(WINDOW_HEIGHT * 0.6 / 5);

	const std::string 
		SOUND_EFFECT_ON_START_PATH = "res/assets/sound_effects/list_start.wav",
		SOUND_EFFECT_ON_EXIT_PATH = "res/assets/sound_effects/list_back.wav",
		SOUND_EFFECT_UP_DOWN = "res/assets/sound_effects/up_down.wav",
		BACKGROUND_IMG_PATH = "res/assets/misc/pokedexList_background.png",
		SPRITES_IMG_BASE_PATH = "res/assets/pokemons/sprites/",
		TYPES_IMG_BASE_PATH = "res/assets/pokemons/types/",
		LIST_BACKGROUND_IMG_PATH_DEFAULT = "res/assets/misc/menu_item_background_default.png",
		LIST_BACKGROUND_IMG_PATH_SELECTED = "res/assets/misc/menu_item_background_selected.png";
	
    const SDL_Color 
		COLOR = { 248, 248, 248 }, 
		HIGHLIGHT_COLOR = { 255, 0, 0 };

    int selectedIndex, offset;
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
    Mix_Chunk* se_up_down, *se_on_start, *se_on_exit;
	TTF_Font* fontSurface;

private:
    PokedexActivityList();
    ~PokedexActivityList();

	void print_dbResults();
	bool initSDL();
	void clearCacheSurfaces();

    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);
    SDL_Rect renderItemBackground(SDL_Surface* surf_display, int i);
    bool renderItemSprites(SDL_Surface* surf_display, int i);
    bool renderItemEntry(SDL_Surface* surf_display, SDL_Rect* rect, TTF_Font* font, int i);

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