#ifndef __POKEDEX_ASSETS__
#define __POKEDEX_ASSETS__

#include<SDL2/SDL.h>
#include<string>

class PokedexAssets {
public:
	typedef struct asset_t {
		std::string path;
		SDL_Surface* surface = nullptr;
		SDL_Rect size;
		bool isLoaded = false;

		asset_t();
		asset_t(std::string p) : path(p) {}
        asset_t(std::string p, std::pair<int,int> s) : path(p), size({0, 0, s.first, s.second}) {}
        asset_t(std::string p, SDL_Surface* sf, SDL_Rect r, bool loadState) : 
			path(p), surface(sf), size(r), isLoaded(loadState) {}
	} asset;
	
protected:
	const std::string 
		POKEMON_ICONS_PATH = "res/assets/pokemons/icons/",
		POKEMON_SPRITES_PATH = "res/assets/pokemons/sprites/";

public:
	PokedexAssets();

};

#endif
