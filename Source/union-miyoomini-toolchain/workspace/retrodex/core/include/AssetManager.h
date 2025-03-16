#ifndef __ASSET_MANAGER__
#define __ASSET_MANAGER__

#include "PokedexAssets.h"

class AssetManager : public PokedexAssets
{
  public:
    typedef std::unordered_map<t_assetType, std::unordered_map<t_assetID, t_asset>> t_assetMap;

  private:
    static AssetManager instance;

    const std::string MISC_SPRITES_PATH = "res/assets/misc/", FONT_PATH = "res/assets/font/",
                      POKEMON_ICONS_PATH   = "res/assets/pokemons/icons/",
                      POKEMON_SPRITES_PATH = "res/assets/pokemons/sprites/",
                      TYPES_SPRITES_PATH   = "res/assets/pokemons/types/";

    /* const std::string SOUND_EFFECT_ON_START_PATH = "res/assets/sound_effects/list_start.wav", */
    /*                   SOUND_EFFECT_ON_EXIT_PATH  = "res/assets/sound_effects/list_back.wav", */
    /*                   SOUND_EFFECT_UP_DOWN       = "res/assets/sound_effects/up_down.wav", */
    /*                   FONT_PATH                  =
     * "res/assets/font/pokemon-dppt/pokemon-dppt.ttf", */
    /*                   BACKGROUND_IMG_PATH        = "res/assets/misc/pokedexList_background.png",
     */
    /*                   SPRITES_IMG_BASE_PATH      = "res/assets/pokemons/sprites/", */
    /*                   TYPES_IMG_BASE_PATH        = "res/assets/pokemons/types/", */
    /*                   LIST_BACKGROUND_IMG_PATH_DEFAULT = */
    /*                       "res/assets/misc/menu_item_background_default.png", */
    /*                   LIST_BACKGROUND_IMG_PATH_SELECTED = */
    /*                       "res/assets/misc/menu_item_background_selected.png"; */

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