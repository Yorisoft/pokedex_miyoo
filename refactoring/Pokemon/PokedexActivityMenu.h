#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

    SDL_Surface* surf_logo;
    SDL_Surface* listEntrySurface;
    TTF_Font* fontSurface;
    SDL_Surface* IDSurface;

    std::vector<std::vector<std::string>>* dbResults;
    int selectedIndex, offset, itemHeight;

    SDL_Color color;
    SDL_Color highlightColor;

    PokedexActivityMenu();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    static PokedexActivityMenu* getInstance();

    void onKeyDown(SDL_Keycode sym, Uint16 mod);

    void onKeyUp(SDL_Keycode sym, Uint16 mod);
};

#endif
