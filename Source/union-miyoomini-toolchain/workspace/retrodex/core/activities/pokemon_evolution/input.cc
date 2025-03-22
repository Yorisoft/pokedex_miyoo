#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Evolution.h"

void PokedexActivity_PokemonView_Evolution::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex < offset)
        {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < evoChain->size() - 1)
    {
        selectedIndex++;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex - offset >= 3)
        {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Evolution::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonA(SDL_Keycode sym, Uint16 mod)
{
    std::cout << evo[2] << '\n';
    PokedexDB::setPokemonID(std::stoi(evo[1]));
    PokedexDB::setPokemonIdentifier(evo[2]);
    PokedexDB::setVersionID(std::stoi(evo[22]));
    PokedexDB::setVersionGroupID(std::stoi(evo[23]));
    PokedexDB::setGenerationID(std::stoi(evo[24]));

    ////Call next activity
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Evolution::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Evolution::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
