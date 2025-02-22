#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Location.h"

PokedexActivity_PokemonView_Location PokedexActivity_PokemonView_Location::instance;

PokedexActivity_PokemonView_Location::PokedexActivity_PokemonView_Location() :
pokemon(nullptr),
routes(nullptr),
selectedIndex(0),
offset(0),
color ({ 64, 64, 64}), 
highlightColor({ 255, 0, 0 }),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
}

PokedexActivity_PokemonView_Location::~PokedexActivity_PokemonView_Location() {}

void PokedexActivity_PokemonView_Location::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";


    // For some reason.. pokemon needs to be created before executeSQL command...
    pokemon = new Pokemon();
    routes = pokemon->getRoutes();
    printPokeInfo();

    std::string sEffectPath = "res/assets/sound_effects/left_right.wav"; // <- empty char is standin for form variant
    sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    sEffect_UpDown = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
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

    delete pokemon;
    pokemon = nullptr;

    //delete routes;
    //routes = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";
}

void PokedexActivity_PokemonView_Location::onLoop() {
}

void PokedexActivity_PokemonView_Location::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Location Items
    //Render background
    std::string backgroundImageFile = "res/assets/misc/pokemon_fr_view_4.png";
    SDL_Surface* backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect = {
        0, 0,
        surf_display->w, surf_display->h
    };
    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    if (!renderPokeInfo(surf_display, font)) {
        std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Render _PokemonView_Location Items
    for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < routes->size(); i++) {
        route = (*routes)[offset + i];

        // Render list items
        if (!renderListItems(surf_display, renderer, font, i)) {
            exit(EXIT_FAILURE);
        }
    }
}

bool PokedexActivity_PokemonView_Location::renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font) {
    // Render icon
    std::string iconPath = PokedexDB::getPokemonIdentifier();
    iconPath = "res/assets/pokemons/icons/" + iconPath + ".png";

    SDL_Surface* iconSurface = PokeSurface::onLoadImg(iconPath);
    if (iconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: iconSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeIconRect = {
        0, 55,
        iconSurface->w * 2, iconSurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, iconSurface, &pokeIconRect);
    SDL_FreeSurface(iconSurface);
    
    // Render Name
    std::string location= pokemon->getName();
    SDL_Surface* locationSurface = TTF_RenderUTF8_Blended(
        font,
        location.c_str(),
        color
    );
    if (locationSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: locationSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect locationRect = {
        (pokeIconRect.x + pokeIconRect.w), pokeIconRect.y + 10,
        locationSurface->w, locationSurface->h
    };
    PokeSurface::onDraw(surf_display, locationSurface, &locationRect);
    SDL_FreeSurface(locationSurface);

    // render poke types
    std::vector<std::string> types = pokemon->getTypes();
    iconPath = "res/assets/pokemons/types/" + types[0] + ".png";

    SDL_Surface* typeASurface = PokeSurface::onLoadImg(iconPath);
    if (typeASurface == NULL) {
        std::cout << "Unable to load surface: typeASurface Type 1" << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect typeARect = {
        pokeIconRect.x + pokeIconRect.w, (locationRect.y + locationRect.h ) + 5,
        typeASurface->w * 2, typeASurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, typeASurface, &typeARect);
    SDL_FreeSurface(typeASurface);

    //List item types_2
    if (types[1] != "NULL") { 
        iconPath = "res/assets/pokemons/types/" + types[1] + ".png";
        SDL_Surface* typeBSurface = PokeSurface::onLoadImg(iconPath);
        if (typeBSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: typeBSurface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect typeBRect = {
            typeARect.x + typeARect.w + 5, typeBRect.y = typeARect.y,
            typeBSurface->w * 2, typeBRect.h = typeBSurface->h * 2
        };
        PokeSurface::onDrawScaled(surf_display, typeBSurface, &typeBRect);
        SDL_FreeSurface(typeBSurface);
    }

    return true;
}

bool PokedexActivity_PokemonView_Location::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    //Render list item background/surface
    SDL_Surface* listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        static_cast<int>(surf_display->w * 0.9),
        itemHeight,
        DEPTH,
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int spacing = 18; 
    SDL_Rect listEntryRect = {
        static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8), 
        65 + (i * (itemHeight + spacing)), 
        static_cast<int>(surf_display->w * 0.45), 
        itemHeight
    };
    if (offset + i == selectedIndex) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderDrawRect(renderer, &listEntryRect);
    }
    else {
        PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    }
    SDL_FreeSurface(listEntrySurface);

    //Render name
    // make all upper case
    std::string location= route[1];
    for (int i = 0; i < location.size(); i++) {
        location[i] = std::toupper(location[i]);
    }
    SDL_Surface* locationSurface = TTF_RenderUTF8_Blended(
        font,
        location.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (locationSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: locationSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect locationRect = {
        listEntryRect.x + (listEntryRect.w / 2) - (locationSurface->w / 2), 
        listEntryRect.y, 
        static_cast<int>(locationSurface->w), 
        static_cast<int>(locationSurface->h)
    };
    PokeSurface::onDrawScaled(surf_display, locationSurface, &locationRect);
    SDL_FreeSurface(locationSurface);

    //Render  condition
    if (route[7] != "NULL") {
        std::string condition = route[7];
        std::string iconPath = "res/assets/encounters/" + condition + ".png";
        SDL_Surface* conditionSurface = PokeSurface::onLoadImg(iconPath);
        if (conditionSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: conditionSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        double scaling = 1.5;
        SDL_Rect conditionRect = {
            listEntryRect.x, 
            listEntryRect.y, 
            static_cast<int>(conditionSurface->w * scaling), 
            static_cast<int>(conditionSurface->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface, &conditionRect);
        SDL_FreeSurface(conditionSurface);
    }

    // Render method
    std::string method = route[2];
    method = method == "walk" ? "grass" : method;
    method = method == "gift-egg" ? "egg" : method;
    method = method == "surf" ? "water" : method;
    std::string iconPath = "res/assets/encounters/" + method + ".png";

    SDL_Surface* methodSurface = PokeSurface::onLoadImg(iconPath);
    if (methodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: methodSurface  " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    double scaling = 1.5;
    SDL_Rect methodRect = {
            listEntryRect.x, 
            (listEntryRect.y + listEntryRect.h ) - static_cast<int>(methodSurface->h * scaling), 
            static_cast<int>(methodSurface->w * scaling),
            static_cast<int>(methodSurface->h * scaling)
        };
    PokeSurface::onDrawScaled(surf_display, methodSurface, &methodRect);
    SDL_FreeSurface(methodSurface);
 
    // Render rate
    std::string rate =  route[5] + '%';
    SDL_Surface* rateSurface = TTF_RenderUTF8_Blended(
        font,
        rate.c_str(),
        offset + i == selectedIndex ? highlightColor : color
    );
    if (rateSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: rateSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect routeRateRect{
        (listEntryRect.x + listEntryRect.w) - rateSurface->w, 
        (listEntryRect.y + listEntryRect.h ) - rateSurface->h, 
        rateSurface->w, 
        rateSurface->h
    };
    PokeSurface::onDraw(surf_display, rateSurface, &routeRateRect);
    SDL_FreeSurface(rateSurface);

    // Render selected Item details
    if (offset + i == selectedIndex) {
        if (!renderItemDetails(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

bool PokedexActivity_PokemonView_Location::renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // Render levels - min
    std::string level = route[3];
    SDL_Surface* levelSurface = TTF_RenderUTF8_Blended(
        font,
        level.c_str(),
        color
    );
    if (levelSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: levelSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect minLevelRect = {
        155, 175, 
        levelSurface->w, levelSurface->h
    };
    PokeSurface::onDraw(surf_display, levelSurface, &minLevelRect);
    SDL_FreeSurface(levelSurface);
 
    // Render levels - max
    level = route[4];
    levelSurface = TTF_RenderUTF8_Blended(
        font,
        level.c_str(),
        color
    );
    if (levelSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: levelSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect maxLevelRect = {
        minLevelRect.x, (minLevelRect.y + minLevelRect.h ) + 10,
        levelSurface->w, levelSurface->h
    };
    PokeSurface::onDraw(surf_display, levelSurface, &maxLevelRect);
    SDL_FreeSurface(levelSurface);

    // Render location
    std::string location = route[1];
    std::string subLocation;
    if ((subLocation = route[6]) != "NULL") {
        location += '\n' + subLocation;
    }
    SDL_Surface* locationSurface = TTF_RenderUTF8_Blended_Wrapped(
        font,
        location.c_str(),
        offset + i == selectedIndex ? highlightColor : color,
        295
    );
    if (locationSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: locationSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect locationRect = {
        15, (WINDOW_HEIGHT / 2 ) + 60,
        locationSurface->w, locationSurface->h
    };
    PokeSurface::onDraw(surf_display, locationSurface, &locationRect);
    SDL_FreeSurface(locationSurface);

    // Render method
    std::string method = route[2];
    method = method == "walk" ? "grass" : method;
    method = method == "gift-egg" ? "egg" : method;
    method = method == "surf" ? "water" : method;
    std::string methodIconPath = "res/assets/encounters/" + method + ".png";

    SDL_Surface* methodSurface = PokeSurface::onLoadImg(methodIconPath);
    if (methodSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: methodSurface  " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    double scaling = 1.5;
    int bottomBorder = 37, border = 10;
    SDL_Rect methodRect = {
        locationRect.x, 
        WINDOW_HEIGHT - methodSurface->h - bottomBorder,
        static_cast<int>(methodSurface->w * scaling), 
        static_cast<int>(methodSurface->h * scaling)
    };
    PokeSurface::onDrawScaled(surf_display, methodSurface, &methodRect);
    SDL_FreeSurface(methodSurface);

    // Render item condition
    if (route[7] != "NULL") {
        std::string condition = route[7];
        std::string conditionIconPath = "res/assets/encounters/" + condition + ".png";
        SDL_Surface* conditionSurface = PokeSurface::onLoadImg(conditionIconPath);
        if (conditionSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: conditionSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect conditionRect = {
            methodRect.x + methodRect.w + border, 
            WINDOW_HEIGHT - conditionSurface->h - bottomBorder,
            static_cast<int>(conditionSurface->w * scaling), 
            static_cast<int>(conditionSurface->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface, &conditionRect);
        SDL_FreeSurface(conditionSurface);
    }
 
    // Render rate 
    std::string rate = route[5] + '%';
    SDL_Surface* rateSurface = TTF_RenderUTF8_Blended_Wrapped(
        font,
        rate.c_str(),
        offset + i == selectedIndex ? highlightColor : color,
        320
    );
    if (rateSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: rateSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect rateRect = {
        250, 
        WINDOW_HEIGHT - rateSurface->h - 20,
        rateSurface->w,
        rateSurface->h
    };
    PokeSurface::onDraw(surf_display, rateSurface, &rateRect);
    SDL_FreeSurface(rateSurface);

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
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Location::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < routes->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Location::onButtonLeft(SDL_Keycode sym, Uint16 mod){
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Location::onButtonRight(SDL_Keycode sym, Uint16 mod){
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_EVOLUTION);
}

void PokedexActivity_PokemonView_Location::onButtonA(SDL_Keycode sym, Uint16 mod){}

void PokedexActivity_PokemonView_Location::onButtonB(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Location::onButtonR(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < routes->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > routes->size() - MAX_VISIBLE_ITEMS) {
                offset = routes->size() - MAX_VISIBLE_ITEMS; 
            }
        }
    }
    else {
        selectedIndex = routes->size() - 1;
        offset = routes->size() - MAX_VISIBLE_ITEMS; 
    }
}

void PokedexActivity_PokemonView_Location::onButtonL(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex >= MAX_VISIBLE_ITEMS) {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset) {
            offset -= MAX_VISIBLE_ITEMS; 
            if (offset < 0) {
                offset = 0;  
            }
        }
    }
    else {
        selectedIndex = 0; 
        offset = 0;  
    }
}

void PokedexActivity_PokemonView_Location::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Location::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}