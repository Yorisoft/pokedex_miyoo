#ifndef __POKEDEXACTIVITY_POKEMONVIEW_INFO__
#define __POKEDEXACTIVITY_POKEMONVIEW_INFO__

#include "AssetManager.h"
#include "PokedexActivity.h"
#include "Pokemon.h"

class PokedexActivity_PokemonView_Info : public PokedexActivity
{
  private:
    static PokedexActivity_PokemonView_Info instance;

    const std::string SOUND_EFFECT_CRY_PATH        = "res/assets/pokemons/cry/",
                      SOUND_EFFECT_LEFT_RIGHT_PATH = "res/assets/sound_effects/left_right.wav";

    bool needRedraw;

    std::vector<std::vector<std::string>> *dbResults;

    SDL_Surface *backgroundSurface, *pokeSprite, *typeA, *typeB, *id, *nameSurface, *height,
        *weight, *gender, *genus, *flavorText;
    SDL_Rect backgroundRect, pokeSpriteRect, typeARect, typeBRect, idRect, nameRect, heightRect,
        weightRect, genderRect, genusRect, fTextRect;
    Mix_Chunk *se_poke_cry, *se_left_right;
    TTF_Font *fontSurface;

    Pokemon *pokemon;

    AssetManager *assetManager;

  private:
    PokedexActivity_PokemonView_Info();
    ~PokedexActivity_PokemonView_Info();

    void printPokeInfo();
    bool initSDL();
    bool renderSprites(SDL_Surface *surf_display);
    bool renderNameID(SDL_Surface *surf_display, TTF_Font *font);
    bool renderHW(SDL_Surface *surf_display, TTF_Font *font);
    bool renderFlavorText(SDL_Surface *surf_display, TTF_Font *font);

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

    static PokedexActivity_PokemonView_Info *getInstance();
};
#endif
