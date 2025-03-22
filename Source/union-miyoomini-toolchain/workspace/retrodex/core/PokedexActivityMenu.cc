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

void PokedexActivityMenu::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                   SDL_Texture *texture, TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        // List Items
        for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++)
        {
            if (!renderListItems(surf_display, i))
            {
                exit(EXIT_FAILURE);
            }
        }

        // List Items
        for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++)
        {
            if (!renderListItems(surf_display, i))
            {
                exit(EXIT_FAILURE);
            }
        }

        needRedraw = false;
    }
}

bool PokedexActivityMenu::renderListItems(SDL_Surface *surf_display, int i)
{
    // List item background
    SDL_Surface *version =
        TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[offset + i][2].c_str(), COLOR);
    if (version == NULL)
    {
        std::cout << "Unable to load surface!"
                  << "name: " << version << " path: " << version
                  << " SDL_Error:  " << TTF_GetError();
    }

    SDL_Surface *version_highlight =
        TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[offset + i][2].c_str(), HIGHLIGHT_COLOR);
    if (version_highlight == NULL)
    {
        std::cout << "Unable to load surface!"
                  << "name: " << version_highlight << " path: " << version_highlight
                  << " SDL_Error:  " << TTF_GetError();
    }

    int leftBorder = 15;
    versionRect.x  = leftBorder + (WINDOW_WIDTH / 2) - (version->w / 2);
    versionRect.y  = (i * ITEM_HEIGHT) + (listEntryRect.h / 2) - (version->h / 2) - 10;
    versionRect.w  = version->w;
    versionRect.h  = version->h;

    listEntryRect.y = (i * ITEM_HEIGHT);

    if (offset + i == selectedIndex)
    {
        SDL_BlitSurface(listEntrySurface_selected, NULL, surf_display, &listEntryRect);
        SDL_BlitSurface(version_highlight, NULL, surf_display, &versionRect);
    }
    else
    {
        SDL_BlitSurface(listEntrySurface_default, NULL, surf_display, &listEntryRect);
        SDL_BlitSurface(version, NULL, surf_display, &versionRect);
    }

    SDL_FreeSurface(version);
    SDL_FreeSurface(version_highlight);

    return true;
}

void PokedexActivityMenu::onFreeze() {}

PokedexActivityMenu *PokedexActivityMenu::getInstance() { return &instance; }

void PokedexActivityMenu::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        if (selectedIndex < offset)
        {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivityMenu::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivityMenu::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonA(SDL_Keycode sym, Uint16 mod)
{
    PokedexDB::setVersionID(std::stoi(game[0]));
    PokedexDB::setGenerationID(std::stoi(game[5]));
    PokedexDB::setVersionGroupID(std::stoi(game[7]));

    PokedexActivityManager::push(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onButtonB(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 3)
    {
        selectedIndex += 3;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += 3;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS)
            {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
    else
    {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset        = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityMenu::onButtonL(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex >= 3)
    {
        selectedIndex -= 3;
        if (selectedIndex < offset)
        {
            offset -= 3; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0)
            {
                offset = 0; // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, se_up_down, 0);
    }
    else
    {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset        = 0; // Cap offset to zero
    }
}

void PokedexActivityMenu::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
