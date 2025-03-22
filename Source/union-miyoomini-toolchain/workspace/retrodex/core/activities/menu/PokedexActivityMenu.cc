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

bool PokedexActivityMenu::initSDL()
{
    try
    {
        // AUDIO
        se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
        if (!se_up_down)
        {
            std::cerr << "Failed to load sound sound_up_down: " << Mix_GetError() << std::endl;
        }

        // FONT
        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_L)->font;

        // Background
        backgroundSurface =
            assetManager->getAsset(AssetManager::MISC, AssetManager::SURFACE_MAIN_MENU_BACKGROUND)
                ->surface;
        backgroundRect =
            assetManager->getAsset(AssetManager::MISC, AssetManager::SURFACE_MAIN_MENU_BACKGROUND)
                ->size;

        // List Item Background
        listEntrySurface_default =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_DEFAULT)
                ->surface;
        listEntrySurface_selected =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_SELECTED)
                ->surface;

        listEntryRect =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_DEFAULT)
                ->size;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
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
