#include "PokedexActivity_PokemonView_Moves.hpp"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves()
    : dbResults(nullptr), pokemon(nullptr), needRedraw(true), backgroundSurface(nullptr),
      listEntrySurface(nullptr), pokeIcon(nullptr), pokeName(nullptr), typeA(nullptr),
      typeB(nullptr), se_left_right(nullptr), se_up_down(nullptr), fontSurface(nullptr),
      selectedIndex(0), offset(0)
{
}

PokedexActivity_PokemonView_Moves::~PokedexActivity_PokemonView_Moves()
{
    if (se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;

    if (se_left_right)
        Mix_FreeChunk(se_left_right);
    se_left_right = nullptr;
}

void PokedexActivity_PokemonView_Moves::print_dbResults()
{
    for (std::vector<std::string> &moves : *dbResults)
    {
        for (auto &col : moves)
        {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivity_PokemonView_Moves::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    pokemon = new Pokemon();

    SQLStatementStore *sql = SQLStatementStore::getInstance();
    std::string query      = sql->getQuery("get_pokemon_moves_detailed");

    dbResults = PokedexDB::executeSQL(&query);
    print_dbResults();

    move = (*dbResults)[selectedIndex];

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cout
            << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate()
{
    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate START \n";

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {}

std::string PokedexActivity_PokemonView_Moves::cleanString(std::string str)
{
    std::string replacement = " ";
    std::string cleanStr    = str;
    size_t pos              = 0;
    while ((pos = cleanStr.find("\n", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    pos = 0;
    while ((pos = cleanStr.find("\r", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    pos = 0;
    while ((pos = cleanStr.find("\f", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    return cleanStr;
}

void PokedexActivity_PokemonView_Moves::onFreeze()
{
    // do thing for now..
}

PokedexActivity_PokemonView_Moves *PokedexActivity_PokemonView_Moves::getInstance()
{
    return &instance;
}