#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity_PokemonView_Evolution PokedexActivity_PokemonView_Evolution::instance;

PokedexActivity_PokemonView_Evolution::PokedexActivity_PokemonView_Evolution() :
pokemon(nullptr),
backgroundSurface(nullptr),
pokeSpriteSurface(nullptr),
pokeIDSurface(nullptr),
pokeNameSurface(nullptr),
pokeMethodSurface(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>((WINDOW_HEIGHT / 3) * 0.7))
{
    fontPath = "res/font/Pokemon_GB.ttf";
}

PokedexActivity_PokemonView_Evolution::~PokedexActivity_PokemonView_Evolution() {
}


void PokedexActivity_PokemonView_Evolution::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate START \n";

    pokemon = new Pokemon();
	evoChain = pokemon->getEvolutionChain();
    printPokeInfo();
    evo = (*evoChain)[selectedIndex];

    fontSurface = TTF_OpenFont(fontPath.c_str(), 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Evolution::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate END \n";
}

void PokedexActivity_PokemonView_Evolution::printPokeInfo() {
    std::vector<unsigned short> stats = pokemon->getBasicStats();
    
    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';
    std::cout << "Evolution Chain ID: " << pokemon->getEvolutionChainID() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Deffense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';

    for (std::vector<std::string> evol : *evoChain) {
        for (auto& c : evol) {
            std::cout << c << '|';
        }
        std::cout << std::endl;
    }

}
void PokedexActivity_PokemonView_Evolution::onDeactivate() {
    selectedIndex = 0, offset = 0;
    // closing font
    TTF_CloseFont(fontSurface);
}
void PokedexActivity_PokemonView_Evolution::onLoop() {}
void PokedexActivity_PokemonView_Evolution::onFreeze() {}
void PokedexActivity_PokemonView_Evolution::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
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
    for (int i = 0; i < 3 && static_cast<std::size_t>(offset + i) < evoChain->size(); i++) {
        if (i > 0 && (*evoChain)[offset + i][1] == (*evoChain)[offset + i - 1][1] || std::stoi((*evoChain)[offset + i][1]) >= 649)
            continue;
        evo = (*evoChain)[offset + i];
        // Render list items
        if (!renderListItems(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivity_PokemonView_Evolution::renderListItems(SDL_Surface* surf_display, int i) {
    std::string listEntryImageFile = "res/icons/icon/evolution_item_background_";
    offset + i == selectedIndex ? listEntryImageFile.append("selected.png") : listEntryImageFile.append("default.png");
    listEntrySurface = PokeSurface::onLoadImg(listEntryImageFile);
    if (listEntrySurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    int spacing = 15; 
    SDL_Rect listEntryRect;
    listEntryRect.x = static_cast<int>(surf_display->w - (surf_display->w * 0.5));
    listEntryRect.y = 65 + (i * (itemHeight + spacing));
    listEntryRect.w = static_cast<int>(surf_display->w * 0.50);
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

    /////////////////////////////////////////////////////////////////////////////
    // render poke sprite
    std::string spritePath = evo[2];
    spritePath = "res/sprites/" + spritePath + ".png";

    pokeSpriteSurface = PokeSurface::onLoadImg(spritePath);
    if (pokeSpriteSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeSpriteSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeSpriteRect;
    pokeSpriteRect.x = static_cast<int>((listEntryRect.x + listEntryRect.w ) - pokeSpriteSurface->w * 1.3);
    pokeSpriteRect.y = listEntryRect.y + 10;
    pokeSpriteRect.w = pokeSpriteSurface->w;
    pokeSpriteRect.h = pokeSpriteSurface->h;

    PokeSurface::onDrawScaled(surf_display, pokeSpriteSurface, &pokeSpriteRect);
    SDL_FreeSurface(pokeSpriteSurface);

    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << evo[1];
    std::string pokeID = formattedID.str();

    pokeIDSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeID.c_str(),
        { 96, 96, 96 }
    );
    if (pokeIDSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIDSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIDRect;
    pokeIDRect.x = listEntryRect.x + 60;
    pokeIDRect.y = listEntryRect.y + 30;
    pokeIDRect.w = static_cast<int>(pokeIDSurface->w * .8);
    pokeIDRect.h = static_cast<int>(pokeIDSurface->h * .8);
    PokeSurface::onDrawScaled(surf_display, pokeIDSurface, &pokeIDRect);
    SDL_FreeSurface(pokeIDSurface);

    /////////////////////////////////////////////////////////////////////////////
    //// Render poke name
    std::string pokeName = evo[4];
    pokeNameSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeName.c_str(),
        { 96, 96, 96 }
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect;
    pokeNameRect.x = listEntryRect.x + 10;
    pokeNameRect.y = pokeIDRect.y + pokeIDRect.h + 5;
    pokeNameRect.w = static_cast<int>(pokeNameSurface->w * 0.8);
    pokeNameRect.h = static_cast<int>(pokeNameSurface->h * 0.8);

    PokeSurface::onDrawScaled(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);


    if (offset + i == selectedIndex) {
        if (!renderPokeInfo(surf_display)) {
            std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }


    ///////////////////////////////////////////////////////////////////////////////
    ////// Render poke method
    //std::string pokeMethod = evo[5];
    //if (pokeMethod == "level-up") {
    //    pokeMethod = "Lv. " + evo[6];

    //}
    //pokeMethodSurface = TTF_RenderText_Blended(
    //    fontSurface,
    //    pokeMethod.c_str(),
    //    { 96, 96, 96 }
    //);
    //if (pokeMethodSurface == NULL) {
    //    std::cout << "Unable to render text! SDL Error: pokeMethodSurface " << TTF_GetError() << std::endl;
    //    exit(EXIT_FAILURE);
    //};

    //SDL_Rect pokeMethodRect;
    //pokeMethodRect.x = pokeNameRect.x;
    //pokeMethodRect.y = pokeNameRect.y + pokeNameRect.h + 30;
    //pokeMethodRect.w = pokeMethodSurface->w * 0.8;
    //pokeMethodRect.h = pokeMethodSurface->h * 0.8;

    //PokeSurface::onDraw(surf_display, pokeMethodSurface, &pokeMethodRect);
    //SDL_FreeSurface(pokeMethodSurface);

    return true;
}

bool PokedexActivity_PokemonView_Evolution::renderPokeInfo(SDL_Surface* surf_display) {
    //Render pokedmon info
    // render poke sprite
    std::string spritePath = evo[2];
    spritePath = "res/sprites/" + spritePath + ".png";

    pokeSpriteSurface = PokeSurface::onLoadImg(spritePath);
    if (pokeSpriteSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeSpriteSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeSpriteRect;
    pokeSpriteRect.x = 50;
    pokeSpriteRect.y = 100;
    pokeSpriteRect.w = pokeSpriteSurface->w * 2;
    pokeSpriteRect.h = pokeSpriteSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeSpriteSurface, &pokeSpriteRect);
    SDL_FreeSurface(pokeSpriteSurface);
    
    /////////////////////////////////////////////////////////////////////////////

    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << evo[1];
    std::string pokeID = formattedID.str();

    pokeIDSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeID.c_str(),
        { 96, 96, 96 }
    );
    if (pokeIDSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIDSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIDRect;
    pokeIDRect.x = 50;
    pokeIDRect.y = WINDOW_HEIGHT/2 + 60;
    pokeIDRect.w = pokeIDSurface->w;
    pokeIDRect.h = pokeIDSurface->h;
    PokeSurface::onDraw(surf_display, pokeIDSurface, &pokeIDRect);
    SDL_FreeSurface(pokeIDSurface);

    /////////////////////////////////////////////////////////////////////////////
    //// Render poke name
    std::string pokeName = evo[4];
    pokeNameSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeName.c_str(),
        { 96, 96, 96 }
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect;
    pokeNameRect.x = pokeIDRect.x;
    pokeNameRect.y = pokeIDRect.y + pokeIDRect.h + 10;
    pokeNameRect.w = static_cast<int>(pokeNameSurface->w * 0.8);
    pokeNameRect.h = static_cast<int>(pokeNameSurface->h * 0.8);

    PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    /////////////////////////////////////////////////////////////////////////////
    //// Render poke method
    std::string pokeMethod = evo[5];
    if (pokeMethod == "level-up") {
        pokeMethod = "Lv. " + evo[6];

    }
    if (pokeMethod == "use-item") {
        pokeMethod = evo[7];

    }
    pokeMethodSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeMethod.c_str(),
        { 96, 96, 96 }
    );
    if (pokeMethodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeMethodSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeMethodRect;
    pokeMethodRect.x = pokeNameRect.x;
    pokeMethodRect.y = pokeNameRect.y + pokeNameRect.h + 30;
    pokeMethodRect.w = static_cast<int>(pokeMethodSurface->w * 0.8);
    pokeMethodRect.h = static_cast<int>(pokeMethodSurface->h * 0.8);

    PokeSurface::onDraw(surf_display, pokeMethodSurface, &pokeMethodRect);
    SDL_FreeSurface(pokeMethodSurface);

    
    return true;
}

PokedexActivity_PokemonView_Evolution* PokedexActivity_PokemonView_Evolution::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Evolution::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex < offset) {
            offset--;
        }
    }
}
void PokedexActivity_PokemonView_Evolution::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < evoChain->size() - 1) {
        selectedIndex++;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}
void PokedexActivity_PokemonView_Evolution::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}
void PokedexActivity_PokemonView_Evolution::onButtonRight(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivity_PokemonView_Evolution::onButtonA(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivity_PokemonView_Evolution::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Evolution::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
