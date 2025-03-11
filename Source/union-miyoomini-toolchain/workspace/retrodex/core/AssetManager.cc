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

    // Pokemon Sprites
    for (const auto &sprite : std::filesystem::directory_iterator(POKEMON_SPRITES_PATH))
    {
        if (std::filesystem::is_regular_file(sprite))
        { // Ensure it's a file
            if (sprite.path().extension() == ".png")
            {
                std::string path         = sprite.path().string();
                std::pair<int, int> size = {2, 2};

                asset sprite_asset(path, "surface", size);
                assetMap->insert({currentAssetID, sprite_asset});
            }
        }
        currentAssetID++;
    }

    // Pokemon Icons
    for (const auto &icon : std::filesystem::directory_iterator(POKEMON_ICONS_PATH))
    {
        if (std::filesystem::is_regular_file(icon))
        { // Ensure it's a file
            if (icon.path().extension() == ".png")
            {
                std::string path         = icon.path().string();
                std::pair<int, int> size = {2, 2};

                char *type = "surface";
                asset icon_asset(path, type, size);
                assetMap->insert({currentAssetID, icon_asset});
            }
        }
        currentAssetID++;
    }

    loadedAssetsIndex = 0;
    totalAssets       = assetMap->size();
}

AssetManager::~AssetManager()
{
    for (std::pair<const int, asset_t> &asset : *assetMap)
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

    if (loadedAssetsIndex < totalAssets)
    {
        asset current_asset = assetMap->at(loadedAssetsIndex);
        int type;

        if (strcmp(current_asset.type, "surface") == 0)
            type = SURFACE;
        else if (strcmp(current_asset.type, "font") == 0)
            type = FONT;
        else if (strcmp(current_asset.type, "audio") == 0)
            type = MIXCHUNK;

        switch (type)
        {
        case SURFACE:
            loadSurface(current_asset);
            break;
        case FONT:
            loadFont(current_asset);
            break;
        case MIXCHUNK:
            loadAudio(current_asset);
            break;
        };

        file = current_asset.path;

        current_asset.isLoaded = true;

        loadedAssetsIndex++;
    }

    allAssetsLoaded = loadedAssetsIndex == totalAssets ? true : false;
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
    auto it = assetMap->find(asset_id);
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
