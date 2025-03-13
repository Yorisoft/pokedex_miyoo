#ifndef __ASSET_MANAGER__
#define __ASSET_MANAGER__

#include "PokedexAssets.h"

class AssetManager : public PokedexAssets
{
  public:
    typedef std::unordered_map<t_assetType, std::unordered_map<t_assetID, t_asset>> t_assetMap;

  private:
    static AssetManager instance;

    const std::string POKEMON_ICONS_PATH   = "res/assets/pokemons/icons/",
                      POKEMON_SPRITES_PATH = "res/assets/pokemons/sprites/";

    int loadedAssets, totalAssets, index;
    bool allAssetsLoaded;

    std::string file;

    t_assetMap *assetMap;

    t_assetType currentAssetType;

    t_assetID currentAssetID;

    AssetManager();
    ~AssetManager();

  public:
    void loadAssets();
    void loadSurface(t_asset &);
    void loadFont(t_asset &);
    void loadAudio(t_asset &);
    t_asset *getAsset(const t_assetType, const t_assetID) const;

    double getCurrentProgress();
    bool isDoneLoading();

    std::string getFile();

    static AssetManager *getInstance();
};

#endif