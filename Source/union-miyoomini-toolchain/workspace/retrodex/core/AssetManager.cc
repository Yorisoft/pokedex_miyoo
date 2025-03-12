// Window Settings
#define WINDOW_HEIGHT 480 // window height in pixels
#define WINDOW_WIDTH 640  // window width in pixels

#include "AssetManager.h"
#include "SDL_image.h"
#include <filesystem>
#include <iostream>

AssetManager AssetManager::instance;

AssetManager::AssetManager()
{
    assetMap        = new assetMap_t();
    allAssetsLoaded = false;
    file            = "";
    currentAssetID  = PokedexAssets::Asset_ID(0);

    // Add the surfaces I want to this map
    // s = small
    // m = medium
    // l = large
    // f = fullscreen

    // Pokemon Sprites
    for (const auto &sprite : std::filesystem::directory_iterator(POKEMON_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(sprite))
        { // Ensure it's a file
            if (sprite.path().extension() == ".png")
            {
                char type[]              = "surface";
                std::string path         = sprite.path().string();
                std::pair<int, int> size = {2, 2};

                asset sprite_asset(path, type, size);
                assetMap->insert({currentAssetID, sprite_asset});
                currentAssetID = PokedexAssets::Asset_ID(currentAssetID + 1);
                std::cout << "Inserted asset: " << currentAssetID << " at path: " << path
                          << std::endl;
            }
        }
    }

    // Pokemon Icons
    for (const auto &icon : std::filesystem::directory_iterator(POKEMON_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(icon))
        { // Ensure it's a file
            if (icon.path().extension() == ".png")
            {
                char type[]              = "surface";
                std::string path         = icon.path().string();
                std::pair<int, int> size = {2, 2};

                asset icon_asset(path, type, size);
                assetMap->insert({currentAssetID, icon_asset});
                currentAssetID = PokedexAssets::Asset_ID(currentAssetID + 1);
                std::cout << "Inserted asset: " << currentAssetID << " at path: " << path
                          << std::endl;
            }
        }
    }

    loadedAssetsIndex = 0;
    currentAssetID    = PokedexAssets::Asset_ID(0);
    totalAssets       = assetMap->size();
}

AssetManager::~AssetManager()
{
    for (std::pair<const Asset_ID, asset_t> &asset : *assetMap)
    {
        if (asset.second.surface)
            SDL_FreeSurface(asset.second.surface);

        asset.second.surface = nullptr;
    }
}

void AssetManager::loadAssets()
{
    enum
    {
        SURFACE,
        FONT,
        MIXCHUNK,
    };
    int assetType;

    if (loadedAssetsIndex < totalAssets && currentAssetID < PokedexAssets::ASSET_COUNT)
    {
        asset *current_asset = &assetMap->at(PokedexAssets::Asset_ID(currentAssetID));
        int type;

        if (strcmp(current_asset->type, "surface") == 0)
            loadSurface(*current_asset);
        else if (strcmp(current_asset->type, "font") == 0)
            loadFont(*current_asset);
        else if (strcmp(current_asset->type, "audio") == 0)
            loadAudio(*current_asset);

        file = current_asset->path;

        current_asset->isLoaded = true;

        loadedAssetsIndex++;

        currentAssetID = PokedexAssets::Asset_ID(currentAssetID + 1);
    }

    if (loadedAssetsIndex == totalAssets)
    {
        allAssetsLoaded = true;

        currentAssetID = PokedexAssets::Asset_ID(0);
    }
}

void AssetManager::loadSurface(asset &asset)
{
    SDL_Surface *tempSurface = IMG_Load(asset.path.c_str());
    if (!tempSurface)
    {
        std::cout << "AssetManager::loadAssets() Unable to load temp surface! File: "
                  << asset.path.c_str() << ".  SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    asset.surface = SDL_CreateRGBSurfaceWithFormat(
        0, asset.size.w * tempSurface->w, asset.size.h * tempSurface->h,
        tempSurface->format->BitsPerPixel, tempSurface->format->format);
    if (!asset.surface)
    {
        std::cout << "AssetManager::loadAssets() Unable to create surface for "
                     "asset.surface! SDL Error: "
                  << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_BlitSurface(tempSurface, NULL, asset.surface, NULL);
    SDL_FreeSurface(tempSurface);
}

void AssetManager::loadFont(asset &asset) {}

void AssetManager::loadAudio(asset &asset) {}

AssetManager::asset *AssetManager::getAsset(const int asset_id) const
{
    auto it = assetMap->find(PokedexAssets::Asset_ID(asset_id));
    if (it != assetMap->end())
    {
        return &it->second;
    }
    return nullptr;
}

double AssetManager::getCurrentProgress()
{
    return (static_cast<double>(loadedAssetsIndex) / totalAssets * 100);
}

bool AssetManager::isDoneLoading() { return allAssetsLoaded; }

std::string AssetManager::getFile() { return file; }

AssetManager *AssetManager::getInstance() { return &instance; }
