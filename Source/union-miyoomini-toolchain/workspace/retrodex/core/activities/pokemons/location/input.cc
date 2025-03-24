#include "PokedexActivityManager.hpp"
#include "PokedexActivity_PokemonView_Location.hpp"

void PokedexActivity_PokemonView_Location::onButtonUp(SDL_Keycode sym, Uint16 mod)
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

void PokedexActivity_PokemonView_Location::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < routes->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Location::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Location::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_EVOLUTION);
}

void PokedexActivity_PokemonView_Location::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Location::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Location::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < routes->size() - MAX_VISIBLE_ITEMS)
    {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > routes->size() - MAX_VISIBLE_ITEMS)
            {
                offset = routes->size() - MAX_VISIBLE_ITEMS;
            }
        }
    }
    else
    {
        selectedIndex = routes->size() - 1;
        offset        = routes->size() - MAX_VISIBLE_ITEMS;
    }
}

void PokedexActivity_PokemonView_Location::onButtonL(SDL_Keycode sym, Uint16 mod)
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

void PokedexActivity_PokemonView_Location::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Location::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
