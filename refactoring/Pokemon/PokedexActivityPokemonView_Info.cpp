#include "PokedexActivityPokemonView_Info.h"
#include "PokedexActivityManager.h"

PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Info::instance;

PokedexActivity_PokemonView_Info::PokedexActivity_PokemonView_Info() : 
pokeIDSurface(nullptr),
pokeNameSurface(nullptr),
pokeIconSurface(nullptr),
pokeType1Surface(nullptr),
pokeType2Surface(nullptr),
backgroundSurface(nullptr),
dbResults(nullptr)
{

}

void PokedexActivity_PokemonView_Info::onActivate() {
    // Load Simple Logo
}

void PokedexActivity_PokemonView_Info::onDeactivate() {
}

void PokedexActivity_PokemonView_Info::onLoop() {
}

void PokedexActivity_PokemonView_Info::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
}

void PokedexActivity_PokemonView_Info::onFreeze() {
// do nothing for now..
}

PokedexActivity_PokemonView_Info* PokedexActivity_PokemonView_Info::getInstance() {
    return &instance;
}
