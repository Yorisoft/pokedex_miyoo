#include "PokedexActivity_PokemonView_Info.h"
#include "PokedexActivityManager.h"

PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Info::instance;

PokedexActivity_PokemonView_Info::PokedexActivity_PokemonView_Info() : 
needRedraw(true),
dbResults(nullptr),
pokemon(nullptr),
backgroundSurface(nullptr),
iconSurface(nullptr),
typeASurface(nullptr),
typeBSurface(nullptr),
idSurface(nullptr),
nameSurface(nullptr),
heightSurface(nullptr),
weightSurface(nullptr),
genderSurface(nullptr),
genusSurface(nullptr),
flavorTextSurface(nullptr),
se_poke_cry(nullptr),
se_left_right(nullptr),
fontSurface(nullptr)
{
}

PokedexActivity_PokemonView_Info::~PokedexActivity_PokemonView_Info(){
	if(se_poke_cry)
		Mix_FreeChunk(se_poke_cry);

	if(se_left_right)
		Mix_FreeChunk(se_left_right);
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
	try{
		// make it a 3 digit
		std::stringstream formattedID;
		formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
		std::string pokeID = formattedID.str();

		std::string pokeCryPath = SOUND_EFFECT_CRY_PATH + pokeID + ' ' + ".wav"; // <- empty char is standin for form variant
		se_poke_cry = Mix_LoadWAV(pokeCryPath.c_str());
		if (!se_poke_cry) {
			std::cerr << "Failed to load sound pokeCry: " << Mix_GetError() << std::endl;
		}
		se_left_right = Mix_LoadWAV(SOUND_EFFECT_LEFT_RIGHT_PATH.c_str());
		if (!se_left_right) {
			std::cerr << "Failed to load sound se_left_right: " << Mix_GetError() << std::endl;
		}

		fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 34);
		if (fontSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load fontSurface! SDL Error:  ") + SDL_GetError());
		}

		// Background Surface
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
		};
		backgroundRect.x = 0;
		backgroundRect.y = 0;
		backgroundRect.w = WINDOW_WIDTH;
		backgroundRect.h = WINDOW_HEIGHT;
		// Pokemon Sprite
		std::string spritePath = SPRITES_IMG_BASE_PATH + PokedexDB::getPokemonIdentifier() + ".png";
		iconSurface = PokeSurface::onLoadImg(spritePath);
		if (iconSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeIconSurface! SDL Error:  ") + SDL_GetError());
		};
		pokeIconRect.x = 50;
		pokeIconRect.y = 100;   
		pokeIconRect.w = iconSurface->w * 2;
		pokeIconRect.h = iconSurface->h * 2;

		// Pokemon Types
		std::vector<std::string> pokeTypes = pokemon->getTypes();
		std::string typePath = TYPES_IMG_BASE_PATH + pokeTypes[0] + ".png";
		typeASurface = PokeSurface::onLoadImg(typePath);
		if (typeASurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeType1Surface! SDL Error:  ") + SDL_GetError());
		};
		pokeType1Rect.x = WINDOW_WIDTH - 15 - (typeASurface->w * 2) * 2;
		pokeType1Rect.y = 155;
		pokeType1Rect.w = typeASurface->w * 2;
		pokeType1Rect.h = typeASurface->h * 2;

		if (pokeTypes[1] != "NULL") {
			// Pokemnon Type 2 
			typePath = TYPES_IMG_BASE_PATH + pokeTypes[1] + ".png";
			typeBSurface = PokeSurface::onLoadImg(typePath);
			if (typeBSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeType2Surface! SDL Error:  ") + SDL_GetError());
			};
			pokeType2Rect.x = pokeType1Rect.x + pokeType1Rect.w + 5;
			pokeType2Rect.y = pokeType1Rect.y;
			pokeType2Rect.w = typeBSurface->w * 2;
			pokeType2Rect.h = typeBSurface->h * 2;
		}

		// Pokemon ID
		idSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			pokeID.c_str(),
			{ 96, 96, 96 }
		);
		if (idSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeIDSurface! SDL Error:  ") + SDL_GetError());
		};
		pokeIDRect.x = WINDOW_WIDTH/2 + 125;
		pokeIDRect.y = 65;
		pokeIDRect.w = idSurface->w;
		pokeIDRect.h = idSurface->h;

		// Pokemon Name
		std::string pokeName = pokemon->getName();
		nameSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			pokeName.c_str(),
			{ 96, 96, 96 }
		);
		if (nameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeNameSurface! SDL Error:  ") + SDL_GetError());
		};
		pokeNameRect.x = pokeIDRect.x;
		pokeNameRect.y = pokeIDRect.y + pokeIDRect.h + 15;
		pokeNameRect.w = static_cast<int>(nameSurface->w);
		pokeNameRect.h = static_cast<int>(nameSurface->h);

		// Pokemon Height && Weight
		std::string height = pokemon->getHeight();
		std::string weight = pokemon->getWeight();

		heightSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			height.c_str(),
			{ 96, 96, 96 }
		);
		if (heightSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeHeightSurface! SDL Error:  ") + SDL_GetError());
		};

		weightSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			weight.c_str(),
			{ 96, 96, 96 }
		);
		if (weightSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeWeightSurface! SDL Error:  ") + SDL_GetError());
		};

		heightRect.x = WINDOW_WIDTH/2 + 125;
		heightRect.y = WINDOW_HEIGHT/2 - 35;
		heightRect.w = heightSurface->w;
		heightRect.h = heightSurface->h;

		weightRect.x = heightRect.x;
		weightRect.y = 10 + heightRect.y + heightRect.h;
		weightRect.w = weightSurface->w;
		weightRect.h = weightSurface->h;

		// Pokemon Gender Rate
		std::vector<double>* genderRates = pokemon->getGenderRates();
		std::stringstream iss;
		iss << (*genderRates)[1] << "/" << (*genderRates)[0];
		std::string genderRatesStr = iss.str();

		genderSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			genderRatesStr.c_str(),
			{ 96, 96, 96 }
		);
		if (genderSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeGenderSurface! SDL Error:  ") + SDL_GetError());
		};
		genderRect.x = weightRect.x;
		genderRect.y = 15 + weightRect.y + weightRect.h;
		genderRect.w = genderSurface->w;
		genderRect.h = genderSurface->h;

		// Pokemon Genus
		std::string pokeGenus = pokemon->getGenus();
		genusSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			pokeGenus.c_str(),
			{ 96, 96, 96 }
		);
		if (genusSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load pokeGenusSurface! SDL Error:  ") + SDL_GetError());
		};
		genusRect.x = 10;
		genusRect.y = 60;
		genusRect.w = genusSurface->w;
		genusRect.h = genusSurface->h;

		// Pokemon Flavor Text
		std::string pokeFlavorText = pokemon->getFlavorText();
		flavorTextSurface = TTF_RenderUTF8_Blended_Wrapped(
			fontSurface,
			pokeFlavorText.c_str(),
			{ 96, 96, 96 },
			620 
		);
		if (flavorTextSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Info::initSDL() Unable to load flavorTextSurface! SDL Error:  ") + SDL_GetError());
		};
		fTextRect.x = 25;
		fTextRect.y = WINDOW_HEIGHT/2 + 100;
		fTextRect.w = flavorTextSurface->w;
		fTextRect.h = flavorTextSurface->h;
	} 
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}

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
    Mix_PlayChannel(-1, se_poke_cry, 0);

	needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Info::onActivate END \n";
}

void PokedexActivity_PokemonView_Info::onDeactivate() {
	if(fontSurface)
		TTF_CloseFont(fontSurface);
	fontSurface = nullptr;

	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

	if(iconSurface)
    	SDL_FreeSurface(iconSurface);
	iconSurface = nullptr;
	
	if(typeASurface)
    	SDL_FreeSurface(typeASurface);
	typeASurface = nullptr;
	if(typeBSurface)
        SDL_FreeSurface(typeBSurface);
	typeBSurface = nullptr;

	if(idSurface)
    	SDL_FreeSurface(idSurface);
	idSurface = nullptr;
	if(nameSurface)
    	SDL_FreeSurface(nameSurface);
	nameSurface = nullptr;

	if(heightSurface)
    	SDL_FreeSurface(heightSurface);
	heightSurface = nullptr;
	if(weightSurface)
    	SDL_FreeSurface(weightSurface);
	weightSurface = nullptr;

	if(genderSurface)
    	SDL_FreeSurface(genderSurface);
	genderSurface = nullptr;

	if(genusSurface)
    	SDL_FreeSurface(genusSurface);
	genderSurface = nullptr;

	if(flavorTextSurface)
		SDL_FreeSurface(flavorTextSurface);
	flavorTextSurface = nullptr;

    delete pokemon;
	pokemon = nullptr;
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
    PokeSurface::onDrawScaled(surf_display, iconSurface, &pokeIconRect);

    // Render Pokemon Type 1 
    PokeSurface::onDrawScaled(surf_display, typeASurface, &pokeType1Rect);

	// Render Pokemon Type 2
    if (typeBSurface != nullptr) {
        PokeSurface::onDrawScaled(surf_display, typeBSurface, &pokeType2Rect);
    }

    return true;
}

bool PokedexActivity_PokemonView_Info::renderNameID(SDL_Surface* surf_display, TTF_Font* font) {
    // Render Item ID
    PokeSurface::onDraw(surf_display, idSurface, &pokeIDRect);

    // Render Pokemon Name
    PokeSurface::onDrawScaled(surf_display, nameSurface, &pokeNameRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderHW(SDL_Surface* surf_display, TTF_Font* font) {

    // Render Pokemon Height & Weight
    PokeSurface::onDrawScaled(surf_display, heightSurface, &heightRect);

    PokeSurface::onDrawScaled(surf_display, weightSurface, &weightRect);

    // Render Pokemon Gender Rates 
    PokeSurface::onDrawScaled(surf_display, genderSurface, &genderRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderFlavorText(SDL_Surface* surf_display, TTF_Font* font) {
    // Render Pokemon Genus
    PokeSurface::onDrawScaled(surf_display, genusSurface, &genusRect);

    // Render Pokemon Flavor Text
    PokeSurface::onDrawScaled(surf_display, flavorTextSurface, &fTextRect);
 
    return true;
}

void PokedexActivity_PokemonView_Info::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Info::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Info::onButtonRight(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;
	
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);
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

