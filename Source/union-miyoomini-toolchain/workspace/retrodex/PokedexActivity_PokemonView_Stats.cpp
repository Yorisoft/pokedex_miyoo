#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Stats.h"

PokedexActivity_PokemonView_Stats PokedexActivity_PokemonView_Stats::instance;

PokedexActivity_PokemonView_Stats::PokedexActivity_PokemonView_Stats() : 
pokemon(nullptr),
dbResults(nullptr)
{
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

    std::string sEffectPath = "res/audio/sound_effects/left_right.wav"; // <- empty char is standin for form variant
    sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }
}

void PokedexActivity_PokemonView_Stats::onDeactivate() {
    delete pokemon;
    
    // in order to play sounds asynchrounously with activity, 
    // we cant call Mix_FreeChunk immediately after playing.. should probably make seperate class for this. 
    /*if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;*/
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
    SDL_Surface* backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
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
    SDL_Surface* pokeIconSurface = PokeSurface::onLoadImg(spritePath);
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

    SDL_Surface* pokeIDSurface = TTF_RenderUTF8_Blended(
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
    SDL_Surface* pokeNameSurface = TTF_RenderUTF8_Blended(
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
    std::vector<std::vector<std::string>>* statNames = PokedexDB::executeSQL(&SQL_getStatNames);

    for (int i = 0; i < stats.size(); ++i) {
        ////////////////////////////////////////////////

        std::string statsName = (*statNames)[i][2];

        SDL_Surface* statsNameSurface = TTF_RenderUTF8_Blended(
            font,
            statsName.c_str(),
            { 248, 248, 248 }
        );

        int topBorder = 80, spacing = 14;
        SDL_Rect statsNameRect;
        statsNameRect.x = (WINDOW_WIDTH / 2) + 20;
        statsNameRect.y = ((statsNameSurface->h + 5) * i) + topBorder;
        statsNameRect.w = statsNameSurface->w;
        statsNameRect.h = statsNameSurface->h;

        PokeSurface::onDrawScaled(surf_display, statsNameSurface, &statsNameRect);
        SDL_FreeSurface(statsNameSurface);

        /////////////////////////////////////////////
        //std::string statIconPath = "res/icons/icon/" + statFileNames[i] + "_icon_HOME.png";
        //SDL_Surface* statIcon = PokeSurface::onLoadImg(statIconPath);
        //if(!statIcon){
        //    std::cout << "Unable to load statIcon! SDL Error: statIcon " << SDL_GetError() << std::endl;
        //    exit(EXIT_FAILURE);
        //}

        ////int topBorder = 80, spacing = 14;
        //SDL_Rect statIconRect;
        //statIconRect.x = WINDOW_WIDTH/2 + 150;
        //statIconRect.y = static_cast<int>(topBorder + (i * (statIcon->h*.5 + spacing)));
        //statIconRect.w = static_cast<int>(statIcon->w * .5);
        //statIconRect.h = static_cast<int>(statIcon->w * .5);
        //
        //PokeSurface::onDrawScaled(surf_display, statIcon, &statIconRect);
        //SDL_FreeSurface(statIcon);

        ////////////////////////////////////////////////

        std::string statsToString = std::to_string(stats[i]);

        SDL_Surface* statsSurface = TTF_RenderUTF8_Blended(
            font,
            statsToString.c_str(),
            { 96, 96, 96 }
        );

        SDL_Rect statsRect;
        statsRect.x = (WINDOW_WIDTH - statsSurface->w) - 10;
        statsRect.y = (38 * i) + 80;
        statsRect.w = statsSurface->w;
        statsRect.h = statsSurface->h;

        PokeSurface::onDrawScaled(surf_display, statsSurface, &statsRect);
        SDL_FreeSurface(statsSurface);

    }

    std::vector<std::vector<std::string>>* abilities = pokemon->getAbilities();
    if (!abilities->empty()) {
        std::string ability = (*abilities)[0][0] + "    " + (*abilities)[0][1];
        SDL_Surface* abilitySurface = TTF_RenderUTF8_Blended_Wrapped(
            font,
            ability.c_str(),
            { 96, 96, 96 },
            520
        );

        int topBorder = 80, spacing = 14;
        SDL_Rect abilityRect;
        abilityRect.x = 130;
        abilityRect.y = WINDOW_HEIGHT/2 + 80;
        abilityRect.w = abilitySurface->w;
        abilityRect.h = abilitySurface->h;

        PokeSurface::onDrawScaled(surf_display, abilitySurface, &abilityRect);
        SDL_FreeSurface(abilitySurface);

        if (abilities->size() > 1) { // has hidden ability
            ability = (*abilities)[1][0] + "    " + (*abilities)[1][1];
            SDL_Surface* H_AbilitySurface = TTF_RenderUTF8_Blended_Wrapped(
                font,
                ability.c_str(),
                { 96, 96, 96 },
                620
            );

            int topBorder = 80, spacing = 14;
            SDL_Rect h_abilityRect;
            h_abilityRect.x = abilityRect.x;
            h_abilityRect.y = WINDOW_HEIGHT - 80;
            h_abilityRect.w = H_AbilitySurface->w;
            h_abilityRect.h = H_AbilitySurface->h;

            PokeSurface::onDrawScaled(surf_display, H_AbilitySurface, &h_abilityRect);
            SDL_FreeSurface(H_AbilitySurface);

        }
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
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod){
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);

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
