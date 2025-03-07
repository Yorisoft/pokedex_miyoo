#ifndef __POKEDEXACTIVITY_SETTING__ 
#define __POKEDEXACTIVITY_SETTING__ 

#include "PokedexActivity.h"
#include <variant>

class PokedexActivitySetting : public PokedexActivity {
private:
    static PokedexActivitySetting instance;

    static const std::string userConfigFile;
    std::map<std::string, int> userSettingMap;

	const std::string
		BACKGROUND_IMG_PATH = "res/assets/misc/settings_background.png",
		LIST_BACKGROUND_IMG_PATH = "res/assets/misc/setting_item_background.png",
		FONT_PATH = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf";

    int selectedSettingIndex, selectedOptionIndex, offset, itemHeight;
	bool needRedraw;

    std::vector<std::pair<SDL_Surface*, SDL_Surface*>> optionNameSurface_cache;
	std::vector<std::vector<std::pair<SDL_Surface*, SDL_Surface*>>> settingOptionsSurface_cache;

    SDL_Surface* backgroundSurface, * listEntrySurface_selected,
		* listEntrySurface_default;
    TTF_Font* fontSurface;
	SDL_Rect backgroundRect, listEntryRect,
			 settingNameRect, settingOptionRect;
    SDL_Color COLOR, HIGHLIGHTED_COLOR;

    std::string setting;
    std::vector<std::string>* settings;
    std::vector<std::vector<std::string>>* languages;
    std::vector<std::vector<std::string>> audioOptions;
    std::vector<std::vector<std::vector<std::string>>>* optionItems;
    std::vector<std::vector<std::string>> settingOptions;
     
	bool initSDL();
	void clearCachedSurfaces();
    bool renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i);
    bool renderSettingOptions(SDL_Surface* surf_display, SDL_Rect* settingRect, TTF_Font* font, int i);
    void loadUserConfig(const std::string&);
    void setUserConfig(const std::string&);
    
public:
	PokedexActivitySetting();
	~PokedexActivitySetting();
    
    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect);
    void onFreeze();

    static PokedexActivitySetting* getInstance();

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