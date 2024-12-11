#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "PokedexActivity.h"
#include "PokeSurface.h"

class PokedexActivityMenu : public PokedexActivity {
private:
    static PokedexActivityMenu instance;

    SDL_Surface* surf_logo;

    std::vector<std::vector<std::string>>* dbResults;

    std::map<std::string, int> languageMap;
    std::map<std::string, int> regionMap;

    PokedexActivityMenu();

public:
    void onActivate();

    void onDeactivate();

    void onLoop();

    void onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture);

    static PokedexActivityMenu* getInstance();
};

#endif
