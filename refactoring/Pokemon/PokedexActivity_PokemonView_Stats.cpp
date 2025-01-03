#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Stats.h"

PokedexActivity_PokemonView_Stats PokedexActivity_PokemonView_Stats::instance;

PokedexActivity_PokemonView_Stats::PokedexActivity_PokemonView_Stats() : 
pokeIDSurface(nullptr),
pokeNameSurface(nullptr),
pokeIconSurface(nullptr),
backgroundSurface(nullptr),
abilitySurface1(nullptr),
abilitySurface2(nullptr),
abilitySurfaceHidden(nullptr),
statsSurface(nullptr),
hpSurface(nullptr),
atkSurface(nullptr),
defSurface(nullptr),
spAtkSurface(nullptr),
spDefSurface(nullptr),
spdSurface(nullptr),
pokemon(nullptr),
dbResults(nullptr),
fontSurface(nullptr)
{
    fontPath = "res/font/pokemon-advanced-battle/pokemon-advanced-battle.ttf";
};

void PokedexActivity_PokemonView_Stats::onActivate() {
    // create new pokemon object
    pokemon = new Pokemon();
    std::vector<unsigned short> stats = pokemon->getBasicStats();
    
    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Deffense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';

}

void PokedexActivity_PokemonView_Stats::onDeactivate() {
    delete pokemon;
}

void PokedexActivity_PokemonView_Stats::onLoop() {
}

void PokedexActivity_PokemonView_Stats::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivityMenu::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render item info
    // Render background
    std::string backgroundImageFile = "res/icons/icon/pokemon_fr_view_2.png";
    backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    if (!renderSprites(surf_display)) {
        std::cout << "Unable to render item sprites! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!renderNameID(surf_display, font)) {
        std::cout << "Unable to render item name and ID! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!renderStats(surf_display, font)) {
        std::cout << "Unable to render item stats! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!renderFlavorText(surf_display)) {
        std::cout << "Unable to render item flavor text! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool PokedexActivity_PokemonView_Stats::renderSprites(SDL_Surface* surf_display) {
    // Render Item sprites
    std::string pokeName = PokedexDB::getPokemonIdentifier();
    std::string spritePath = "res/sprites/" + pokeName + ".png";
    pokeIconSurface = PokeSurface::onLoadImg(spritePath);
    if (pokeIconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIconSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect;   
    pokeIconRect.x = 50;
    pokeIconRect.y = 100;   
    pokeIconRect.w = pokeIconSurface->w * 2;
    pokeIconRect.h = pokeIconSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);
    SDL_FreeSurface(pokeIconSurface);

    return true;
}

bool PokedexActivity_PokemonView_Stats::renderNameID(SDL_Surface* surf_display, TTF_Font* font){
    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
    std::string pokeID = formattedID.str();

    pokeIDSurface = TTF_RenderUTF8_Blended(
        font,
        pokeID.c_str(),
        { 96, 96, 96 }
    );
    if (pokeIDSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIDSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIDRect;
    pokeIDRect.x = 10;
    pokeIDRect.y = 60;
    pokeIDRect.w = pokeIDSurface->w;
    pokeIDRect.h = pokeIDSurface->h;
    PokeSurface::onDraw(surf_display, pokeIDSurface, &pokeIDRect);
    SDL_FreeSurface(pokeIDSurface);

    // Render Item ID
    std::string pokeName = pokemon->getName();
    pokeNameSurface = TTF_RenderUTF8_Blended(
        font,
        pokeName.c_str(),
        { 96, 96, 96 }
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect;
    pokeNameRect.x = pokeIDRect.x + pokeIDRect.w + 10;
    pokeNameRect.y = pokeIDRect.y;
    pokeNameRect.w = pokeNameSurface->w;
    pokeNameRect.h = pokeNameSurface->h;

    PokeSurface::onDrawScaled(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    return true;
}

bool PokedexActivity_PokemonView_Stats::renderStats(SDL_Surface* surf_display, TTF_Font* font) {
    std::stringstream iss;
    std::vector<unsigned short> stats = pokemon->getBasicStats();

    for (int i = 0; i < stats.size(); ++i) {
        std::string statsToString = std::to_string(stats[i]);

        statsSurface = TTF_RenderUTF8_Blended(
            font,
            statsToString.c_str(),
            { 96, 96, 96 }
        );

        SDL_Rect statsRect;
        statsRect.x = (WINDOW_WIDTH - statsSurface->w) - 60;
        statsRect.y = (38 * i) + 80;
        statsRect.w = statsSurface->w;
        statsRect.h = statsSurface->h;

        PokeSurface::onDrawScaled(surf_display, statsSurface, &statsRect);
        SDL_FreeSurface(statsSurface);

    }

    return true;
}

bool PokedexActivity_PokemonView_Stats::renderFlavorText(SDL_Surface* surf_display) {
    return true;
}

void PokedexActivity_PokemonView_Stats::onFreeze(){
}

PokedexActivity_PokemonView_Stats* PokedexActivity_PokemonView_Stats::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Stats::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonLeft(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_MOVES);
}
void PokedexActivity_PokemonView_Stats::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonB(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Stats::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
