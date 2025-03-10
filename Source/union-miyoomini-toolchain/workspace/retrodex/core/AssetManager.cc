// Window Settings
#define WINDOW_HEIGHT 480 // window height in pixels
#define WINDOW_WIDTH 640  // window width in pixels

#include "AssetManager.h"
#include "PokeSurface.h"
#include <filesystem>

AssetManager AssetManager::instance;

AssetManager::AssetManager()
{
    assetMap = new assetMap_t();
    allAssetsLoaded = false;
    file = "";

    // Add the surfaces I want to this map
    // s = snall
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
                std::string path = icon.path().string();
                std::pair<int, int> size = {100, 100};

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
                std::string path = sprite.path().string();
                std::pair<int, int> size = {100, 100};

                asset sprite_asset(path, size);
                assetMap->insert({sprite.path().stem().string() + "_l", sprite_asset});
            }
        }
    }

    loadedAssetsIndex = 0;
    totalAssets = assetMap->size();
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

        it->second.surface = PokeSurface::onLoadImg(it->second.path);
        if (it->second.surface == NULL)
        {
            std::cout << "Unable to load asset! Asset path" << it->second.path
                      << "\nSDL Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_Surface *scaledSurface = SDL_CreateRGBSurface(
            0, it->second.size.w, it->second.size.h, it->second.surface->format->BitsPerPixel,
            it->second.surface->format->Rmask, it->second.surface->format->Gmask,
            it->second.surface->format->Bmask, it->second.surface->format->Amask);

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
