#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Location.h"

PokedexActivity_PokemonView_Location PokedexActivity_PokemonView_Location::instance;

PokedexActivity_PokemonView_Location::PokedexActivity_PokemonView_Location() :
pokemon(nullptr),
routes(nullptr),
needRedraw(true),
selectedIndex(0),
offset(0),
color ({ 64, 64, 64}), 
highlightColor({ 255, 0, 0 }),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
}

PokedexActivity_PokemonView_Location::~PokedexActivity_PokemonView_Location() {}

bool PokedexActivity_PokemonView_Location::initSDL() {
  try {
    sEffect = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
    if (!sEffect) {
      std::cerr << "Failed to load sound sEffect: " << Mix_GetError()
                << std::endl;
    }

    sEffect_UpDown = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
    if (!sEffect) {
      std::cerr << "Failed to load sound sEffect: " << Mix_GetError()
                << std::endl;
    }

	TTF_Font* temp_font = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
	if (temp_font == NULL) {
		std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
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
        itemHeight,
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
        temp_font,
        pokemon->getName().c_str(),
        color
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
		locationNameSurface = TTF_RenderUTF8_Blended(
			temp_font,
			location.c_str(),
			offset + i == selectedIndex ? highlightColor : color
		);
		if (locationNameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load locationNameSurface! SDL Error:  ") + SDL_GetError());
		};

		// CONDITION
		if ((*routes)[i][7] != "NULL") {
			std::string condition = (*routes)[i][7];
			std::string path = 
				METHOD_IMG_BASE_PATH + 
				condition + 
				".png";
			SDL_Surface* conditionSurface = PokeSurface::onLoadImg(path);
			if (conditionSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load conditionSurface! SDL Error:  ") + SDL_GetError());
			};

			conditionSurface_cache.push_back(conditionSurface);
		}

		// METHOD
		std::string method = (*routes)[i][2];
		method = method == "walk" ? "grass" : method;
		method = method == "gift-egg" ? "egg" : method;
		method = method == "surf" ? "water" : method;
		std::string iconPath = 
			METHOD_IMG_BASE_PATH +
			method + 
			".png";

		SDL_Surface* methodSurface = PokeSurface::onLoadImg(iconPath);
		if (methodSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load methodSurface! SDL Error:  ") + SDL_GetError());
		};

		// RATE
		std::string rate =  (*routes)[i][5] + '%';
		SDL_Surface* rateSurface = TTF_RenderUTF8_Blended(
			temp_font,
			rate.c_str(),
			offset + i == selectedIndex ? highlightColor : color
		);
		if (rateSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load rateSurface! SDL Error:  ") + SDL_GetError());
		};

		// Min Level
		SDL_Surface* minLevelSurface = TTF_RenderUTF8_Blended(
			temp_font,
			(*routes)[i][3].c_str(),
			color
		);
		if (minLevelSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load minLevelSurface! SDL Error:  ") + SDL_GetError());
		};

		// Max Level
		SDL_Surface* maxLevelSurface = TTF_RenderUTF8_Blended(
			temp_font,
			(*routes)[i][4].c_str(),
			color
		);
		if (maxLevelSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load maxLevelSurface! SDL Error:  ") + SDL_GetError());
		};

		// Detail Location Name
		location = (*routes)[i][1];
		std::string subLocation;
		if ((subLocation = (*routes)[i][6]) != "NULL") {
			location += '\n' + subLocation;
		}
		SDL_Surface* detailLocationNameSurface = TTF_RenderUTF8_Blended_Wrapped(
			temp_font,
			location.c_str(),
			offset + i == selectedIndex ? highlightColor : color,
			295
		);
		if (detailLocationNameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load detailLocationNameSurface! SDL Error:  ") + SDL_GetError());
		};

		locationNameSurface_cache.push_back(locationNameSurface);
		methodSurface_cache.push_back(methodSurface);
		rateSurface_cache.push_back(rateSurface);
		levelSurface_cache.push_back({minLevelSurface, maxLevelSurface});
		detailLocationNameSurface_cache.push_back(detailLocationNameSurface);
	}
		if(temp_font)
			TTF_CloseFont(temp_font);
		temp_font = nullptr;
  } 
  catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }

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

}

void PokedexActivity_PokemonView_Location::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Location::onActivate START \n";

    // For some reason.. pokemon needs to be created before executeSQL command...
    pokemon = new Pokemon();
    routes = pokemon->getRoutes();
    printPokeInfo();

	/* locationNameSurface_cache.clear(); */
	/* conditionSurface_cache.clear(); */
	/* methodSurface_cache.clear(); */
	/* rateSurface_cache.clear(); */
	/* levelSurface_cache.clear(); */
	/* detailLocationNameSurface_cache.clear(); */

	if(!initSDL()){
		std::cout << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: " << std::endl;
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

    //delete routes;
    //routes = nullptr;

    selectedIndex = 0, offset = 0;

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
		levelSurface_cache[i + offset].first->w,
		levelSurface_cache[i + offset].first->h
	};
    PokeSurface::onDraw(surf_display, levelSurface_cache[i + offset].first, &minLevelRect);

    // Render levels - max
	maxLevelRect = {
		minLevelRect.x, 
		(minLevelRect.y + minLevelRect.h ) + 10,
		levelSurface_cache[i + offset].second->w,
		levelSurface_cache[i + offset].second->h
	};
    PokeSurface::onDraw(surf_display, levelSurface_cache[i + offset].second, &maxLevelRect);

    // Render location
    detailLocationRect = {
        15, (WINDOW_HEIGHT / 2 ) + 60,
        detailLocationNameSurface_cache[i + offset]->w, 
		detailLocationNameSurface_cache[i + offset]->h
    };
    PokeSurface::onDraw(surf_display, detailLocationNameSurface_cache[i + offset], &detailLocationRect);

    // Render method
    double scaling = 1.5;
    int bottomBorder = 37, border = 10;
    methodRect = {
        detailLocationRect.x, 
        WINDOW_HEIGHT - methodSurface_cache[i + offset]->h - bottomBorder,
        static_cast<int>(methodSurface_cache[i + offset]->w * scaling), 
        static_cast<int>(methodSurface_cache[i + offset]->h * scaling)
    };
    PokeSurface::onDrawScaled(surf_display, methodSurface_cache[i + offset], &methodRect);

    // Render item condition
    if (route[7] != "NULL") {
        conditionRect = {
            methodRect.x + methodRect.w + border, 
            WINDOW_HEIGHT - conditionSurface_cache[i + offset]->h - bottomBorder,
            static_cast<int>(conditionSurface_cache[i + offset]->w * scaling), 
            static_cast<int>(conditionSurface_cache[i + offset]->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface_cache[i + offset], &conditionRect);
    }
 
    // Render rate 
    rateRect = {
        250, 
        WINDOW_HEIGHT - rateSurface_cache[i + offset]->h - 20,
        rateSurface_cache[i + offset]->w,
        rateSurface_cache[i + offset]->h
    };
    PokeSurface::onDraw(surf_display, rateSurface_cache[i + offset], &rateRect);

    return true;
}

bool PokedexActivity_PokemonView_Location::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    //Render list item background/surface
    int spacing = 18; 
    listEntryRect = {
        static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8), 
        65 + (i * (itemHeight + spacing)), 
        static_cast<int>(surf_display->w * 0.45), 
        itemHeight
    };
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);

    if (offset + i == selectedIndex) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Black
        SDL_RenderDrawRect(renderer, &listEntryRect);
    }

    //Render name
	locationNameRect = {
		listEntryRect.x + (listEntryRect.w / 2) - (locationNameSurface_cache[i + offset]->w / 2), 
		listEntryRect.y, 
		static_cast<int>(locationNameSurface_cache[i + offset]->w), 
		static_cast<int>(locationNameSurface_cache[i + offset]->h)
	};
    PokeSurface::onDrawScaled(surf_display, locationNameSurface_cache[i + offset], &locationNameRect);

    //Render  condition
    if (route[7] != "NULL") {
        double scaling = 1.5;
        conditionRect = {
            listEntryRect.x, 
            listEntryRect.y, 
            static_cast<int>(conditionSurface_cache[i + offset]->w * scaling), 
            static_cast<int>(conditionSurface_cache[i + offset]->h * scaling)
        };
        PokeSurface::onDrawScaled(surf_display, conditionSurface_cache[i + offset], &conditionRect);
    }

    // Render method
    double scaling = 1.5;
	 methodRect = {
		listEntryRect.x, 
		(listEntryRect.y + listEntryRect.h ) - static_cast<int>(methodSurface_cache[i + offset]->h * scaling), 
		static_cast<int>(methodSurface_cache[i + offset]->w * scaling),
		static_cast<int>(methodSurface_cache[i + offset]->h * scaling)
	};
    PokeSurface::onDrawScaled(surf_display, methodSurface_cache[i + offset], &methodRect);
 
    // Render rate
    rateRect = {
        (listEntryRect.x + listEntryRect.w) - rateSurface_cache[i + offset]->w, 
        (listEntryRect.y + listEntryRect.h ) - rateSurface_cache[i + offset]->h, 
        rateSurface_cache[i + offset]->w, 
        rateSurface_cache[i + offset]->h
    };
    PokeSurface::onDraw(surf_display, rateSurface_cache[i + offset], &rateRect);

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