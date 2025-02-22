#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Evolution.h"

PokedexActivity_PokemonView_Evolution PokedexActivity_PokemonView_Evolution::instance;

PokedexActivity_PokemonView_Evolution::PokedexActivity_PokemonView_Evolution() :
pokemon(nullptr),
sEffect(nullptr),
color({ 64, 64, 64 }),
highlightColor({ 255, 0, 0 }),
selectedIndex(0),
offset(0),
itemHeight(static_cast<int>((WINDOW_HEIGHT / 3) * 0.7))
{
}

PokedexActivity_PokemonView_Evolution::~PokedexActivity_PokemonView_Evolution() {
}

void PokedexActivity_PokemonView_Evolution::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate START \n";

    pokemon = new Pokemon();
	evoChain = pokemon->getEvolutionChain();
    printPokeInfo();
    evo = (*evoChain)[selectedIndex];

    std::string sEffectPath = "res/assets/sound_effects/left_right.wav"; // <- empty char is standin for form variant
    sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

    sEffect_UpDown = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
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
    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;
}

void PokedexActivity_PokemonView_Evolution::onLoop() {}

void PokedexActivity_PokemonView_Evolution::onFreeze() {}

void PokedexActivity_PokemonView_Evolution::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    //Render background
    std::string backgroundImageFile = "res/assets/misc/pokemon_fr_view_5.png";
    SDL_Surface* backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect = {
        0,
        0,
        surf_display->w,
        surf_display->h
    };
    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    for (int i = 0; i < 3 && static_cast<std::size_t>(offset + i) < evoChain->size(); i++) {
        if (i > 0 && (*evoChain)[offset + i][1] == (*evoChain)[offset + i - 1][1]){ // < -- skip forms. for now..
            if (selectedIndex == evoChain->size() - 1) {
                selectedIndex--;
                if (selectedIndex - offset >= 3) {
                    offset--;
                }
            }
            continue;
        }

        // Render list items
        if (!renderListItems(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }
}

bool PokedexActivity_PokemonView_Evolution::renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i) {
    std::string listEntryImageFile = "res/assets/misc/evolution_item_background_";
    offset + i == selectedIndex ? listEntryImageFile.append("selected.png") : listEntryImageFile.append("default.png");

    SDL_Surface* listEntrySurface = PokeSurface::onLoadImg(listEntryImageFile);
    if (listEntrySurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    int spacing = 15; 
    SDL_Rect listEntryRect = {
        static_cast<int>(surf_display->w - (surf_display->w * 0.5)),
        65 + (i * (itemHeight + spacing)),
        static_cast<int>(surf_display->w * 0.50),
        itemHeight
    };
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

    // render poke sprite
    std::string poke = (*evoChain)[offset + i][2];
    poke = "res/assets/pokemons/sprites/" + poke + ".png";

    SDL_Surface* pokeSurface = PokeSurface::onLoadImg(poke);
    if (pokeSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeRect = {
        static_cast<int>((listEntryRect.x + listEntryRect.w ) - pokeSurface->w * 1.3),
        listEntryRect.y + 10,
        pokeSurface->w,
        pokeSurface->h
    };
    PokeSurface::onDrawScaled(surf_display, pokeSurface, &pokeRect);
    SDL_FreeSurface(pokeSurface);

    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << (*evoChain)[offset + i][1];
    std::string id = formattedID.str();

    SDL_Surface* idSurface = TTF_RenderUTF8_Blended(
        font,
        id.c_str(),
        { 96, 96, 96 }
    );
    if (idSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: idSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect idRect = {
        listEntryRect.x + 60,
        listEntryRect.y + 20,
        static_cast<int>(idSurface->w * .8),
        static_cast<int>(idSurface->h * .8)
    };
    PokeSurface::onDrawScaled(surf_display, idSurface, &idRect);
    SDL_FreeSurface(idSurface);

    //// Render poke name( level or item)
    std::string name = (*evoChain)[offset + i][3];
    SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(
        font,
        name.c_str(),
        { 96, 96, 96 }
    );
    if (nameSurface == NULL ) {
        std::cout << "Unable to render text! SDL Error: nameSurface" << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect nameRect = {
        listEntryRect.x + 10,
        idRect.y + idRect.h + 20,
        static_cast<int>(nameSurface->w * 0.8),
        static_cast<int>(nameSurface->h * 0.8)
    };
    PokeSurface::onDrawScaled(surf_display, nameSurface, &nameRect);
    SDL_FreeSurface(nameSurface);

    if (offset + i == selectedIndex) {
        if (!renderPokeInfo(surf_display, font, i)) {
            std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return true;
}

bool PokedexActivity_PokemonView_Evolution::renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // render poke sprite
    std::string poke = (*evoChain)[offset + i][2];
    poke = "res/assets/pokemons/sprites/" + poke + ".png";

    SDL_Surface* pokeSurface = PokeSurface::onLoadImg(poke);
    if (pokeSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeRect= {
        50,
        90,
        pokeSurface->w * 2,
        pokeSurface->h * 2
    };
    PokeSurface::onDrawScaled(surf_display, pokeSurface, &pokeRect);
    SDL_FreeSurface(pokeSurface);
    
    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << (*evoChain)[offset + i][1];
    std::string id = formattedID.str();

    SDL_Surface* idSurface = TTF_RenderUTF8_Blended(
        font,
        id.c_str(),
        { 96, 96, 96 }
    );
    if (idSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: idSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect idRect = {
        20,
        WINDOW_HEIGHT/2 + 45,
        idSurface->w,
        idSurface->h
    };
    PokeSurface::onDraw(surf_display, idSurface, &idRect);
    SDL_FreeSurface(idSurface);

    //// Render poke name
    std::string name = (*evoChain)[offset + i][3];
    SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(
        font,
        name.c_str(),
        { 96, 96, 96 }
    );
    if (nameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: nameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect nameRect = {
        idRect.x,
        idRect.y + idRect.h + 10,
        static_cast<int>(nameSurface->w),
        static_cast<int>(nameSurface->h)
    };
    PokeSurface::onDraw(surf_display, nameSurface, &nameRect);
    SDL_FreeSurface(nameSurface);
    
    //// Render poke method( level or item)
    std::string method = (*evoChain)[offset + i][4];
    if (method == "NULL") {
        method = "__";
    }
    else if (method == "level-up" && (*evoChain)[offset + i][5] != "NULL") { // BY LEVEL
        method = "res/assets/encounters/" + method + ".png";
    }
    else if (method == "use-item") { // BY ITEM
        method = "res/icons/items/" + (*evoChain)[offset + i][6] + ".png";
    }
    else if (method == "trade") { // BY ITEM
        method = "res/assets/encounters/" + method + ".png";
    }
    else if ((*evoChain)[offset + i][8] != "NULL") { // BY HAPPYNESS
        method = "happiness";
        method = "res/assets/encounters/" + method + ".png";
    }
    //else if (method == "level-up" && (*evoChain)[offset + i][7] != "NULL") { // BY TIME OF DAY
    //    method = "res/assets/encounters/" + method + ".png";
    //}
    //if (method == "Level up" && (*evoChain)[offset + i][7] != "NULL") {
    //    method = "Time: " + (*evoChain)[offset + i][7];
    //    if ((*evoChain)[offset + i][8] != "NULL") {
    //        method += " Happiness: " + (*evoChain)[offset + i][8];
    //    }
    //}
    //else if (method == "Level up" && (*evoChain)[offset + i][8] != "NULL") {
    //    method = "Happiness: " + (*evoChain)[offset + i][8];
    //}
    SDL_Surface* methodSurface;
    SDL_Surface* methodSurfaceInfo;
    SDL_Rect methodRect;
    if (method == "__") {
        methodSurface = TTF_RenderUTF8_Blended(
            font,
            method.c_str(),
            { 96, 96, 96 }
        );
        methodRect = {
            nameRect.x,
            nameRect.y + nameRect.h + 5,
            static_cast<int>(methodSurface->w ),
            static_cast<int>(methodSurface->h )
        };
    }
    else {
        methodSurface = PokeSurface::onLoadImg(method);
        methodRect = {
            nameRect.x,
            nameRect.y + nameRect.h + 5,
            static_cast<int>(methodSurface->w * 1.5),
            static_cast<int>(methodSurface->h * 1.5)
        };
    }
    if (methodSurface == NULL ) {
        std::cout << "Unable to render text! SDL Error: methodSurface" << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    PokeSurface::onDrawScaled(surf_display, methodSurface, &methodRect);
    SDL_FreeSurface(methodSurface);

    //
    if ((*evoChain)[offset + i][5] != "NULL" || (*evoChain)[offset + i][8] != "NULL") {
        std::string methodInfo = (*evoChain)[offset + i][5] != "NULL" ? (*evoChain)[offset + i][5] : (*evoChain)[offset + i][8];
        methodSurfaceInfo = TTF_RenderUTF8_Blended(
            font,
            methodInfo.c_str(),
            { 96, 96, 96 }
        );
        if (methodSurfaceInfo == NULL ) {
            std::cout << "Unable to render text! SDL Error: methodSurfaceInfo" << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect pokeMethodInfoRect = {
            methodRect.x + methodRect.w,
            nameRect.y + nameRect.h + 10,
            static_cast<int>(methodSurfaceInfo->w * .9),
            static_cast<int>(methodSurfaceInfo->h * .9)
        };
        PokeSurface::onDrawScaled(surf_display, methodSurfaceInfo, &pokeMethodInfoRect);
        SDL_FreeSurface(methodSurfaceInfo);
    }

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
        // Play the sound effect
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedIndex < evoChain->size() - 1) {
        selectedIndex++;
        evo = (*evoChain)[selectedIndex];
        if (selectedIndex - offset >= 3) {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect_UpDown, 0);
    }
}

void PokedexActivity_PokemonView_Evolution::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Evolution::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonA(SDL_Keycode sym, Uint16 mod) {
    std::cout << evo[2] << '\n';
    PokedexDB::setPokemonID(std::stoi(evo[1]));
    PokedexDB::setPokemonIdentifier(evo[2]);
    PokedexDB::setVersionID(std::stoi(evo[11]));
    PokedexDB::setVersionGroupID(std::stoi(evo[12]));
    PokedexDB::setGenerationID(std::stoi(evo[13]));

    ////Call next activity
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Evolution::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Evolution::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Evolution::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
