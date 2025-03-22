#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity_PokemonView_Evolution PokedexActivity_PokemonView_Evolution::instance;

PokedexActivity_PokemonView_Evolution::PokedexActivity_PokemonView_Evolution()
    : needRedraw(true), selectedIndex(0), offset(0), pokemon(nullptr), backgroundSurface(nullptr),
      listEntryBackground_default(nullptr), listEntryBackground_selected(nullptr),
      se_up_down(nullptr), se_left_right(nullptr), assetManager(nullptr)
{
}

PokedexActivity_PokemonView_Evolution::~PokedexActivity_PokemonView_Evolution()
{
    if (se_up_down)
        Mix_FreeChunk(se_up_down);

    if (se_left_right)
        Mix_FreeChunk(se_left_right);
}

void PokedexActivity_PokemonView_Evolution::printPokeInfo()
{
    std::vector<unsigned short> stats = pokemon->getBasicStats();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';
    std::cout << "Evolution Chain ID: " << pokemon->getEvolutionChainID() << '\n';
    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Deffense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';

    for (std::vector<std::string> r : *pokemon->getEvolutionChain())
    {
        for (auto &c : r)
        {
            std::cout << c << '|';
        }
        std::cout << std::endl;
    }
}

void PokedexActivity_PokemonView_Evolution::onDeactivate()
{
    std::cout << "PokedexActivity_PokemonView_Evolution::onDeactive START \n";

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Evolution::onDeactive END \n";
}

void PokedexActivity_PokemonView_Evolution::onLoop() {}

void PokedexActivity_PokemonView_Evolution::onFreeze() {}

PokedexActivity_PokemonView_Evolution *PokedexActivity_PokemonView_Evolution::getInstance()
{
    return &instance;
}
