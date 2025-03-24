#ifndef __POKEDEXACTIVITY_POKEMONVIEW_LOCATION__
#define __POKEDEXACTIVITY_POKEMONVIEW_LOCATION__

#include "AssetManager.hpp"
#include "PokedexActivity.hpp"
#include "Pokemon.hpp"

class PokedexActivity_PokemonView_Location : public PokedexActivity
{
  private:
    static PokedexActivity_PokemonView_Location instance;

    const int ITEM_HEIGHT = static_cast<int>(WINDOW_HEIGHT * 0.7 / 5);

    const std::string SOUND_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav",
                      SOUND_UP_DOWN_PATH    = "res/assets/sound_effects/up_down.wav",
                      FONT_PATH             = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf",
                      BACKGROUND_IMG_PATH   = "res/assets/misc/pokemon_fr_view_4.png",
                      TYPE_IMG_BASE_PATH    = "res/assets/pokemons/types/",
                      METHOD_IMG_BASE_PATH  = "res/assets/pokemons/encounters/",
                      ICON_IMG_BASE_PATH    = "res/assets/pokemons/icons/";

    const SDL_Color COLOR = {64, 64, 64}, HIGHLIGHT_COLOR{255, 0, 0};

    int selectedIndex, offset;

    bool needRedraw;

    std::vector<std::vector<std::string>> *dbResults;
    std::vector<std::vector<std::string>> *routes;
    std::vector<std::string> route;

    std::vector<SDL_Surface *> locationNameSurface_cache;
    std::vector<SDL_Surface *> conditionSurface_cache;
    std::vector<SDL_Surface *> methodSurface_cache;
    std::vector<SDL_Surface *> rateSurface_cache;
    std::vector<std::pair<SDL_Surface *, SDL_Surface *>> levelSurface_cache;
    std::vector<SDL_Surface *> detailLocationNameSurface_cache;

    SDL_Surface *backgroundSurface, *listEntrySurface, *pokeIcon, *pokeName, *typeA, *typeB;
    SDL_Rect backgroundRect, listEntryRect, pokeIconRect, pokeNameRect, typeARect, typeBRect,
        locationNameRect, methodRect, conditionRect, rateRect, minLevelRect, maxLevelRect,
        detailLocationRect;
    Mix_Chunk *se_left_right, *se_up_down;
    TTF_Font *fontSurface;

    Pokemon *pokemon;

    AssetManager *assetManager;

  private:
    bool initSDL();
    void printPokeInfo();

    bool renderListItems(SDL_Surface *surf_display, SDL_Renderer *renderer, TTF_Font *font, int i);
    bool renderItemDetails(SDL_Surface *surf_display, TTF_Font *font, int i);

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
    PokedexActivity_PokemonView_Location();
    ~PokedexActivity_PokemonView_Location();

    void onActivate();
    void onDeactivate();
    void onLoop();
    void onRender(SDL_Surface *surf_display, SDL_Renderer *renderer, SDL_Texture *texture,
                  TTF_Font *font, Mix_Chunk *sEffect);
    void onFreeze();

    static PokedexActivity_PokemonView_Location *getInstance();
};
#endif