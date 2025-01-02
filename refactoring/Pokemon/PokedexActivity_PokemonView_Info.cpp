#include "PokedexActivity_PokemonView_Info.h"
#include "PokedexActivityManager.h"

PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Info::instance;

PokedexActivity_PokemonView_Info::PokedexActivity_PokemonView_Info() : 
pokeIDSurface(nullptr),
pokeNameSurface(nullptr),
pokeIconSurface(nullptr),
pokeType1Surface(nullptr),
pokeType2Surface(nullptr),
pokeGenusSurface(nullptr),
flavorTextSurface(nullptr),
pokeHeightSurface(nullptr),
pokeWeightSurface(nullptr),
backgroundSurface(nullptr),
fontSurface(nullptr),
dbResults(nullptr),
pokemon(nullptr)
{

    fontPath = "res/font/Pokemon_GB.ttf";
}

void PokedexActivity_PokemonView_Info::onActivate() {
    // create new pokemon object
    pokemon = new Pokemon();
    std::vector<double>* genderRates = pokemon->getGenderRates();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';


    std::cout << "Gender Ratio: " << '\n';
    std::cout << "Male: " << (*genderRates)[0] << '\n';
    std::cout << "Female: " << (*genderRates)[1] << '\n';

    fontSurface = TTF_OpenFont("res/font/Pokemon_GB.ttf", 22);
    if (!fontSurface) {
        std::cerr << "PokedexActivity_PokemonView_Info::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }
}

void PokedexActivity_PokemonView_Info::onDeactivate() {
    TTF_CloseFont(fontSurface);
}

void PokedexActivity_PokemonView_Info::onLoop() {
}

void PokedexActivity_PokemonView_Info::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    //std::cout << "PokedexActivityMenu::onRender START \n";
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render List Items
    std::string backgroundImageFile = "res/icons/icon/pokemon_fr_view_1.png";
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

    if (!renderNameID(surf_display)) {
        std::cout << "Unable to render item name and ID! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!renderHW(surf_display)) {
        std::cout << "Unable to render item height and weight! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!renderFlavorText(surf_display)) {
        std::cout << "Unable to render item flavor text! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void PokedexActivity_PokemonView_Info::onFreeze() {
// do nothing for now..
}

PokedexActivity_PokemonView_Info* PokedexActivity_PokemonView_Info::getInstance() {
    return &instance;
}

bool PokedexActivity_PokemonView_Info::renderSprites(SDL_Surface* surf_display) {
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

    // Render Item types 
    std::vector<std::string> pokeTypes = pokemon->getTypes();
    spritePath = "res/types/" + pokeTypes[0] + ".png";
    pokeType1Surface = PokeSurface::onLoadImg(spritePath);
    if (pokeType1Surface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeType1Surface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeType1Rect;
    pokeType1Rect.x = surf_display->w - 15 - (pokeType1Surface->w * 2) * 2;
    pokeType1Rect.y = 155;
    pokeType1Rect.w = pokeType1Surface->w * 2;
    pokeType1Rect.h = pokeType1Surface->h * 2;

    PokeSurface::onDrawScaled(surf_display, pokeType1Surface, &pokeType1Rect);
    SDL_FreeSurface(pokeType1Surface);

    if (pokeTypes[1] != "NULL") {
        // Render Item types 
        spritePath = "res/types/" + pokeTypes[1] + ".png";
        pokeType2Surface = PokeSurface::onLoadImg(spritePath);
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

bool PokedexActivity_PokemonView_Info::renderNameID(SDL_Surface* surf_display) {
    // Render Item ID
    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
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
    pokeIDRect.x = WINDOW_WIDTH/2 + 125;
    pokeIDRect.y = 70;
    pokeIDRect.w = pokeIDSurface->w;
    pokeIDRect.h = pokeIDSurface->h;
    PokeSurface::onDraw(surf_display, pokeIDSurface, &pokeIDRect);
    SDL_FreeSurface(pokeIDSurface);

    // Render Item ID
    std::string pokeName = pokemon->getName();
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
    pokeNameRect.y = pokeIDRect.y + pokeIDRect.h + 25;
    pokeNameRect.w = static_cast<int>(pokeNameSurface->w * 0.8);
    pokeNameRect.h = static_cast<int>(pokeNameSurface->h * 0.8);

    PokeSurface::onDrawScaled(surf_display, pokeNameSurface, &pokeNameRect);
    SDL_FreeSurface(pokeNameSurface);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderHW(SDL_Surface* surf_display) {
    std::string height = pokemon->getHeight();
    std::string weight = pokemon->getWeight();

    pokeHeightSurface = TTF_RenderText_Blended(
        fontSurface,
        height.c_str(),
        { 96, 96, 96 }
    );
    if (pokeHeightSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeHeightSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect heightRect;
    heightRect.x = WINDOW_WIDTH/2 + 122;
    heightRect.y = WINDOW_HEIGHT/2 - 35;
    heightRect.w = pokeHeightSurface->w;
    heightRect.h = pokeHeightSurface->h;

    PokeSurface::onDrawScaled(surf_display, pokeHeightSurface, &heightRect);
    SDL_FreeSurface(pokeHeightSurface);

    ///////
    pokeWeightSurface = TTF_RenderText_Blended(
        fontSurface,
        weight.c_str(),
        { 96, 96, 96 }
    );
    if (pokeWeightSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeWeightSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect weightRect;
    weightRect.x = heightRect.x;
    weightRect.y = 25 + heightRect.y + heightRect.h;
    weightRect.w = pokeWeightSurface->w;
    weightRect.h = pokeWeightSurface->h;

    PokeSurface::onDrawScaled(surf_display, pokeWeightSurface, &weightRect);
    SDL_FreeSurface(pokeWeightSurface);

    /// 
    std::vector<double>* genderRates = pokemon->getGenderRates();

    std::stringstream iss;
    iss << (*genderRates)[0] << "/" << (*genderRates)[1];
    std::string genderRatesStr = iss.str();
    SDL_Surface* pokeGenderSurface = TTF_RenderText_Blended(
        fontSurface,
        genderRatesStr.c_str(),
        { 96, 96, 96 }
    );
    if (pokeGenderSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeGenderSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect genderRect;
    genderRect.x = weightRect.x;
    genderRect.y = 25 + weightRect.y + weightRect.h;
    genderRect.w = pokeGenderSurface->w;
    genderRect.h = pokeGenderSurface->h;

    PokeSurface::onDrawScaled(surf_display, pokeGenderSurface, &genderRect);
    SDL_FreeSurface(pokeGenderSurface);
    return true;
}

bool PokedexActivity_PokemonView_Info::renderFlavorText(SDL_Surface* surf_display) {
    // pokemon genus
    std::string pokeGenus = pokemon->getGenus();
    pokeGenusSurface = TTF_RenderText_Blended(
        fontSurface,
        pokeGenus.c_str(),
        { 96, 96, 96 }
    );
    if (pokeGenusSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeGenusSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect genusRect;
    genusRect.x = 10;
    genusRect.y = 60;
    genusRect.w = 300;
    genusRect.h = 25;

    PokeSurface::onDrawScaled(surf_display, pokeGenusSurface, &genusRect);
    SDL_FreeSurface(pokeGenusSurface);

    // flavor text
    std::string pokeFlavorText = pokemon->getFlavorText();
    flavorTextSurface = TTF_RenderText_Blended_Wrapped(
        fontSurface,
        pokeFlavorText.c_str(),
        { 96, 96, 96 },
        640 
    );
    if (flavorTextSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: flavorTextSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    
    SDL_Rect fTextRect;
    fTextRect.x = 20;
    fTextRect.y = WINDOW_HEIGHT/2 + 100;
    fTextRect.w = flavorTextSurface->w;
    fTextRect.h = flavorTextSurface->h;

    PokeSurface::onDrawScaled(surf_display, flavorTextSurface, &fTextRect);
    SDL_FreeSurface(flavorTextSurface);
 
    return true;
}

void PokedexActivity_PokemonView_Info::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Info::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
    //PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Info::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonB(SDL_Keycode sym, Uint16 mod) {
    ////Set pokemon identifier for PokedexDB
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}

