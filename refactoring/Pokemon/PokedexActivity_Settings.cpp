#include "PokedexActivityManager.h"
#include "PokedexActivity_Settings.h"

PokedexActivity_Settings PokedexActivity_Settings::instance;

PokedexActivity_Settings::PokedexActivity_Settings() :
backgroundSurface(nullptr),
fontPath("res/font/Pokemon_GB.ttf"),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>((WINDOW_HEIGHT / 3) * 0.7))
{
}

PokedexActivity_Settings::~PokedexActivity_Settings() {
}

void PokedexActivity_Settings::onActivate() {
    std::cout << "PokedexActivity_Settings::onActivate START \n";
    // Populate the map with translations for "language"
    language_map[1] = "言語";         // Japanese
    language_map[3] = "언어";         // Korean
    language_map[5] = "Langue";      // French
    language_map[6] = "Sprache";     // German
    language_map[7] = "Lenguaje";    // Spanish
    language_map[8] = "Lingua";      // Italian
    language_map[9] = "Language";    // English
    language_map[10] = "Jazyk";      // Czech

    fontSurface = TTF_OpenFont(fontPath.c_str(), 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_Settings::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::cout << "PokedexActivity_Settings::onActivate END \n";
}
void PokedexActivity_Settings::printOptionsInfo() {
}
void PokedexActivity_Settings::onDeactivate() {
    // closing font
    TTF_CloseFont(fontSurface);

    fontPath.clear();

    selectedIndex = 0,
        offset = 0,
        itemHeight = static_cast<int>((WINDOW_HEIGHT / 3) * 0.7);
}
void PokedexActivity_Settings::onLoop() {}
void PokedexActivity_Settings::onFreeze() {}
void PokedexActivity_Settings::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Location Items
    //Render background
    std::string backgroundImageFile = "res/icons/icon/pokemon_fr_view_5.png";
    backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    //// Render List Items
    //for (int i = 0; i < 3 && offset + i < evoChain->size(); i++) {
    //    if (i > 0 && (*evoChain)[offset + i][1] == (*evoChain)[offset + i - 1][1] || std::stoi((*evoChain)[offset + i][1]) >= 1000)
    //        continue;
    //    evo = (*evoChain)[offset + i];
    //    // Render list items
    //    if (!renderListItems(surf_display, i)) {
    //        exit(EXIT_FAILURE);
    //    }
    //}

}

bool PokedexActivity_Settings::renderListItems(SDL_Surface* surf_display, int i) {
    return true;
}

void PokedexActivity_Settings::printOptionsInfo() {
}

PokedexActivity_Settings* PokedexActivity_Settings::getInstance() {
    return &instance;
}

void PokedexActivity_Settings::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    //if (selectedIndex > 0) {
    //    selectedIndex--;
    //    evo = (*evoChain)[selectedIndex];
    //    if (selectedIndex < offset) {
    //        offset--;
    //    }
    //}
}
void PokedexActivity_Settings::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    //if (selectedIndex < evoChain->size() - 1) {
    //    selectedIndex++;
    //    evo = (*evoChain)[selectedIndex];
    //    if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
    //        offset++;
    //    }
    //}
}
void PokedexActivity_Settings::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivity_Settings::onButtonRight(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivity_Settings::onButtonA(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}
void PokedexActivity_Settings::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}
void PokedexActivity_Settings::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivity_Settings::onButtonStart(SDL_Keycode sym, Uint16 mod) {}
