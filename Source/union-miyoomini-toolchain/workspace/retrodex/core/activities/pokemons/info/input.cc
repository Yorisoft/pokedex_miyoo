#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Info.h"

void PokedexActivity_PokemonView_Info::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Info::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
