#ifndef __POKEDEXACTIVITY_POKEMONVIEW_MOVES__
#define __POKEDEXACTIVITY_POKEMONVIEW_MOVES__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Moves : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Moves instance;

	const int ITEM_HEIGHT = (static_cast<int>(WINDOW_HEIGHT * 0.7 / 5));
    const SDL_Color 
		COLOR = { 64, 64, 64}, 
		HIGHLIGHT_COLOR = { 255, 0, 0 };

	const std::string 
		SOUND_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav",
		SOUND_UP_DOWN_PATH = "res/assets/sound_effects/up_down.wav",
		FONT_PATH = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf",
		BACKGROUND_IMG_PATH = "res/assets/misc/pokemon_fr_view_3.png",
		TYPE_IMG_BASE_PATH = "res/assets/pokemons/types/",
		METHOD_IMG_BASE_PATH = "res/assets/pokemons/encounters/",
		ICON_IMG_BASE_PATH = "res/assets/pokemons/icons/";

    Pokemon* pokemon;

    int selectedIndex, offset;
	bool needRedraw;

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> move;
    
	std::vector<SDL_Surface*> typeSurface_cache;
	std::vector<SDL_Surface*> nameSurface_cache;
	std::vector<SDL_Surface*> methodSurface_cache;
	std::vector<SDL_Surface*> levelSurface_cache;
	std::vector<SDL_Surface*> ppSurface_cache;
	std::vector<SDL_Surface*> pwrSurface_cache;
	std::vector<SDL_Surface*> classSurface_cache;
	std::vector<SDL_Surface*> accrySurface_cache;
	std::vector<SDL_Surface*> summarySurface_cache;

	SDL_Surface* backgroundSurface, *listEntrySurface,
		*pokeIconSurface, *pokeNameSurface, 
		*typeASurface, *typeBSurface;
	SDL_Rect backgroundRect, listEntryRect, 
			 typeRect, nameRect, methodRect, 
			 levelRect, ppRect, pwrRect, pokeIconRect,
			 pokeNameRect, typeARect, typeBRect,
			 classRect, accryRect, summaryRect;
    Mix_Chunk* se_left_right, *se_up_down;
	TTF_Font *fontSurface;

    PokedexActivity_PokemonView_Moves();
    ~PokedexActivity_PokemonView_Moves();

	bool initSDL();
	void print_dbResults();
	void clearCachedSurfaces();

    bool renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i);
    bool renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i);
    std::string cleanString(std::string str);


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

    static PokedexActivity_PokemonView_Moves* getInstance();
};
#endif
