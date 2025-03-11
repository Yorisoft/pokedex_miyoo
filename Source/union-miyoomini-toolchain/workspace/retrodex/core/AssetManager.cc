// Window Settings
#define WINDOW_HEIGHT 480 // window height in pixels
#define WINDOW_WIDTH 640  // window width in pixels

#include "AssetManager.h"
#include "PokeSurface.h"
#include <filesystem>

AssetManager AssetManager::instance;

AssetManager::AssetManager()
{
    assetMap        = new assetMap_t();
    allAssetsLoaded = false;
    file            = "";

    // Add the surfaces I want to this map
    // s = small
    // m = medium
    // l = large
    // f = fullscreen

    // Pokemon Icons
    for (const auto &icon : std::filesystem::directory_iterator(POKEMON_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(icon))
        { // Ensure it's a file
            if (icon.path().extension() == ".png")
            {
                std::string path         = icon.path().string();
                std::pair<int, int> size = {2, 2};

                asset icon_asset(path, size);
                assetMap->insert({icon.path().stem().string() + "_s", icon_asset});
            }
        }
    }

    // Pokemon Sprites
    for (const auto &sprite : std::filesystem::directory_iterator(POKEMON_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(sprite))
        { // Ensure it's a file
            if (sprite.path().extension() == ".png")
            {
                std::string path         = sprite.path().string();
                std::pair<int, int> size = {2, 2};

                asset sprite_asset(path, size);
                assetMap->insert({sprite.path().stem().string() + "_l", sprite_asset});
            }
        }
    }

    loadedAssetsIndex = 0;
    totalAssets       = assetMap->size();
}

AssetManager::~AssetManager()
{
    for (std::pair<const std::string, asset_t> &asset : *assetMap)
    {
        if (asset.second.surface)
            SDL_FreeSurface(asset.second.surface);

        asset.second.surface = nullptr;
    }
}

void AssetManager::loadAssets()
{
    if (loadedAssetsIndex < totalAssets)
    {
        auto it = std::next(assetMap->begin(), loadedAssetsIndex);

        SDL_Surface *tempSurface = IMG_Load(it->second.path.c_str());
        if (!tempSurface)
        {
            std::cout << "AssetManager::loadAssets() Unable to load temp surface! File: "
                      << it->second.path.c_str() << ".  SDL Error: " << IMG_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        it->second.surface = SDL_CreateRGBSurfaceWithFormat(
            0, it->second.size.w * tempSurface->w, it->second.size.h * tempSurface->h,
            tempSurface->format->BitsPerPixel, tempSurface->format->format);
        if (!it->second.surface)
        {
            std::cout << "AssetManager::loadAssets() Unable to create surface for "
                         "it->second.surface! SDL Error: "
                      << IMG_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_BlitSurface(tempSurface, NULL, it->second.surface, NULL);
        SDL_FreeSurface(tempSurface);

        file = it->second.path;

        it->second.isLoaded = true;

        loadedAssetsIndex++;
    }

    allAssetsLoaded = loadedAssetsIndex == totalAssets ? true : false;
}

AssetManager::asset *AssetManager::getAsset(const std::string &assetName)
{
    auto it = assetMap->find(assetName);
    if (it != assetMap->end())
    {
        return &it->second;
    }
    return nullptr;
}

double AssetManager::getCurrentProgress() { return loadedAssetsIndex / totalAssets * 100; }

bool AssetManager::isDoneLoading() { return allAssetsLoaded; }

std::string AssetManager::getFile() { return file; }

AssetManager *AssetManager::getInstance() { return &instance; }
