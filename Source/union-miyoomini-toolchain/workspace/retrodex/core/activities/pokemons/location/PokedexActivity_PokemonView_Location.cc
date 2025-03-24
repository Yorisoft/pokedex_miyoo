#include "PokedexActivity_PokemonView_Location.hpp"
#include "PokedexActivityManager.hpp"
#include "name_to_id.hpp"

PokedexActivity_PokemonView_Location PokedexActivity_PokemonView_Location::instance;

PokedexActivity_PokemonView_Location::PokedexActivity_PokemonView_Location()
    : selectedIndex(0), offset(0), needRedraw(true), dbResults(nullptr), routes(nullptr),
      backgroundSurface(nullptr), listEntrySurface(nullptr), pokeIcon(nullptr), pokeName(nullptr),
      typeA(nullptr), typeB(nullptr), se_left_right(nullptr), se_up_down(nullptr), pokemon(nullptr),
      assetManager(nullptr)
{
}

PokedexActivity_PokemonView_Location::~PokedexActivity_PokemonView_Location()
{
    if (se_up_down)
        Mix_FreeChunk(se_up_down);

    if (se_left_right)
        Mix_FreeChunk(se_left_right);
}

void PokedexActivity_PokemonView_Location::printPokeInfo()
{
    std::vector<unsigned short> stats = pokemon->getBasicStats();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Deffense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';

    for (std::vector<std::string> route : *routes)
    {
        for (std::string c : route)
        {
            std::cout << c << '|';
        }
        std::cout << '\n';
    }
    std::cout << "Done printing routes \n ";
}

void PokedexActivity_PokemonView_Location::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

    pokemon = new Pokemon();
    routes  = pokemon->getRoutes();
    printPokeInfo();

    // Cant call this here?
    // route = (*routes)[selectedIndex];

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cerr
            << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";
}

void PokedexActivity_PokemonView_Location::onDeactivate()
{
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

    delete pokemon;
    pokemon = nullptr;

    // pointer comes from PokedexDB
    // PokedexDB handles its own pointers
    /* /1* delete routes; *1/ */
    routes = nullptr;

    selectedIndex = 0, offset = 0;

    route.clear();

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";
}

void PokedexActivity_PokemonView_Location::onLoop() {}

void PokedexActivity_PokemonView_Location::onFreeze() {}

PokedexActivity_PokemonView_Location *PokedexActivity_PokemonView_Location::getInstance()
{
    return &instance;
}