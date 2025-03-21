#ifndef __POKEDEXACTIVITY_SETTING__
#define __POKEDEXACTIVITY_SETTING__

#include "AssetManager.h"
#include "PokedexActivity.h"
#include <variant>

class PokedexActivitySetting : public PokedexActivity
{
  private:
    static PokedexActivitySetting instance;

    const SDL_Color COLOR = {0, 0, 0}, HIGHLIGHTED_COLOR = {248, 184, 112};

    const int ITEM_HEIGHT = (static_cast<int>(WINDOW_HEIGHT / 9));

    static const std::string userConfigFile;
    std::map<std::string, int> userSettingMap;

    int selectedSettingIndex, selectedOptionIndex, offset;
    bool needRedraw;

    SDL_Surface *backgroundSurface, *listEntrySurface_selected, *listEntrySurface_default;
    TTF_Font *fontSurface;
    SDL_Rect backgroundRect, listEntryRect, settingNameRect, settingOptionRect;

    std::string setting;
    std::vector<std::string> *settings;
    std::vector<std::vector<std::string>> *languages;
    std::vector<std::vector<std::string>> audioOptions;
    std::vector<std::vector<std::vector<std::string>>> *optionItems;
    std::vector<std::vector<std::string>> settingOptions;

    AssetManager *assetManager;

    bool initSDL();
    void clearCachedSurfaces();
    bool renderListItems(SDL_Surface *surf_display, TTF_Font *font, int i);
    bool renderSettingOptions(SDL_Surface *surf_display, SDL_Rect *settingRect, TTF_Font *font,
                              int i);
    void loadUserConfig(const std::string &);
    void setUserConfig(const std::string &);

  public:
    PokedexActivitySetting();
    ~PokedexActivitySetting();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface *surf_display, SDL_Renderer *renderer, SDL_Texture *texture,
                  TTF_Font *font, Mix_Chunk *sEffect);
    void onFreeze();

    static PokedexActivitySetting *getInstance();

    void onButtonUp(SDL_Keycode, Uint16);
    void onButtonDown(SDL_Keycode, Uint16);
    void onButtonLeft(SDL_Keycode, Uint16);
    void onButtonRight(SDL_Keycode, Uint16);
    void onButtonA(SDL_Keycode, Uint16);
    void onButtonB(SDL_Keycode, Uint16);
    void onButtonR(SDL_Keycode, Uint16);
    void onButtonL(SDL_Keycode, Uint16);
    void onButtonSelect(SDL_Keycode, Uint16);
    void onButtonStart(SDL_Keycode, Uint16);
};

#endif