#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Location.h"

PokedexActivity_PokemonView_Location PokedexActivity_PokemonView_Location::instance;

PokedexActivity_PokemonView_Location::PokedexActivity_PokemonView_Location() :
selectedIndex(0),
offset(0),
needRedraw(true),
dbResults(nullptr),
routes(nullptr),
backgroundSurface(nullptr), 
listEntrySurface(nullptr),
iconSurface(nullptr), 
pokeNameSurface(nullptr), 
typeASurface(nullptr), 
typeBSurface(nullptr),
se_left_right(nullptr),
se_up_down(nullptr),
pokemon(nullptr)
{
}

PokedexActivity_PokemonView_Location::~PokedexActivity_PokemonView_Location() {
	if(se_up_down)
		Mix_FreeChunk(se_up_down);

	if(se_left_right)
		Mix_FreeChunk(se_left_right);
}

bool PokedexActivity_PokemonView_Location::initSDL() {
 std::cout << "PokedexActivity_PokemonView_Location::initSDL START \n";
  try {
	se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
	if (!se_left_right) {
	  std::cerr << "Failed to load sound sEffect: " << Mix_GetError()
				<< std::endl;
	}

	se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
	if (!se_left_right) {
	  std::cerr << "Failed to load sound sEffect: " << Mix_GetError()
				<< std::endl;
	}
	
	fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 34);
	if (fontSurface == NULL) {
		std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	// Background
	backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
	if (backgroundSurface == NULL) {
		throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
	};

	backgroundRect = {
		0, 0,
	    WINDOW_WIDTH, 
		WINDOW_HEIGHT
	};

	// List Entry background
    listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        static_cast<int>(WINDOW_WIDTH * 0.9),
        ITEM_HEIGHT,
        DEPTH,
        SDL_PIXELFORMAT_RGBA32
    );
    if (!listEntrySurface) {
		throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load listEntrySurface! SDL Error:  ") + SDL_GetError());
    }

	// Pokemon Icon
    std::string iconPath = PokedexDB::getPokemonIdentifier();
    iconPath = ICON_IMG_BASE_PATH + iconPath + ".png";
    iconSurface = PokeSurface::onLoadImg(iconPath);
    if (iconSurface == NULL) {
		throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load iconSurface! SDL Error:  ") + SDL_GetError());
    };
    iconRect = {
        0, 
		55,
        iconSurface->w * 2, 
		iconSurface->h * 2
    };

	// Pokemon Name
    pokeNameSurface = TTF_RenderUTF8_Blended(
        fontSurface,
        pokemon->getName().c_str(),
        COLOR
    );
    if (pokeNameSurface == NULL) {
		throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load pokeNameSurface! SDL Error:  ") + SDL_GetError());
    };
    pokeNameRect = {
        (iconRect.x + iconRect.w), 
		iconRect.y + 10,
        pokeNameSurface->w, 
		pokeNameSurface->h
    };

	// Pokemon Types
	std::string typePath = 
		TYPE_IMG_BASE_PATH + 
		pokemon->getTypes()[0] + 
		".png";
    typeASurface = PokeSurface::onLoadImg(typePath);
    if (typeASurface == NULL) {
		throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load typeASurface! SDL Error:  ") + SDL_GetError());
    };
    typeARect = {
        iconRect.x + iconRect.w, 
		(pokeNameRect.y + pokeNameRect.h ) + 5,
        typeASurface->w * 2,
		typeASurface->h * 2
    };

    if (pokemon->getTypes()[1] != "NULL") { 
		typePath = 
			TYPE_IMG_BASE_PATH + 
			pokemon->getTypes()[1] + 
			".png";
        typeBSurface = PokeSurface::onLoadImg(typePath);
        if (typeBSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load typeBSurface! SDL Error:  ") + SDL_GetError());
        };
        typeBRect = {
            typeARect.x + typeARect.w + 5, 
			typeBRect.y = typeARect.y,
            typeBSurface->w * 2, 
			typeBRect.h = typeBSurface->h * 2
        };
    }

	// Location
	for(size_t i = 0; i < routes->size(); i++){
		// NAME
		std::string location = (*routes)[i][1];
		for (int i = 0; i < location.size(); i++) {
			location[i] = std::toupper(location[i]);
		}
		SDL_Surface* locationNameSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			location.c_str(),
			COLOR
		);
		if (locationNameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load locationNameSurface! SDL Error:  ") + SDL_GetError());
		};

		// CONDITION
		SDL_Surface* conditionSurface = nullptr;
		if ((*routes)[i][7] != "NULL") {
			std::string condition = (*routes)[i][7];
			std::string path = 
				METHOD_IMG_BASE_PATH + 
				condition + 
				".png";
			conditionSurface = PokeSurface::onLoadImg(path);
			if (conditionSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load conditionSurface! SDL Error:  ") + SDL_GetError());
			};

		}

		// METHOD
		std::string method = (*routes)[i][2];
		method = method == "walk" ? "grass" : method;
		method = method == "gift-egg" ? "egg" : method;
		method = method == "surf" ? "water" : method;
		std::string path = 
			METHOD_IMG_BASE_PATH +
			method + 
			".png";

		SDL_Surface* methodSurface = PokeSurface::onLoadImg(path);
		if (methodSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load methodSurface! SDL Error:  ") + SDL_GetError());
		};

		// RATE
		std::string rate =  (*routes)[i][5] + '%';
		SDL_Surface* rateSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			rate.c_str(),
			COLOR
		);
		if (rateSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load rateSurface! SDL Error:  ") + SDL_GetError());
		};

		// Min Level
		SDL_Surface* minLevelSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			(*routes)[i][3].c_str(),
			COLOR
		);
		if (minLevelSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load minLevelSurface! SDL Error:  ") + SDL_GetError());
		};

		// Max Level
		SDL_Surface* maxLevelSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			(*routes)[i][4].c_str(),
			COLOR
		);
		if (maxLevelSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load maxLevelSurface! SDL Error:  ") + SDL_GetError());
		};

		// Detail Location Name
		location = (*routes)[i][1];
		std::string subLocation;
		SDL_Surface* detailLocationNameSurface = nullptr;
		if ((subLocation = (*routes)[i][6]) != "NULL") {
			location += '\n' + subLocation;
		}
		detailLocationNameSurface = TTF_RenderUTF8_Blended_Wrapped(
			fontSurface,
			location.c_str(),
			COLOR,
			295
		);
		if (detailLocationNameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load detailLocationNameSurface! SDL Error:  ") + SDL_GetError());
		};

		locationNameSurface_cache.push_back(locationNameSurface);
		methodSurface_cache.push_back(methodSurface);
		conditionSurface_cache.push_back(conditionSurface);
		rateSurface_cache.push_back(rateSurface);
		levelSurface_cache.push_back({minLevelSurface, maxLevelSurface});
		detailLocationNameSurface_cache.push_back(detailLocationNameSurface);
	}
  } 
  catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

 	std::cout << "PokedexActivity_PokemonView_Location::initSDL END \n";
  	return true;
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
	std::cout << "Done printing routes \n ";

}

void PokedexActivity_PokemonView_Location::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

    pokemon = new Pokemon();
    routes = pokemon->getRoutes();
    printPokeInfo();

	// Cant call this here? 
	//route = (*routes)[selectedIndex];
	
	if(!initSDL()){
		std::cerr << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}

	needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";
}

void PokedexActivity_PokemonView_Location::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

	if(iconSurface)
		SDL_FreeSurface(iconSurface);
	iconSurface = nullptr;

	if(pokeNameSurface)
		SDL_FreeSurface(pokeNameSurface);
	pokeNameSurface = nullptr;

	if(typeASurface)
		SDL_FreeSurface(typeASurface);
	typeASurface = nullptr;

	if(typeBSurface)
		SDL_FreeSurface(typeBSurface);
	typeBSurface = nullptr;

	if(listEntrySurface)
		SDL_FreeSurface(listEntrySurface);
	listEntrySurface = nullptr;

	for(SDL_Surface* surface : locationNameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : conditionSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : methodSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(SDL_Surface* surface : rateSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	for(std::pair<SDL_Surface*, SDL_Surface*> surfaces: levelSurface_cache){
		if(surfaces.first){
			SDL_FreeSurface(surfaces.first);
			surfaces.first = nullptr;
		}
		if(surfaces.second){
			SDL_FreeSurface(surfaces.second);
			surfaces.second = nullptr;
		}
	}

	for(SDL_Surface* surface : detailLocationNameSurface_cache)
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}

	locationNameSurface_cache.clear();
	conditionSurface_cache.clear();
	methodSurface_cache.clear();
	rateSurface_cache.clear();
	levelSurface_cache.clear();
	detailLocationNameSurface_cache.clear();

    delete pokemon;
    pokemon = nullptr;

	//pointer comes from PokedexDB
	//PokedexDB handles its own pointers
	/* /1* delete routes; *1/ */
	routes = nullptr;

    selectedIndex = 0, offset = 0;

	route.clear();

    std::cout << "PokedexActivity_PokemonView_Location::onActivate END \n";
}

void PokedexActivity_PokemonView_Location::onLoop() {
}

void PokedexActivity_PokemonView_Location::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
	if(needRedraw){
		try{
			SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));
			// Render _PokemonView_Location Items
			//Render background
			if(!PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect)){
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::onRender() Unable to render backgroundSurface! SDL Error:  ") + SDL_GetError());
			}

			if (!renderPokeInfo(surf_display, font)) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::onRender() - Failed at renderPokeInfo! SDL Error:  ") + SDL_GetError());
			}
			
			// Render _PokemonView_Location Items
			for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < routes->size(); i++) {
				route = (*routes)[offset + i];

				// Render selected Item details
				if (offset + i == selectedIndex) {
					if (!renderItemDetails(surf_display, font, i)) {
						throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::onRender() - Failed at renderItemDetails! SDL Error:  ") + SDL_GetError());
					}
				}

				// Render list items
				if (!renderListItems(surf_display, renderer, font, i)) {
					throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::onRender() Unable to load renderListItems! SDL Error:  ") + SDL_GetError());
				}
			}

			needRedraw = false;
		}
		catch(const std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

bool PokedexActivity_PokemonView_Location::renderPokeInfo(SDL_Surface* surf_display, TTF_Font* font) {
	try{
		// Render icon
		if(!PokeSurface::onDrawScaled(surf_display, iconSurface, &iconRect)){
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::renderPokeInfo() Unable to load iconSurface! SDL Error:  ") + SDL_GetError());
		}
		
		// Render Name
		if(!PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect)){
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::renderPokeInfo() Unable to load pokeNameSurface! SDL Error:  ") + SDL_GetError());
		}

		// render poke types
		if(!PokeSurface::onDrawScaled(surf_display, typeASurface, &typeARect)){
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::renderPokeInfo() Unable to load typeASurface! SDL Error:  ") + SDL_GetError());
		}

		//List item types_2
		if (pokemon->getTypes()[1] != "NULL") { 
			if(!PokeSurface::onDrawScaled(surf_display, typeBSurface, &typeBRect)){
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::renderPokeInfo() Unable to load typeBSurface! SDL Error:  ") + SDL_GetError());
			}
		}
	}
	catch(const std::runtime_error& e){
		std::cerr << e.what() <<std::endl;
    	return false;
	}

    return true;
}

bool PokedexActivity_PokemonView_Location::renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i) {
    // Render levels - min
	minLevelRect = {
		155, 175, 
		levelSurface_cache[offset + i].first->w,
		levelSurface_cache[offset + i].first->h
	};
    PokeSurface::onDraw(surf_display, levelSurface_cache[offset + i].first, &minLevelRect);

    // Render levels - max
	maxLevelRect = {
		minLevelRect.x, 
		(minLevelRect.y + minLevelRect.h ) + 10,
		levelSurface_cache[offset + i].second->w,
		levelSurface_cache[offset + i].second->h
	};
    PokeSurface::onDraw(surf_display, levelSurface_cache[offset + i].second, &maxLevelRect);

    // Render location
    detailLocationRect = {
        15, (WINDOW_HEIGHT / 2 ) + 60,
        detailLocationNameSurface_cache[offset + i]->w, 
		detailLocationNameSurface_cache[offset + i]->h
    };
    PokeSurface::onDraw(surf_display, detailLocationNameSurface_cache[offset + i], &detailLocationRect);

    // Render method
    double scaling = 1.5;
    int bottomBorder = 37, border = 10;
    methodRect = {
        detailLocationRect.x, 
        WINDOW_HEIGHT - methodSurface_cache[offset + i]->h - bottomBorder,
        static_cast<int>(methodSurface_cache[offset + i]->w * scaling), 
        static_cast<int>(methodSurface_cache[offset + i]->h * scaling)
    };
    PokeSurface::onDrawScaled(surf_display, methodSurface_cache[offset + i], &methodRect);

    // Render item condition
    if (route[7] != "NULL") {
        conditionRect = {
            methodRect.x + methodRect.w + border, 
            WINDOW_HEIGHT - conditionSurface_cache[offset + i]->h - bottomBorder,
            static_cast<int>(conditionSurface_cache[offset + i]->w * scaling), 
            static_cast<int>(conditionSurface_cache[offset + i]->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface_cache[offset + i], &conditionRect);
    }
 
    // Render rate 
    rateRect = {
        250, 
        WINDOW_HEIGHT - rateSurface_cache[offset + i]->h - 20,
        rateSurface_cache[offset + i]->w,
        rateSurface_cache[offset + i]->h
    };
    PokeSurface::onDraw(surf_display, rateSurface_cache[offset + i], &rateRect);

    return true;
}

bool PokedexActivity_PokemonView_Location::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    //Render list item background/surface
    int spacing = 18; 
    listEntryRect = {
        static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8), 
        65 + (i * (ITEM_HEIGHT + spacing)), 
        static_cast<int>(surf_display->w * 0.45), 
        ITEM_HEIGHT
    };
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);

    if (offset + i == selectedIndex) {
        // Set the render draw color
		// Would prefer a method like this.
		// Texture gets updates by screen surface, 
		// Render gets updated by texture. Any changes to either will be overwritten
        /* SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); */
		/* SDL_RenderDrawRect(renderer, &listEntryRect); */   

		// Draw red border using SDL_FillRect
		// TODO: replaces with surface from image
        const int BORDER_WIDTH = 2;
        SDL_Rect borderRects[] = {
            {listEntryRect.x, listEntryRect.y, listEntryRect.w, BORDER_WIDTH},                     // Top
            {listEntryRect.x, listEntryRect.y + listEntryRect.h - BORDER_WIDTH, listEntryRect.w, BORDER_WIDTH}, // Bottom
            {listEntryRect.x, listEntryRect.y, BORDER_WIDTH, listEntryRect.h},                      // Left
            {listEntryRect.x + listEntryRect.w - BORDER_WIDTH, listEntryRect.y, BORDER_WIDTH, listEntryRect.h}  // Right
        };

        for (const auto& rect : borderRects) {
            SDL_FillRect(surf_display, &rect, SDL_MapRGB(surf_display->format, 255, 0, 0));
        }
	}

    //Render name
	locationNameRect = {
		listEntryRect.x + (listEntryRect.w / 2) - (locationNameSurface_cache[offset + i]->w / 2), 
		listEntryRect.y, 
		static_cast<int>(locationNameSurface_cache[offset + i]->w), 
		static_cast<int>(locationNameSurface_cache[offset + i]->h)
	};
    PokeSurface::onDrawScaled(surf_display, locationNameSurface_cache[offset + i], &locationNameRect);

    //Render  condition
    if (route[7] != "NULL") {
        double scaling = 1.5;
        conditionRect = {
            listEntryRect.x, 
            listEntryRect.y, 
            static_cast<int>(conditionSurface_cache[offset + i]->w * scaling), 
            static_cast<int>(conditionSurface_cache[offset + i]->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface_cache[offset + i], &conditionRect);
    }

    // Render method
    double scaling = 1.5;
	 methodRect = {
		listEntryRect.x, 
		(listEntryRect.y + listEntryRect.h ) - static_cast<int>(methodSurface_cache[offset + i]->h * scaling), 
		static_cast<int>(methodSurface_cache[offset + i]->w * scaling),
		static_cast<int>(methodSurface_cache[offset + i]->h * scaling)
	};
    PokeSurface::onDrawScaled(surf_display, methodSurface_cache[offset + i], &methodRect);
 
    // Render rate
    rateRect = {
        (listEntryRect.x + listEntryRect.w) - rateSurface_cache[offset + i]->w, 
        (listEntryRect.y + listEntryRect.h ) - rateSurface_cache[offset + i]->h, 
        rateSurface_cache[offset + i]->w, 
        rateSurface_cache[offset + i]->h
    };
    PokeSurface::onDraw(surf_display, rateSurface_cache[offset + i], &rateRect);

    return true;
}

void PokedexActivity_PokemonView_Location::onFreeze() {
}

PokedexActivity_PokemonView_Location* PokedexActivity_PokemonView_Location::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Location::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Location::onButtonDown(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < routes->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Location::onButtonLeft(SDL_Keycode sym, Uint16 mod){
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Location::onButtonRight(SDL_Keycode sym, Uint16 mod){
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_EVOLUTION);
}

void PokedexActivity_PokemonView_Location::onButtonA(SDL_Keycode sym, Uint16 mod){}

void PokedexActivity_PokemonView_Location::onButtonB(SDL_Keycode sym, Uint16 mod){
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Location::onButtonR(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

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
	needRedraw = true;

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