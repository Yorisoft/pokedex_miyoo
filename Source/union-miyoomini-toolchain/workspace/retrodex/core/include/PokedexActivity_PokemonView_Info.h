#ifndef __POKEDEXACTIVITY_POKEMONVIEW_INFO__
#define __POKEDEXACTIVITY_POKEMONVIEW_INFO__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Info : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Info instance; 

	const std::string BACKGROUND_IMG_PATH = "res/assets/misc/pokemon_fr_view_1.png";
	const std::string SPRITES_IMG_BASE_PATH = "res/assets/pokemons/sprites/";
	const std::string TYPES_IMG_BASE_PATH = "res/assets/pokemons/types/";

	bool needRedraw;

    std::vector<std::vector<std::string>>* dbResults;

    Pokemon* pokemon;

	SDL_Surface* backgroundSurface, *pokeIconSurface,
		*pokeType1Surface, *pokeType2Surface,
		*pokeIDSurface, *pokeNameSurface,
		*pokeHeightSurface, *pokeWeightSurface,
		*pokeGenderSurface, *pokeGenusSurface,
		*flavorTextSurface;
	SDL_Rect backgroundRect, pokeIconRect,
			 pokeType1Rect, pokeType2Rect,
			 pokeIDRect, pokeNameRect, 
			 heightRect, weightRect,
			 genderRect, genusRect,
			 fTextRect;
	Mix_Chunk* pokeCry;
private:
    PokedexActivity_PokemonView_Info();
	void printPokeInfo();
	bool initSDL();
    bool renderSprites(SDL_Surface* surf_display);
    bool renderNameID(SDL_Surface* surf_display, TTF_Font* font);
    bool renderHW(SDL_Surface* surf_display, TTF_Font* font);
    bool renderFlavorText(SDL_Surface* surf_display, TTF_Font* font);

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

    static PokedexActivity_PokemonView_Info* getInstance();
};
#endif
