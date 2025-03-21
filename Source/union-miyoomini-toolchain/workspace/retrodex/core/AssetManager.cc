// Window Settings
#define WINDOW_HEIGHT 480 // window height in pixels
#define WINDOW_WIDTH 640  // window width in pixels

#include "AssetManager.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "name_to_id.h"
#include <filesystem>
#include <iostream>

AssetManager AssetManager::instance;

AssetManager::AssetManager()
    : loadedAssets(0), index(0), allAssetsLoaded(false), file(""), currentAssetType(FONT),
      currentAssetID(t_assetID(0))
{
    assetMap = new t_assetMap();

    //=================================================LOAD_ASSETS================================================================
    std::string path;
    t_assetType type;
    std::string name;
    std::pair<int, int> size;

    //===================== Fonts
    type = FONT;
    path = FONT_PATH + "pokemon-dppt/pokemon-dppt.ttf";
    name = "pokemon-dppt-m";
    size = {34, 0}; // {font_size, blank_value}

    (*assetMap)[type].emplace(FONT_POKEMON_DPPT_M, t_asset(name, path, type, size));

    type = FONT;
    path = FONT_PATH + "pokemon-dppt/pokemon-dppt.ttf";
    name = "pokemon-dppt-l";
    size = {46, 0}; // {font_size, blank_value}

    (*assetMap)[type].emplace(FONT_POKEMON_DPPT_L, t_asset(name, path, type, size));

    type = FONT;
    path = FONT_PATH + "pokemon-advanced-battle/pokemon-advanced-battle.ttf";
    name = "pokemon-advanced-battle-l";
    size = {46, 0}; // {font_size, blank_value}

    (*assetMap)[type].emplace(FONT_POKEMON_ADVANCED_BATTLE_L, t_asset(name, path, type, size));

    /* std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name << std::endl; */

    //===================== Backgrounds
    type = MISC;
    path = MISC_SPRITES_PATH + "menu_background.png";
    name = "menu_background";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_MAIN_MENU_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokedexList_background.png";
    name = "pokedex_background";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_POKEDEX_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokemon_fr_view_1.png";
    name = "pokemon_view_1";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_INFO_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokemon_fr_view_2.png";
    name = "pokemon_view_2";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_STATS_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokemon_fr_view_3.png";
    name = "pokemon_view_3";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_MOVES_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokemon_fr_view_4.png";
    name = "pokemon_view_4";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_LOCATION_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokemon_fr_view_5.png";
    name = "pokemon_view_5";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_EVOLUTION_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "settings_background.png";
    name = "settings_background";
    size = {WINDOW_WIDTH, WINDOW_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_SETTINGS_SCREEN_BACKGROUND, t_asset(name, path, type, size));

    //===================== List Item Backgrounds
    const int MENU_ITEM_HEIGHT      = static_cast<int>(WINDOW_HEIGHT / 5);
    const int POKEDEX_ITEM_HEIGHT   = static_cast<int>(WINDOW_HEIGHT * 0.6 / 5);
    double heightRatio              = static_cast<double>(POKEDEX_ITEM_HEIGHT) / 22.0;
    const int EVOLUTION_ITEM_WIDTH  = static_cast<int>(WINDOW_WIDTH * 0.5);
    const int EVOLUTION_ITEM_HEIGHT = static_cast<int>((WINDOW_HEIGHT / 3) * 0.7);
    const int SETTINGS_ITEM_WIDTH   = static_cast<int>(WINDOW_WIDTH * .87);
    const int SETTINGS_ITEM_HEIGHT  = static_cast<int>(WINDOW_HEIGHT / 9);

    type = MISC;
    path = MISC_SPRITES_PATH + "menu_item_background_default.png";
    name = "menu_item_background_default";
    size = {WINDOW_WIDTH, MENU_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_MENU_ITEM_BACKGROUND_DEFAULT,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "menu_item_background_selected.png";
    name = "menu_item_background_selected";
    size = {WINDOW_WIDTH, MENU_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_MENU_ITEM_BACKGROUND_SELECTED,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokedex_item_background_default.png";
    name = "pokedex_item_background_default";
    size = {static_cast<int>(heightRatio * 151), POKEDEX_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_POKEDEX_ITEM_BACKGROUND_DEFAULT,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "pokedex_item_background_selected.png";
    name = "pokedex_item_background_selected";
    size = {static_cast<int>(heightRatio * 151), POKEDEX_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_POKEDEX_ITEM_BACKGROUND_SELECTED,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "evolution_item_background_default.png";
    name = "evolution_item_background_selected";
    size = {EVOLUTION_ITEM_WIDTH, EVOLUTION_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_DEFAULT,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "evolution_item_background_selected.png";
    name = "evolution_item_background_selected";
    size = {EVOLUTION_ITEM_WIDTH, EVOLUTION_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_SELECTED,
                              t_asset(name, path, type, size));

    type = MISC;
    path = MISC_SPRITES_PATH + "setting_item_background.png";
    name = "setting_item_background";
    size = {SETTINGS_ITEM_WIDTH, SETTINGS_ITEM_HEIGHT};

    (*assetMap)[type].emplace(SURFACE_SETTINGS_LIST_ITEM_BACKGROUND,
                              t_asset(name, path, type, size));

    //===================== Pokemon Sprites
    for (const auto &sprite : std::filesystem::directory_iterator(POKEMON_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(sprite) && sprite.path().extension() == ".png")
        { // Ensure it's a file and a png
            t_assetType type         = POKEMON_SPRITES;
            std::string path         = sprite.path().string();
            std::string name         = sprite.path().stem().string();
            std::pair<int, int> size = {192, 192};

            if (POKEMON_NAMETOID_MAP.find(name) != POKEMON_NAMETOID_MAP.end())
            {
                t_asset sprite_asset(name, path, type, size);

                (*assetMap)[type].emplace(POKEMON_NAMETOID_MAP.at(name), sprite_asset);
            }
            else
            {
                std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name
                          << std::endl;
            }
        }
    }

    //=====================  Pokemon Icons
    for (const auto &icon : std::filesystem::directory_iterator(POKEMON_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(icon) && icon.path().extension() == ".png")
        { // Ensure it's a file
            t_assetType type         = POKEMON_ICON;
            std::string path         = icon.path().string();
            std::string name         = icon.path().stem().string();
            std::pair<int, int> size = {112, 84};

            if (POKEMON_NAMETOID_MAP.find(name) != POKEMON_NAMETOID_MAP.end())
            {
                t_asset icon_asset(name, path, type, size);

                (*assetMap)[type].emplace(POKEMON_NAMETOID_MAP.at(name), icon_asset);
            }
            else
            {
                std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name
                          << std::endl;
            }
        }
    }

    //=====================  Type Icons
    for (const auto &pokeType : std::filesystem::directory_iterator(TYPES_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(pokeType) && pokeType.path().extension() == ".png")
        { // Ensure it's a file
            t_assetType type         = TYPES;
            std::string path         = pokeType.path().string();
            std::string name         = pokeType.path().stem().string();
            std::pair<int, int> size = {96, 32};

            if (POKEMON_NAMETOID_MAP.find(name) != POKEMON_NAMETOID_MAP.end())
            {
                t_asset type_asset(name, path, type, size);

                (*assetMap)[type].emplace(POKEMON_NAMETOID_MAP.at(name), type_asset);
            }
            else
            {
                std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name
                          << std::endl;
            }
        }
    }

    //===================== Encounter
    for (const auto &encounter : std::filesystem::directory_iterator(ENCOUNTER_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(encounter) && encounter.path().extension() == ".png")
        { // Ensure it's a file
            t_assetType type         = MISC;
            std::string path         = encounter.path().string();
            std::string name         = encounter.path().stem().string();
            std::pair<int, int> size = {36, 36};

            if (POKEMON_NAMETOID_MAP.find(name) != POKEMON_NAMETOID_MAP.end())
            {
                t_asset ecounter_asset(name, path, type, size);

                (*assetMap)[type].emplace(POKEMON_NAMETOID_MAP.at(name), ecounter_asset);
            }
            else
            {
                std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name
                          << std::endl;
            }
        }
    }

    //===================== Items
    for (const auto &item : std::filesystem::directory_iterator(ITEMS_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(item) && item.path().extension() == ".png")
        { // Ensure it's a file
            t_assetType type         = ITEMS;
            std::string path         = item.path().string();
            std::string name         = item.path().stem().string();
            std::pair<int, int> size = {36, 36};

            if (POKEMON_NAMETOID_MAP.find(name) != POKEMON_NAMETOID_MAP.end())
            {
                t_asset item_asset(name, path, type, size);

                (*assetMap)[type].emplace(POKEMON_NAMETOID_MAP.at(name), item_asset);
            }
            else
            {
                std::cout << "Could not find asset in NAMETOID table. Asset Name: " << name
                          << std::endl;
            }
        }
    }

    //=================================================COUNT_ASSETS================================================================

    for (auto &ast_type : *assetMap)
        for (auto &ast : ast_type.second)
            totalAssets++;
}

AssetManager::~AssetManager()
{
    for (std::pair<const t_assetType, std::unordered_map<t_assetID, t_asset>> &assets : *assetMap)
    {
        for (std::pair<const t_assetID, t_asset> &asset : assets.second)
        {
            if (asset.second.surface)
                SDL_FreeSurface(asset.second.surface);

            asset.second.surface = nullptr;
        }
    }
}

void AssetManager::loadAssets()
{
    /* typedef enum _assetType */
    /* { */
    /* FONT, */
    /* MISC, */
    /* POKEMON_SPRITES, */
    /* POKEMON_ICON, */
    /* TYPES, */
    /* ITEMS, */
    /* POKEMON_CRY, */
    /* SOUND_EFFECT, */
    /* } t_assetType; */

    if (!allAssetsLoaded)
    {
        auto &currentAssetCategory = (*assetMap)[currentAssetType];
        if (index <= currentAssetCategory.size())
        {
            auto it = currentAssetCategory.begin();
            std::advance(it, index);
            t_asset *current_asset = &it->second;

            switch (current_asset->type)
            {
            case POKEMON_SPRITES:
            case POKEMON_ICON:
            case ITEMS:
            case MISC:
            case TYPES:
                loadSurface(*current_asset);
                break;
            case SOUND_EFFECT:
            case POKEMON_CRY:
                loadAudio(*current_asset);
                break;
            case FONT:
                loadFont(*current_asset);
                break;
            }

            current_asset->isLoaded = true;

            loadedAssets++;
            index++;

            file = current_asset->name;
        }

        if (index == currentAssetCategory.size()) // end of inner map
        {
            currentAssetType = t_assetType(currentAssetType + 1);
            index            = 0;

            if (currentAssetType == (*assetMap).size()) // end of out map
            {
                allAssetsLoaded = true;
            }
        }
    }
}

void AssetManager::loadSurface(t_asset &asset)
{
    SDL_Surface *tempSurface = IMG_Load(asset.path.c_str());
    if (!tempSurface)
    {
        std::cout << "AssetManager::loadAssets() Unable to load tempSurface! File: "
                  << asset.path.c_str() << ".  SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Surface *optimizedSurface =
        SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!optimizedSurface)
    {
        std::cout << "AssetManager::loadAssets() Unable to create surface for "
                     "optimizedSurface! SDL Error: "
                  << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(tempSurface);
    tempSurface = nullptr;

    /* if (asset.size.w == -1 || asset.size.h == -1) // using -1 to represent fullscreen asset */
    /* { */
    /*     asset.surface = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT, */
    /*                                                    optimizedSurface->format->BitsPerPixel, */
    /*                                                    optimizedSurface->format->format); */
    /* } */
    /* else */
    /* { */
    /*     asset.surface = SDL_CreateRGBSurfaceWithFormat( */
    /*         0, asset.size.w * optimizedSurface->w, asset.size.h * optimizedSurface->h, */
    /*         optimizedSurface->format->BitsPerPixel, optimizedSurface->format->format); */
    /* } */

    asset.surface = SDL_CreateRGBSurfaceWithFormat(0,
                                                   asset.size.w,
                                                   asset.size.h,
                                                   optimizedSurface->format->BitsPerPixel,
                                                   optimizedSurface->format->format);

    SDL_BlitScaled(optimizedSurface, NULL, asset.surface, NULL);
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = nullptr;
}

void AssetManager::loadFont(t_asset &asset)
{ // Using size.w for font size
    asset.font = TTF_OpenFont(asset.path.c_str(), asset.size.w);
    if (asset.font == NULL)
    {
        std::cout << "Unable to load font!"
                  << "name: " << asset.name << " path: " << asset.path
                  << " SDL_Error:  " << TTF_GetError();
    }
}

void AssetManager::loadAudio(t_asset &asset) {}

AssetManager::t_asset *AssetManager::getAsset(const t_assetType type, const t_assetID id) const
{
    if (assetMap->find(type) != assetMap->end())
    {
        if ((*assetMap)[type].find(id) != (*assetMap)[type].end())
        {
            return &(*assetMap)[type][id];
        }
        else
        {
            std::cout << "Error: id - " << id << " is not a valid asset id \n";
        }
    }
    else
    {
        std::cout << "Error: type - " << type << " is not a valid asset map type \n";
    }

    return nullptr;
}

double AssetManager::getCurrentProgress()
{
    return (static_cast<double>(loadedAssets) / totalAssets * 100);
}

bool AssetManager::isDoneLoading() { return allAssetsLoaded; }

std::string AssetManager::getFile() { return file; }

AssetManager *AssetManager::getInstance() { return &instance; }
