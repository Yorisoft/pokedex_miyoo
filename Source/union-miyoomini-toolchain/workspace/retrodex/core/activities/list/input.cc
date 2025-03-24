#include "PokedexActivityList.hpp"
#include "PokedexActivityManager.hpp"

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        if (selectedIndex < offset)
        {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonA(SDL_Keycode sym, Uint16 mod)
{
    ////Set pokemon identifier for PokedexDB
    PokedexDB::setPokemonID(std::stoi(pokemon[0]));
    PokedexDB::setPokemonIdentifier(pokemon[1]);

    ////Call next activity
    PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivityList::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(-1, se_on_exit, 0);
    // need to find a way to call Mix_CloseChunk() without stopping the audio immediately

    ////Set pokemon identifier for PokedexDB
    PokedexActivityManager::back();
}

void PokedexActivityList::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS)
    {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += MAX_VISIBLE_ITEMS;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS)
            {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately
    }
    else
    {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset        = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityList::onButtonL(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex >= MAX_VISIBLE_ITEMS)
    {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset)
        {
            offset -= MAX_VISIBLE_ITEMS; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0)
            {
                offset = 0; // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately
    }
    else
    {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset        = 0; // Cap offset to zero
    }
}

void PokedexActivityList::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
