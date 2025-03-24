#include "PokedexActivityManager.hpp"
#include "PokedexActivity_PokemonView_Moves.hpp"

void PokedexActivity_PokemonView_Moves::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        if (selectedIndex < offset)
        {
            offset--;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS)
    {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS)
            {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS;
            }
        }
    }
    else
    {
        selectedIndex = dbResults->size() - 1;
        offset        = dbResults->size() - MAX_VISIBLE_ITEMS;
    }
}

void PokedexActivity_PokemonView_Moves::onButtonL(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex >= MAX_VISIBLE_ITEMS)
    {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset)
        {
            offset -= MAX_VISIBLE_ITEMS;
            if (offset < 0)
            {
                offset = 0;
            }
        }
    }
    else
    {
        selectedIndex = 0;
        offset        = 0;
    }
}

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
