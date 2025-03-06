#ifndef __POKEDEXACTIVITY_POKEMONVIEW_EVOLUTION__ 
#define __POKEDEXACTIVITY_POKEMONVIEW_EVOLUTION__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Evolution : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Evolution instance;

    Pokemon* pokemon;

	const int ITEM_HEIGHT = static_cast<int>((WINDOW_HEIGHT / 3) * 0.7);

	const std::string 
		SOUND_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav",
		SOUND_UP_DOWN_PATH = "res/assets/sound_effects/up_down.wav",
		BACKGROUND_IMG_PATH = "res/assets/misc/pokemon_fr_view_5.png",
		FONT_PATH ="res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 
		ENTRY_BACKGROUND_IMAGE_PATH_DEFAULT = "res/assets/misc/evolution_item_background_default.png",
		ENTRY_BACKGROUND_IMAGE_PATH_SELECTED = "res/assets/misc/evolution_item_background_selected.png",
		SPRITE_IMG_BASE_PATH = "res/assets/pokemons/sprites/",
		TYPE_IMG_BASE_PATH = "res/assets/pokemons/types/",
		METHOD_IMG_BASE_PATH = "res/assets/pokemons/encounters/",
		ICON_IMG_BASE_PATH = "res/assets/pokemons/icons/";

    const SDL_Color 
		COLOR = { 64, 64, 64 },
		HIGHLIGHT_COLOR = { 255, 0, 0 };

    int selectedIndex, offset;

	bool needRedraw;

    std::string fontPath;

    std::vector<std::string> evo;
    std::vector<std::vector<std::string>>* evoChain;

	std::vector<SDL_Surface*> pokeSprite_cache;
	std::vector<SDL_Surface*> idSurface_cache;
	std::vector<SDL_Surface*> nameSurface_cache;
	std::vector<SDL_Surface*> methodSurface_cache;
	std::vector<SDL_Surface*> methodInfoSurface_cache;


	SDL_Surface* backgroundSurface, 
		*listEntrySurface_default, *listEntrySurface_selected;
	SDL_Rect backgroundRect, listEntryRect,
			 pokeRect, idRect, nameRect,
			 methodRect, methodInfoRect;
    Mix_Chunk* se_left_right, * se_up_down;
	TTF_Font* fontSurface;

	bool initSDL();
    void printPokeInfo();
	void clearCachedSurfaces();

    bool renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font, int i);
    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);

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
    PokedexActivity_PokemonView_Evolution();
    ~PokedexActivity_PokemonView_Evolution();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();

    static PokedexActivity_PokemonView_Evolution* getInstance();
};
#endif