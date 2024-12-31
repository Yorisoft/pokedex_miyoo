#ifndef __POKEDEXACTIVITY_SETTING__ 
#define __POKEDEXACTIVITY_SETTING__ 

#include "PokedexActivity.h"

class PokedexActivitySetting : public PokedexActivity {
    private:
        static const int OPTION_ITEMS_SIZE = 1;

        static PokedexActivitySetting instance;
        std::vector<std::string>* optionItems;
        std::map<int, std::string> language_map;
    
    
        int selectedIndex, offset, itemHeight;
        std::string fontPath;
    
        SDL_Surface* backgroundSurface, * listEntrySurface;
        TTF_Font* fontSurface;
        SDL_Color color, highlightColor;
    
        bool renderListItems(SDL_Surface* surf_display, int i);
        //void printOptionsInfo();
    
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