#ifndef __POKEDEXACTIVITY_POKEMONVIEW_STATS__
#define __POKEDEXACTIVITY_POKEMONVIEW_STATS__

#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Stats : public PokedexActivity {
private:
    static PokedexActivity_PokemonView_Stats instance;

	const std::string
		FONT_PATH = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf",
		SOUND_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav",
		BACKGROUND_IMG_PATH = "res/assets/misc/pokemon_fr_view_2.png",
		SPRITE_IMG_BASE_PATH = "res/assets/pokemons/sprites/";

	bool needRedraw;

    std::vector<std::vector<std::string>>* dbResults;

    Pokemon* pokemon;

	std::vector<std::vector<std::string>>* statNames;
	std::vector<SDL_Surface*> statsNameSurface_cache;
	std::vector<SDL_Surface*> statsSurface_cache;

	SDL_Surface* backgroundSurface, *iconSurface,
		*idSurface, *nameSurface,
		*abilitySurface, *h_abilitySurface;
	SDL_Rect backgroundRect, iconRect,
			 idRect, nameRect,
			 statsNameRect, statsRect,
			 abilityRect, h_abilityRect;
    Mix_Chunk* se_left_right;
	TTF_Font* fontSurface;

private:
    PokedexActivity_PokemonView_Stats();
    ~PokedexActivity_PokemonView_Stats();

	void printPokeInfo();
	bool initSDL();
	void clearCachedSurfaces();

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

    static PokedexActivity_PokemonView_Stats* getInstance();
};
#endif