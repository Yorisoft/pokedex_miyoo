#ifndef __POKEDEXACTIVITYMENU_H__
#define __POKEDEXACTIVITYMENU_H__

#include "AssetManager.hpp"
#include "PokedexActivity.hpp"

class PokedexActivityMenu : public PokedexActivity
{
  private:
    static PokedexActivityMenu instance;

    const int ITEM_HEIGHT = (static_cast<int>(WINDOW_HEIGHT / 5));

    const std::string SOUND_UP_DOWN_PATH = "res/assets/sound_effects/up_down.wav";

    const SDL_Color COLOR = {248, 248, 248}, HIGHLIGHT_COLOR = {255, 0, 0};

    int selectedIndex, offset;

    bool needRedraw;

    std::vector<std::string> game;
    std::vector<std::vector<std::string>> *dbResults;

    SDL_Surface *backgroundSurface, *listEntrySurface_default, *listEntrySurface_selected;
    SDL_Rect backgroundRect, listEntryRect, versionRect;
    TTF_Font *fontSurface;
    Mix_Chunk *se_up_down;

    AssetManager *assetManager;

  private:
    PokedexActivityMenu();
    ~PokedexActivityMenu();

    bool initSDL();
    void print_dbResults();
    bool renderListItems(SDL_Surface *surf_display, int i);

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

  public:
    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface *surf_display, SDL_Renderer *renderer, SDL_Texture *texture,
                  TTF_Font *font, Mix_Chunk *sEffect);
    void onFreeze();
    static PokedexActivityMenu *getInstance();
};

#endif
