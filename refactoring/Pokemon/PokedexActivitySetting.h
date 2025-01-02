#ifndef __POKEDEXACTIVITY_SETTING__ 
#define __POKEDEXACTIVITY_SETTING__ 

#include "PokedexActivity.h"
#include <variant>

enum  {
    LANGUAGE = 0, 
    AUDIO 
};
class PokedexActivitySetting : public PokedexActivity {
private:
    static const int OPTION_ITEMS = 2;
    static const std::string userConfigFile;

    static PokedexActivitySetting instance;
    std::vector<std::string>* settings;
    std::string setting;
    std::vector<std::vector<std::string>>* languages;
    std::vector<std::vector<std::string>> audioOptions;
    std::vector<std::vector<std::vector<std::string>>>* optionItems;
    std::vector<std::vector<std::string>> settingOptions;
     
    std::vector<std::pair<std::string, std::variant<int, std::string>>> userConfigs;

    int selectedSettingIndex, selectedOptionIndex, offset, itemHeight;
    std::string fontPath;

    std::map<std::string, int> userSettingMap;
    SDL_Surface* backgroundSurface, * listEntrySurface,
        * optionNameSurface, * settingOptionsSurface;
    TTF_Font* fontSurface;
    SDL_Color color, highlightColor;

    bool renderListItems(SDL_Surface* surf_display, int i);
    bool renderSettingOptions(SDL_Surface* surf_display, SDL_Rect* settingRect, int i);
    //void printOptionsInfo();
    void loadUserConfig(const std::string&);
    void setUserConfig(const std::string&);
    
public:
        PokedexActivitySetting();
        ~PokedexActivitySetting();
    
    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);
    void onFreeze();
//
    static PokedexActivitySetting* getInstance();
//
    void onButtonUp(SDL_Keycode, Uint16);
    void onButtonDown(SDL_Keycode, Uint16);
    void onButtonLeft(SDL_Keycode, Uint16);
    void onButtonRight(SDL_Keycode, Uint16);
    void onButtonA(SDL_Keycode, Uint16);
    void onButtonB(SDL_Keycode, Uint16);
    void onButtonSelect(SDL_Keycode, Uint16);
    void onButtonStart(SDL_Keycode, Uint16);
};

#endif