#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Location.h"

PokedexActivity_PokemonView_Location PokedexActivity_PokemonView_Location::instance;

PokedexActivity_PokemonView_Location::PokedexActivity_PokemonView_Location() :
pokemon(nullptr),
routes(nullptr),
fontSurface(nullptr),
backgroundSurface(nullptr),
pokeIconSurface(nullptr),
pokeNameSurface(nullptr),
pokeType1Surface(nullptr),
pokeType2Surface(nullptr),
listEntrySurface(nullptr),
routeNameSurface(nullptr),
routeMethodSurface(nullptr),
routeRateSurface(nullptr),
routeLvlSurface(nullptr),
routeSmrySurface(nullptr),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
    fontPath = "res/font/Pokemon_GB.ttf";
    // color maps
    color = { 64, 64, 64};
    highlightColor = { 255, 0, 0 };

}

PokedexActivity_PokemonView_Location::~PokedexActivity_PokemonView_Location() {}

void PokedexActivity_PokemonView_Location::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";
    // For some reason.. pokemon needs to be created before executeSQL command...
    pokemon = new Pokemon();
    routes = pokemon->getRoutes();
    printPokeInfo();


    fontSurface = TTF_OpenFont(fontPath.c_str(), 18);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Location::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";

}

void PokedexActivity_PokemonView_Location::printPokeInfo() {
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

    for (std::vector<std::string> route : *routes) {
        for (std::string c : route) {
            std::cout << c << '|';
        }
        std::cout << '\n';
    }

}
void PokedexActivity_PokemonView_Location::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

    selectedIndex = 0, offset = 0;

    // closing font
    TTF_CloseFont(fontSurface);


    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";

}

void PokedexActivity_PokemonView_Location::onLoop() {

}

void PokedexActivity_PokemonView_Location::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Location Items
    //Render background
    std::string backgroundImageFile = "res/icons/icon/pokemon_fr_view_4.png";
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

    if (!renderPokeInfo(surf_display)) {
        std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Render _PokemonView_Location Items
    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < routes->size(); i++) {
        route = (*routes)[offset + i];

        // Render list items
        if (!renderListItems(surf_display, renderer, i)) {
            exit(EXIT_FAILURE);
        }
    }
}


bool PokedexActivity_PokemonView_Location::renderPokeInfo(SDL_Surface* surf_display) {
    //Render pokedmon info
    // render poke icon
    std::string iconPath = PokedexDB::getPokemonIdentifier();
    iconPath = "res/icons/" + iconPath + ".png";

    pokeIconSurface = PokeSurface::onLoadImg(iconPath);
    if (pokeIconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIconSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect;
    pokeIconRect.x = 0;
    pokeIconRect.y = 50;
    pokeIconRect.w = pokeIconSurface->w * 2;
    pokeIconRect.h = pokeIconSurface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);
    SDL_FreeSurface(pokeIconSurface);
    
    /////////////////////////////////////////////////////////////////////////////

    // Render Name
    std::string pokeName = pokemon->getName();
    pokeNameSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeName.c_str(),
        color
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeNameRect;
    pokeNameRect.x = (pokeIconRect.x + pokeIconRect.w);
    pokeNameRect.y = (pokeIconRect.y + pokeIconRect.h/2) - 10;
    pokeNameRect.w = pokeNameSurface->w;
    pokeNameRect.h = pokeNameSurface->h;
    PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    ////////////////////////////////////////////////////////////////////////////


    // render poke types
    std::vector<std::string> pokemonTypes = pokemon->getTypes();
    iconPath = "res/types/" + pokemonTypes[0] + ".png";
    pokeType1Surface = PokeSurface::onLoadImg(iconPath);

    if (pokeType1Surface == NULL) {
        std::cout << "Unable to load surface: pokeType1Surface Type 1" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect pokeType1Rect;
    pokeType1Rect.x = pokeIconRect.x + pokeIconRect.w;
    pokeType1Rect.y = (pokeNameRect.y + pokeNameRect.h ) + 5;
    pokeType1Rect.w = pokeType1Surface->w * 2;
    pokeType1Rect.h = pokeType1Surface->h * 2;
    PokeSurface::onDrawScaled(surf_display, pokeType1Surface, &pokeType1Rect);
    SDL_FreeSurface(pokeType1Surface);

    //List item types_2
    if (pokemonTypes[1] != "NULL") { 
        iconPath = "res/types/" + pokemonTypes[1] + ".png";
        pokeType2Surface = PokeSurface::onLoadImg(iconPath);
        if (pokeType2Surface == NULL) {
            std::cout << "Unable to render text! SDL Error: pokeType2Surface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect pokeType2Rect;
        pokeType2Rect.x = pokeType1Rect.x + pokeType1Rect.w + 5;
        pokeType2Rect.y = pokeType1Rect.y;
        pokeType2Rect.w = pokeType2Surface->w * 2;
        pokeType2Rect.h = pokeType2Surface->h * 2;

        PokeSurface::onDrawScaled(surf_display, pokeType2Surface, &pokeType2Rect);
        SDL_FreeSurface(pokeType2Surface);
    }

    return true;
}

bool PokedexActivity_PokemonView_Location::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, int i) {
    //Render list item background/surface
    listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        surf_display->w * 0.9,
        itemHeight,
        DEPTH,
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int spacing = 18; 
    SDL_Rect listEntryRect;
    listEntryRect.x = surf_display->w - (surf_display->w * 0.45) - 8;
    listEntryRect.y = 65 + (i * (itemHeight + spacing));
    listEntryRect.w = surf_display->w * 0.45;
    listEntryRect.h = itemHeight;
    if (offset + i == selectedIndex) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderDrawRect(renderer, &listEntryRect);
    }
    else {
        PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    }
    SDL_FreeSurface(listEntrySurface);

    /////////////////////////////////////////////////////////////////////////////
    //Render item title/name
    
    // make all upper case
    std::string moveName = route[1];
    for (int i = 0; i < moveName.size(); i++) {
        moveName[i] = std::toupper(moveName[i]);
    }
    routeNameSurface = TTF_RenderText_Blended(
        fontSurface,
        moveName.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (routeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveNameRect;
    moveNameRect.x = listEntryRect.x + (listEntryRect.w / 2) - (routeNameSurface->w / 2);
    moveNameRect.y = listEntryRect.y;
    moveNameRect.w = routeNameSurface->w * 0.8;
    moveNameRect.h = routeNameSurface->h * 0.8;
    PokeSurface::onDrawScaled(surf_display, routeNameSurface, &moveNameRect);
    SDL_FreeSurface(routeNameSurface);

    /////////////////////////////////////////////////////////////////////////////
    // Render method
    std::string methodString = route[2];
    routeMethodSurface = TTF_RenderText_Blended(
        fontSurface,
        methodString.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (routeMethodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeMethodSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect routeMethodRect;
    routeMethodRect.x = listEntryRect.x;
    routeMethodRect.y = (listEntryRect.y + listEntryRect.h ) - routeMethodSurface->h;
    routeMethodRect.w = routeMethodSurface->w;
    routeMethodRect.h = routeMethodSurface->h;
    PokeSurface::onDraw(surf_display, routeMethodSurface, &routeMethodRect);
    SDL_FreeSurface(routeMethodSurface);
 
    /////////////////////////////////////////////////////////////////////////////
    // Render rate
    std::string rateString =  (std::to_string((std::stoi(route[5])) / 2) + '%');
    routeRateSurface = TTF_RenderText_Blended(
        fontSurface,
        rateString.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (routeRateSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeRateSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect routeRateRect;
    routeRateRect.x = (listEntryRect.x + listEntryRect.w) - routeRateSurface->w;
    routeRateRect.y = (listEntryRect.y + listEntryRect.h ) - routeRateSurface->h;
    routeRateRect.w = routeRateSurface->w;
    routeRateRect.h = routeRateSurface->h;
    PokeSurface::onDraw(surf_display, routeRateSurface, &routeRateRect);
    SDL_FreeSurface(routeRateSurface);

    /////////////////////////////////////////////////////////////////////////////
    // Render selected Item details
    if (offset + i == selectedIndex) {
        //redern item details
        if (!renderItemDetails(surf_display, i)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;

}

bool PokedexActivity_PokemonView_Location::renderItemDetails(SDL_Surface* surf_display, int i) {
    /////////////////////////////////////////////////////////////////////////////
    // Render levels - min
    std::string lvlString = route[3];
    routeLvlSurface = TTF_RenderText_Blended(
        fontSurface,
        lvlString.c_str(),
        color
    );
    if (routeLvlSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeLvlSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect routeMinLvlRect;
    routeMinLvlRect.x = 150;
    routeMinLvlRect.y = 180;
    routeMinLvlRect.w = routeLvlSurface->w;
    routeMinLvlRect.h = routeLvlSurface->h;
    PokeSurface::onDraw(surf_display, routeLvlSurface, &routeMinLvlRect);
    SDL_FreeSurface(routeLvlSurface);
 
    /////////////////////////////////////////////////////////////////////////////
    // Render levels - max
    lvlString = route[4];
    routeLvlSurface = TTF_RenderText_Blended(
        fontSurface,
        lvlString.c_str(),
        color
    );
    if (routeLvlSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeLvlSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect routeMaxLvlRect;
    routeMaxLvlRect.x = routeMinLvlRect.x;
    routeMaxLvlRect.y = (routeMinLvlRect.y + routeMinLvlRect.h ) + 30;
    routeMaxLvlRect.w = routeLvlSurface->w;
    routeMaxLvlRect.h = routeLvlSurface->h;
    PokeSurface::onDraw(surf_display, routeLvlSurface, &routeMaxLvlRect);
    SDL_FreeSurface(routeLvlSurface);

    ////////////////////////////////////////////////////////////////////////////
    // Render 
    std::string effect = 
        route[1] + '\n' +
        route[2] + '\n' + 
        (std::to_string((std::stoi(route[5])) / 2) + '%');
    routeSmrySurface = TTF_RenderText_Blended_Wrapped(
        fontSurface,
        effect.c_str(),
        color,
        320
    );
    if (routeSmrySurface == NULL) {
        std::cout << "Unable to render text! SDL Error: routeSmrySurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect moveEffectRect;
    moveEffectRect.x = 15;
    moveEffectRect.y = (WINDOW_HEIGHT / 2 ) + 75;
    moveEffectRect.w = routeSmrySurface->w;
    moveEffectRect.h = routeSmrySurface->h;
    PokeSurface::onDraw(surf_display, routeSmrySurface, &moveEffectRect);
    SDL_FreeSurface(routeSmrySurface);

    ////////////////////////////////////////////////////////////////////////////
    return true;
}

void PokedexActivity_PokemonView_Location::onFreeze() {

}

PokedexActivity_PokemonView_Location* PokedexActivity_PokemonView_Location::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Location::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
    }
}

void PokedexActivity_PokemonView_Location::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < routes->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
    }
}


void PokedexActivity_PokemonView_Location::onButtonLeft(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Location::onButtonRight(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_EVOLUTION);
}

void PokedexActivity_PokemonView_Location::onButtonA(SDL_Keycode sym, Uint16 mod){}

void PokedexActivity_PokemonView_Location::onButtonB(SDL_Keycode sym, Uint16 mod){}

void PokedexActivity_PokemonView_Location::onButtonR(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < routes->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            // Ensure offset doesn't go out of bounds
            if (offset > routes->size() - MAX_VISIBLE_ITEMS) {
                offset = routes->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
    }
    else {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = routes->size() - 1;
        offset = routes->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivity_PokemonView_Location::onButtonL(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex >= MAX_VISIBLE_ITEMS) {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset) {
            offset -= MAX_VISIBLE_ITEMS; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0) {
                offset = 0;  // Cap offset to zero
            }
        }
    }
    else {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset = 0;  // Cap offset to zero
    }
}
