#include "PokedexActivity_PokemonView_Stats.hpp"

PokedexActivity_PokemonView_Stats PokedexActivity_PokemonView_Stats::instance;

PokedexActivity_PokemonView_Stats::PokedexActivity_PokemonView_Stats()
    : pokemon(nullptr), dbResults(nullptr), backgroundSurface(nullptr), pokeSprite(nullptr),
      pokeID(nullptr), pokeName(nullptr), abilitySurface(nullptr), h_abilitySurface(nullptr),
      se_left_right(nullptr), needRedraw(true) {};

PokedexActivity_PokemonView_Stats::~PokedexActivity_PokemonView_Stats()
{
    // in order to play sounds asynchrounously with activity,
    // we cant call Mix_FreeChunk immediately after playing.. should probably make seperate class
    // for this.
    if (se_left_right)
        Mix_FreeChunk(se_left_right);
    se_left_right = nullptr;
}

void PokedexActivity_PokemonView_Stats::printPokeInfo()
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
    std::cout << "Defense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';
}

void PokedexActivity_PokemonView_Stats::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Stats::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
    printPokeInfo();

    SQLStatementStore *sql = SQLStatementStore::getInstance();
    std::string query      = sql->getQuery("get_stat_names");
    statNames              = PokedexDB::executeSQL(&query);

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cout
            << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Stats::onActivate END \n";
}

void PokedexActivity_PokemonView_Stats::onDeactivate()
{
    delete pokemon;
    pokemon = nullptr;
}

void PokedexActivity_PokemonView_Stats::onLoop() {}

void PokedexActivity_PokemonView_Stats::onFreeze() {}

PokedexActivity_PokemonView_Stats *PokedexActivity_PokemonView_Stats::getInstance()
{
    return &instance;
}
