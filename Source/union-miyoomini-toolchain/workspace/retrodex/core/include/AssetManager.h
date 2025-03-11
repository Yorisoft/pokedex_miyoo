#ifndef __ASSET_MANAGER__
#define __ASSET_MANAGER__

#include "PokedexAssets.h"
#include <unordered_map>

class AssetManager : public PokedexAssets
{
  public:
    typedef std::unordered_map<int, asset> assetMap_t;

  private:
    static AssetManager instance;

    assetMap_t *assetMap;

    int currentAssetID = 0;
    double loadedAssetsIndex, totalAssets;
    bool allAssetsLoaded;

    std::string file;

    AssetManager();
    ~AssetManager();

  public:
    void loadAssets();
    void loadSurface(asset &);
    void loadFont(asset &);
    void loadAudio(asset &);
    asset *getAsset(const int) const;

    double getCurrentProgress();
    bool isDoneLoading();

    std::string getFile();

    static AssetManager *getInstance();
};

#endif
