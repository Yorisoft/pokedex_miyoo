#ifndef __POKEDEXACTIVITY_POKEMONVIEW_MOVES__
#define __POKEDEXACTIVITY_POKEMONVIEW_MOVES__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Moves : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Moves instance;

    int selectedIndex, offset, itemHeight;
	bool needRedraw;

	const std::string 
		SOUND_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav",
		SOUND_UP_DOWN_PATH = "res/assets/sound_effects/up_down.wav",
		BACKGROUND_IMG_PATH = "res/assets/misc/pokemon_fr_view_3.png",
		TYPE_IMG_BASE_PATH = "res/assets/pokemons/types/",
		METHOD_IMG_BASE_PATH = "res/assets/pokemons/encounters/",
		ICON_IMG_BASE_PATH = "res/assets/pokemons/icons/";

    std::vector<std::vector<std::string>>* dbResults;
    std::vector<std::string> move;
    
    Pokemon* pokemon;

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
    Mix_Chunk* sound_left_right, *sound_up_down;
    SDL_Color color, highlightColor;


    PokedexActivity_PokemonView_Moves();

	bool initSDL();
	void printPokeInfo();

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
