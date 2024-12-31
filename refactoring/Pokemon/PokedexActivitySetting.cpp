#include "PokedexActivitySetting.h"
#include "PokedexActivityManager.h"
//
PokedexActivitySetting PokedexActivitySetting::instance;

PokedexActivitySetting::PokedexActivitySetting() :
backgroundSurface(nullptr),
fontSurface(nullptr),
listEntrySurface(nullptr),
optionItems(nullptr),
fontPath("res/font/Pokemon_GB.ttf"),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT / 9))
{
}

PokedexActivitySetting::~PokedexActivitySetting() {
}

void PokedexActivitySetting::onActivate() {
    std::cout << "PokedexActivitySetting::onActivate START \n";
    color = { 255, 255, 255 }, highlightColor = { 255, 0, 0 };
    optionItems = new std::vector<std::string>();
    optionItems->push_back("Language");

    //// Populate the map with translations for "language"
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
        std::cerr << "PokedexActivitySetting::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::cout << "PokedexActivitySetting::onActivate END \n";
}
void PokedexActivitySetting::onDeactivate() {
    // closing font
    //TTF_CloseFont(fontSurface);

    //fontPath.clear();

    //selectedIndex = 0,
    //    offset = 0;
}
void PokedexActivitySetting::onLoop() {}
void PokedexActivitySetting::onFreeze() {}
void PokedexActivitySetting::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Location Items
    //Render background
    std::string backgroundImageFile = "res/icons/icon/settings_background.png";
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
    // MAX_ITEMS = 7
    for (int i = 0; i < 7 && offset + i < optionItems->size(); i++) {
        //evo = (*optionItems)[offset + i];
        //// Render list items
        if (!renderListItems(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivitySetting::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    //listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
    //    0,
    //    surf_display->w,
    //    itemHeight,
    //    DEPTH,
    //    SDL_PIXELFORMAT_RGBA32
    //);
    //if (!listEntrySurface) {
    //    std::cout << "Unable to render surface! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
    //    exit(EXIT_FAILURE);
    //}
    //// Define red color
    //Uint32 redColor = SDL_MapRGB(listEntrySurface->format, 255, 0, 0);

    //// Fill the surface with red
    //SDL_FillRect(listEntrySurface, nullptr, redColor);

    std::string ListackgroundImageFile = "res/icons/icon/setting_item_background.png";
    listEntrySurface = PokeSurface::onLoadImg(ListackgroundImageFile);
    if (listEntrySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    int topBorderW = 168;
    SDL_Rect listEntryRect;
    listEntryRect.x = 42;
    listEntryRect.y = (i * itemHeight) + topBorderW;
    listEntryRect.w = surf_display->w * .87;
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);
    return true;
}

//void PokedexActivitySetting::printOptionsInfo() {
//}

PokedexActivitySetting* PokedexActivitySetting::getInstance() {
    return &instance;
}

void PokedexActivitySetting::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    //if (selectedIndex > 0) {
    //    selectedIndex--;
    //    evo = (*optionItems)[selectedIndex];
    //    if (selectedIndex < offset) {
    //        offset--;
    //    }
    //}
}
void PokedexActivitySetting::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    //if (selectedIndex < optionItems->size() - 1) {
    //    selectedIndex++;
    //    evo = (*optionItems)[selectedIndex];
    //    if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
    //        offset++;
    //    }
    //}
}
void PokedexActivitySetting::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonRight(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonA(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonStart(SDL_Keycode sym, Uint16 mod) {}