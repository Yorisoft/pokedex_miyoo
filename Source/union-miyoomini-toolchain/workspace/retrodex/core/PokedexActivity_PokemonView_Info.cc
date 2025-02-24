#include "PokedexActivity_PokemonView_Info.h"
#include "PokedexActivityManager.h"

PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Info::instance;

PokedexActivity_PokemonView_Info::PokedexActivity_PokemonView_Info() : 
dbResults(nullptr),
pokemon(nullptr),
pokeCry(nullptr),
backgroundSurface(nullptr),
pokeIconSurface(nullptr),
pokeType1Surface(nullptr),
pokeType2Surface(nullptr),
pokeIDSurface(nullptr),
pokeNameSurface(nullptr),
pokeHeightSurface(nullptr),
pokeWeightSurface(nullptr),
pokeGenderSurface(nullptr),
pokeGenusSurface(nullptr),
flavorTextSurface(nullptr)
{
}

void PokedexActivity_PokemonView_Info::printPokeInfo(){
    std::vector<double>* genderRates = pokemon->getGenderRates();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Regional ID: " << pokemon->getRegionalID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';
    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "Gender Ratio: " << '\n';
    std::cout << "Female: " << (*genderRates)[0] << '\n';
    std::cout << "Male: " << (*genderRates)[1] << '\n';

}

bool PokedexActivity_PokemonView_Info::initSDL(){
	TTF_Font* temp_font = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
	if (temp_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		return -1;
	}

    // make it a 3 digit
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
    std::string pokeID = formattedID.str();

    std::string pokeCryPath = "res/assets/pokemons/cry/" + pokeID + ' ' + ".wav"; // <- empty char is standin for form variant
    pokeCry = Mix_LoadWAV(pokeCryPath.c_str());
    if (!pokeCry) {
        std::cerr << "Failed to load sound pokeCry: " << Mix_GetError() << std::endl;
    }

	// Background Surface
	backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
	if (backgroundSurface == NULL) {
		std::cout << "Unable to render text! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
		return -1;
	};
	backgroundRect.x = 0;
	backgroundRect.y = 0;
	backgroundRect.w = WINDOW_WIDTH;
	backgroundRect.h = WINDOW_HEIGHT;

    // Pokemon Sprite
    std::string pokeIdentifier = PokedexDB::getPokemonIdentifier();
    std::string path = SPRITES_IMG_BASE_PATH + pokeIdentifier + ".png";
    pokeIconSurface = PokeSurface::onLoadImg(path);
    if (pokeIconSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIconSurface " << SDL_GetError() << std::endl;
		return -1;
    };
    pokeIconRect.x = 50;
    pokeIconRect.y = 100;   
    pokeIconRect.w = pokeIconSurface->w * 2;
    pokeIconRect.h = pokeIconSurface->h * 2;

	// Pokemon Types
    std::vector<std::string> pokeTypes = pokemon->getTypes();
    path = TYPES_IMG_BASE_PATH + pokeTypes[0] + ".png";
    pokeType1Surface = PokeSurface::onLoadImg(path);
    if (pokeType1Surface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeType1Surface " << SDL_GetError() << std::endl;
		return -1;
    };
    pokeType1Rect.x = WINDOW_WIDTH - 15 - (pokeType1Surface->w * 2) * 2;
    pokeType1Rect.y = 155;
    pokeType1Rect.w = pokeType1Surface->w * 2;
    pokeType1Rect.h = pokeType1Surface->h * 2;


    if (pokeTypes[1] != "NULL") {
        // Pokemnon Type 2 
        path = TYPES_IMG_BASE_PATH + pokeTypes[1] + ".png";
        pokeType2Surface = PokeSurface::onLoadImg(path);
        if (pokeType2Surface == NULL) {
            std::cout << "Unable to render text! SDL Error: pokeType2Surface " << SDL_GetError() << std::endl;
			return -1;
        };
        pokeType2Rect.x = pokeType1Rect.x + pokeType1Rect.w + 5;
        pokeType2Rect.y = pokeType1Rect.y;
        pokeType2Rect.w = pokeType2Surface->w * 2;
        pokeType2Rect.h = pokeType2Surface->h * 2;
	}

	// Pokemon ID
    pokeIDSurface = TTF_RenderUTF8_Blended(
        temp_font,
        pokeID.c_str(),
        { 96, 96, 96 }
    );
    if (pokeIDSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeIDSurface " << TTF_GetError() << std::endl;
		return -1;
    };
    pokeIDRect.x = WINDOW_WIDTH/2 + 125;
    pokeIDRect.y = 65;
    pokeIDRect.w = pokeIDSurface->w;
    pokeIDRect.h = pokeIDSurface->h;

	// Pokemon Name
    std::string pokeName = pokemon->getName();
    pokeNameSurface = TTF_RenderUTF8_Blended(
        temp_font,
        pokeName.c_str(),
        { 96, 96, 96 }
    );
    if (pokeNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeNameSurface " << TTF_GetError() << std::endl;
		return -1;
    };
    pokeNameRect.x = pokeIDRect.x;
    pokeNameRect.y = pokeIDRect.y + pokeIDRect.h + 15;
    pokeNameRect.w = static_cast<int>(pokeNameSurface->w);
    pokeNameRect.h = static_cast<int>(pokeNameSurface->h);

	// Pokemon Height && Weight
    std::string height = pokemon->getHeight();
    std::string weight = pokemon->getWeight();

    pokeHeightSurface = TTF_RenderUTF8_Blended(
        temp_font,
        height.c_str(),
        { 96, 96, 96 }
    );
    if (pokeHeightSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeHeightSurface " << TTF_GetError() << std::endl;
		return -1;
    };

    pokeWeightSurface = TTF_RenderUTF8_Blended(
        temp_font,
        weight.c_str(),
        { 96, 96, 96 }
    );
    if (pokeWeightSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeWeightSurface " << TTF_GetError() << std::endl;
		return -1;
    };

    heightRect.x = WINDOW_WIDTH/2 + 125;
    heightRect.y = WINDOW_HEIGHT/2 - 35;
    heightRect.w = pokeHeightSurface->w;
    heightRect.h = pokeHeightSurface->h;

    weightRect.x = heightRect.x;
    weightRect.y = 10 + heightRect.y + heightRect.h;
    weightRect.w = pokeWeightSurface->w;
    weightRect.h = pokeWeightSurface->h;

	// Pokemon Gender Rate
    std::vector<double>* genderRates = pokemon->getGenderRates();
    std::stringstream iss;
    iss << (*genderRates)[1] << "/" << (*genderRates)[0];
    std::string genderRatesStr = iss.str();

    pokeGenderSurface = TTF_RenderUTF8_Blended(
        temp_font,
        genderRatesStr.c_str(),
        { 96, 96, 96 }
    );
    if (pokeGenderSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeGenderSurface " << TTF_GetError() << std::endl;
		return -1;
    };
    genderRect.x = weightRect.x;
    genderRect.y = 15 + weightRect.y + weightRect.h;
    genderRect.w = pokeGenderSurface->w;
    genderRect.h = pokeGenderSurface->h;

    // Pokemon Genus
    std::string pokeGenus = pokemon->getGenus();
    pokeGenusSurface = TTF_RenderUTF8_Blended(
        temp_font,
        pokeGenus.c_str(),
        { 96, 96, 96 }
    );
    if (pokeGenusSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: pokeGenusSurface " << TTF_GetError() << std::endl;
		return -1;
    };
    genusRect.x = 10;
    genusRect.y = 60;
    genusRect.w = pokeGenusSurface->w;
    genusRect.h = pokeGenusSurface->h;

    // Pokemon Flavor Text
    std::string pokeFlavorText = pokemon->getFlavorText();
    flavorTextSurface = TTF_RenderUTF8_Blended_Wrapped(
        temp_font,
        pokeFlavorText.c_str(),
        { 96, 96, 96 },
        620 
    );
    if (flavorTextSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: flavorTextSurface " << TTF_GetError() << std::endl;
		return -1;
    };
    fTextRect.x = 25;
    fTextRect.y = WINDOW_HEIGHT/2 + 100;
    fTextRect.w = flavorTextSurface->w;
    fTextRect.h = flavorTextSurface->h;

	return true;
}

void PokedexActivity_PokemonView_Info::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Info::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
	printPokeInfo();

	// Initialize SDL components
	if(!initSDL()){
		std::cout << "PokedexActivity_PokemonView_Info::initSDL(): SDL Error: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

    // Play the sound effect
    Mix_PlayChannel(-1, pokeCry, 0);

	needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Info::onActivate END \n";
}

void PokedexActivity_PokemonView_Info::onDeactivate() {
	if(pokeCry)
		Mix_FreeChunk(pokeCry);

	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);

	if(pokeIconSurface)
    	SDL_FreeSurface(pokeIconSurface);
	
	if(pokeType1Surface)
    	SDL_FreeSurface(pokeType1Surface);
	if(pokeType2Surface)
        SDL_FreeSurface(pokeType2Surface);

	if(pokeIDSurface)
    	SDL_FreeSurface(pokeIDSurface);
	if(pokeNameSurface)
    	SDL_FreeSurface(pokeNameSurface);

	if(pokeHeightSurface)
    	SDL_FreeSurface(pokeHeightSurface);
	if(pokeWeightSurface)
    	SDL_FreeSurface(pokeWeightSurface);

	if(pokeGenderSurface)
    	SDL_FreeSurface(pokeGenderSurface);

	if(pokeGenusSurface)
    	SDL_FreeSurface(pokeGenusSurface);

	if(flavorTextSurface)
		SDL_FreeSurface(flavorTextSurface);

    delete pokemon;
}

void PokedexActivity_PokemonView_Info::onLoop() {
}

void PokedexActivity_PokemonView_Info::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    //std::cout << "PokedexActivityMenu::onRender START \n";
	if(needRedraw){
		// Clear the display surface
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

		PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

		if (!renderSprites(surf_display)) {
			std::cout << "Unable to render item sprites! SDL Error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!renderNameID(surf_display, font)) {
			std::cout << "Unable to render item name and ID! SDL Error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!renderHW(surf_display, font)) {
			std::cout << "Unable to render item height and weight! SDL Error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!renderFlavorText(surf_display, font)) {
			std::cout << "Unable to render item flavor text! SDL Error: " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}
		needRedraw = false;
	}
}

void PokedexActivity_PokemonView_Info::onFreeze() {
// do nothing for now..
}

PokedexActivity_PokemonView_Info* PokedexActivity_PokemonView_Info::getInstance() {
    return &instance;
}

bool PokedexActivity_PokemonView_Info::renderSprites(SDL_Surface* surf_display) {
	// Render Pokemon Sprite
    PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);

    // Render Pokemon Type 1 
    PokeSurface::onDrawScaled(surf_display, pokeType1Surface, &pokeType1Rect);

	// Render Pokemon Type 2
    if (pokeType2Surface != nullptr) {
        PokeSurface::onDrawScaled(surf_display, pokeType2Surface, &pokeType2Rect);
    }

    return true;
}

bool PokedexActivity_PokemonView_Info::renderNameID(SDL_Surface* surf_display, TTF_Font* font) {
    // Render Item ID
    PokeSurface::onDraw(surf_display, pokeIDSurface, &pokeIDRect);

    // Render Pokemon Name
    PokeSurface::onDrawScaled(surf_display, pokeNameSurface, &pokeNameRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderHW(SDL_Surface* surf_display, TTF_Font* font) {

    // Render Pokemon Height & Weight
    PokeSurface::onDrawScaled(surf_display, pokeHeightSurface, &heightRect);

    PokeSurface::onDrawScaled(surf_display, pokeWeightSurface, &weightRect);

    // Render Pokemon Gender Rates 
    PokeSurface::onDrawScaled(surf_display, pokeGenderSurface, &genderRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderFlavorText(SDL_Surface* surf_display, TTF_Font* font) {
    // Render Pokemon Genus
    PokeSurface::onDrawScaled(surf_display, pokeGenusSurface, &genusRect);

    // Render Pokemon Flavor Text
    PokeSurface::onDrawScaled(surf_display, flavorTextSurface, &fTextRect);
 
    return true;
}

void PokedexActivity_PokemonView_Info::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    std::string sEffectPath = "res/assets/sound_effects/left_right.wav"; // <- empty char is standin for form variant
    Mix_Chunk* sEffect = Mix_LoadWAV(sEffectPath.c_str());
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }
    // Play the sound effect
    Mix_PlayChannel(1, sEffect, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Info::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}

