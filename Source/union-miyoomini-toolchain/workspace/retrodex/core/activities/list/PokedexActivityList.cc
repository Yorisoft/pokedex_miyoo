#include "PokedexActivityList.hpp"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList()
    : selectedIndex(0), offset(0), needRedraw(true), dbResults(nullptr), backgroundSurface(nullptr),
      listBackgroundSurface_default(nullptr), listBackgroundSurface_selected(nullptr),
      se_up_down(nullptr), se_on_start(nullptr), se_on_exit(nullptr), assetManager(nullptr)
{
}

PokedexActivityList::~PokedexActivityList()
{
    // Cant delete in onDeactivate or auido will be cut short
    //  deleting here is not best practice, this will only ever be called once,
    //  onActivate/Deactivate will be called many times.
    if (se_on_start)
        Mix_FreeChunk(se_on_start);
    se_on_start = nullptr;

    if (se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;

    if (se_on_exit)
        Mix_FreeChunk(se_on_exit);
    se_on_exit = nullptr;
}

void PokedexActivityList::print_dbResults()
{
    for (auto &row : *dbResults)
    {
        for (auto &col : row)
        {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivityList::onActivate()
{
    std::cout << "PokedexActivityList::onActivate START \n";

    // Pokemon List DB Results
    SQLStatementStore *sql = SQLStatementStore::getInstance();
    std::string query      = sql->getQuery("get_pokemon_name_id");
    dbResults              = PokedexDB::executeSQL(&query);
    pokemon                = (*dbResults)[selectedIndex];
    print_dbResults();

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cerr << "PokedexActivityList::onActivate - Error in initSDL(), SDL Error: "
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    Mix_PlayChannel(-1, se_on_start, 0);

    needRedraw = true;

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate()
{
    pokemon.clear();

    selectedIndex = 0, offset = 0;
}

void PokedexActivityList::onLoop() { pokemon = (*dbResults)[selectedIndex]; }

void PokedexActivityList::onFreeze()
{
    // do thing for now..
}

PokedexActivityList *PokedexActivityList::getInstance() { return &instance; }
