#include "AssetManager.h"
#include "SDL_image.h"
#include "name_to_id.h"
#include <filesystem>
#include <iostream>

AssetManager AssetManager::instance;

AssetManager::AssetManager()
    : loadedAssets(0), index(0), allAssetsLoaded(false), file(""), currentAssetID(t_assetID(0))
{
    assetMap = new t_assetMap();

    // Pokemon Sprites
    for (const auto &sprite : std::filesystem::directory_iterator(POKEMON_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(sprite) && sprite.path().extension() == ".png")
        { // Ensure it's a file and a png
            t_assetType type         = POKEMON_SPRITES;
            std::string path         = sprite.path().string();
            std::string name         = sprite.path().stem().string();
            std::pair<int, int> size = {2, 2};

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

    // Pokemon Icons
    for (const auto &icon : std::filesystem::directory_iterator(POKEMON_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(icon) && icon.path().extension() == ".png")
        { // Ensure it's a file
            t_assetType type         = POKEMON_ICON;
            std::string path         = icon.path().string();
            std::string name         = icon.path().stem().string();
            std::pair<int, int> size = {2, 2};

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

    for (auto &ast_type : *assetMap)
        for (auto &ast : ast_type.second)
            totalAssets++;

    currentAssetType = POKEMON_SPRITES; // POKEMON_SPRITES == 0
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
    /*     SOUND_EFFECT, */
    /*     FONT, */
    /*     POKEMON_SPRITES, */
    /*     POKEMON_ICON, */
    /*     POKEMON_CRY, */
    /*     ITEMS */
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

    asset.surface = SDL_CreateRGBSurfaceWithFormat(
        0, asset.size.w * optimizedSurface->w, asset.size.h * optimizedSurface->h,
        optimizedSurface->format->BitsPerPixel, optimizedSurface->format->format);

    SDL_Rect assetSurfaceRect = {
        0,
        0,
        asset.surface->w,
        asset.surface->h,
    };
    SDL_BlitScaled(optimizedSurface, NULL, asset.surface, &assetSurfaceRect);
    SDL_FreeSurface(optimizedSurface);
    optimizedSurface = nullptr;
}

void AssetManager::loadFont(t_asset &asset) {}

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
