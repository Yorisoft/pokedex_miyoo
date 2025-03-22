#include "PokedexActivity_PokemonView_Info.h"

PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Info::instance;

PokedexActivity_PokemonView_Info::PokedexActivity_PokemonView_Info()
    : needRedraw(true), dbResults(nullptr), pokemon(nullptr), assetManager(nullptr),
      backgroundSurface(nullptr), pokeSprite(nullptr), typeA(nullptr), typeB(nullptr), id(nullptr),
      nameSurface(nullptr), height(nullptr), weight(nullptr), gender(nullptr), genus(nullptr),
      flavorText(nullptr), se_poke_cry(nullptr), se_left_right(nullptr), fontSurface(nullptr)
{
}

PokedexActivity_PokemonView_Info::~PokedexActivity_PokemonView_Info()
{
    if (se_poke_cry)
        Mix_FreeChunk(se_poke_cry);

    if (se_left_right)
        Mix_FreeChunk(se_left_right);
}

void PokedexActivity_PokemonView_Info::printPokeInfo()
{
    std::vector<double> *genderRates = pokemon->getGenderRates();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Regional ID: " << pokemon->getRegionalID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';
    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "Gender Ratio: " << '\n';
    std::cout << "Female: " << (*genderRates)[0] << '\n';
    std::cout << "Male: " << (*genderRates)[1] << '\n';
}

void PokedexActivity_PokemonView_Info::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Info::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
    printPokeInfo();

    assetManager = AssetManager::getInstance();

    // Initialize SDL components
    if (!initSDL())
    {
        std::cout << "PokedexActivity_PokemonView_Info::initSDL(): SDL Error: " << SDL_GetError()
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    // Play the sound effect
    Mix_PlayChannel(-1, se_poke_cry, 0);

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Info::onActivate END \n";
}

void PokedexActivity_PokemonView_Info::onDeactivate()
{
    delete pokemon;
    pokemon = nullptr;
}

void PokedexActivity_PokemonView_Info::onLoop() {}

void PokedexActivity_PokemonView_Info::onFreeze()
{
    // do nothing for now..
}

PokedexActivity_PokemonView_Info *PokedexActivity_PokemonView_Info::getInstance()
{
    return &instance;
}
