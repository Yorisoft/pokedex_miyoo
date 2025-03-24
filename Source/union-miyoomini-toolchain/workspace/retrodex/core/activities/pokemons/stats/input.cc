#include "PokedexActivityManager.hpp"
#include "PokedexActivity_PokemonView_Stats.hpp"

void PokedexActivity_PokemonView_Stats::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
    // PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Stats::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Stats::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
