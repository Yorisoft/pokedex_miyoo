#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu()
    : selectedIndex(0), offset(0), needRedraw(true), dbResults(nullptr), backgroundSurface(nullptr),
      listEntrySurface_default(nullptr), listEntrySurface_selected(nullptr), fontSurface(nullptr),
      se_up_down(nullptr), assetManager(nullptr)
{
}

PokedexActivityMenu::~PokedexActivityMenu()
{
    if (se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;
}

void PokedexActivityMenu::print_dbResults()
{
    for (std::vector<std::string> &row : *dbResults)
    {
        for (auto &col : row)
        {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivityMenu::onActivate()
{
    std::cout << "PokedexActivityMenu::onActivate START \n";

    needRedraw = true;

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    game      = (*dbResults)[selectedIndex];
    print_dbResults();

    assetManager = AssetManager::getInstance();

    /* clearCacheSurfaces(); */

    if (!initSDL())
    {
        std::cout << "PokedexActivityMenu::onActivate - Error in initSDL(), SDL Error: "
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() { game.clear(); }

void PokedexActivityMenu::onLoop()
{
    // Set Game version and regional pokedex ID for PokedexDB
    game = (*dbResults)[selectedIndex];
}

void PokedexActivityMenu::onFreeze() {}

PokedexActivityMenu *PokedexActivityMenu::getInstance() { return &instance; }
