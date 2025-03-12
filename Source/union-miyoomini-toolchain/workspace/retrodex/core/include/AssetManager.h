#ifndef __ASSET_MANAGER__
#define __ASSET_MANAGER__

#include "PokedexAssets.h"
#include <unordered_map>

class AssetManager : public PokedexAssets
{
  public:
    typedef std::unordered_map<Asset_ID, asset> assetMap_t;

  private:
    static AssetManager instance;

    int loadedAssetsIndex, totalAssets;
    bool allAssetsLoaded;

    std::string file;

    assetMap_t *assetMap;

    Asset_ID currentAssetID;

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
