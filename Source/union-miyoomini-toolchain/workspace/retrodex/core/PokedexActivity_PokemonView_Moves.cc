#include "PokedexActivityManager.h"
#include "PokedexActivity_PokemonView_Moves.h"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves() :
dbResults(nullptr),
pokemon(nullptr),
needRedraw(true),
backgroundSurface(nullptr), 
listEntrySurface(nullptr),
pokeIconSurface(nullptr), 
pokeNameSurface(nullptr), 
typeASurface(nullptr), 
typeBSurface(nullptr),
selectedIndex(0),
offset(0),
color({ 64, 64, 64}),
highlightColor({ 255, 0, 0 }),
itemHeight(static_cast<int>(WINDOW_HEIGHT * 0.7 / 5))
{
}

bool PokedexActivity_PokemonView_Moves::initSDL(){
	try{
		sound_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
		if (!sound_left_right) {
			std::cerr << "Warning: PokedexActivity_PokemonView_Moves::initSDL() Unable to load sound_left_right mix! SDL Error:  " << + Mix_GetError() << std::endl;
		}

		sound_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
		if (!sound_up_down) {
			std::cerr << "Warning: PokedexActivity_PokemonView_Moves::initSDL() Unable to load sound_up_down mix! SDL Error:  " << + Mix_GetError() << std::endl;
		}

		fontSurface = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
		if (fontSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load fontSurface! SDL Error:  ") + SDL_GetError());
		}
		// Background
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
		}

		backgroundRect.x = 0;
		backgroundRect.y = 0;
		backgroundRect.w = WINDOW_WIDTH;
		backgroundRect.h = WINDOW_HEIGHT;

		// List Item
		listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
			0,
			static_cast<int>(WINDOW_WIDTH * 0.9),
			itemHeight,
			DEPTH,
			SDL_PIXELFORMAT_RGBA32
		);
		if (!listEntrySurface) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load listEntrySurface! SDL Error:  ") + SDL_GetError());
		}
		
		// Pokemon Icon
		std::string pokemonIcon = 
			ICON_IMG_BASE_PATH + 
			PokedexDB::getPokemonIdentifier() + 
			".png";
		pokeIconSurface = PokeSurface::onLoadImg(pokemonIcon);
		if (pokeIconSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load pokeSurface! SDL Error:  ") + SDL_GetError());
		};

		pokeIconRect = {
			0,
			55,
			pokeIconSurface->w * 2,
			pokeIconSurface->h * 2
		};

		// Pokemon Name 
		pokeNameSurface = TTF_RenderUTF8_Blended(
			fontSurface,
			pokemon->getName().c_str(),
			color
		);
		if (pokeNameSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load pokeNameSurface! SDL Error:  ") + SDL_GetError());
		}

		pokeNameRect = {
			(pokeIconRect.x + pokeIconRect.w),
			pokeIconRect.y + 10,
			pokeNameSurface->w,
			pokeNameSurface->h
		};

		// Pokemon Types
		std::string typeA = 
			TYPE_IMG_BASE_PATH + 
			pokemon->getTypes()[0] + 
			".png";
		typeASurface = PokeSurface::onLoadImg(typeA);
		if (typeASurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load typeASurface! SDL Error:  ") + SDL_GetError());
		};

		typeARect = {
			pokeIconRect.x + pokeIconRect.w,
			(pokeNameRect.y + pokeNameRect.h ) + 5,
			typeASurface->w * 2,
			typeASurface->h * 2
		};
		
		if (pokemon->getTypes()[1] != "NULL") { 
			std::string typeB = 
				TYPE_IMG_BASE_PATH + 
				pokemon->getTypes()[1] + 
				".png";
			typeBSurface = PokeSurface::onLoadImg(typeB);
			if (typeBSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load typeBSurface! SDL Error:  ") + SDL_GetError());
			};

			typeBRect = {
				typeARect.x + typeARect.w + 5,
				typeARect.y,
				typeBSurface->w * 2,
				typeBSurface->h * 2
			};
		}

		// Moves
		for(size_t i = 0; i < dbResults->size(); i++){
			std::vector<std::string> move = (*dbResults)[i];

			std::string typePath = TYPE_IMG_BASE_PATH + (*dbResults)[i][2] + ".png";
			SDL_Surface* typeSurface = PokeSurface::onLoadImg(typePath);
			if (typeSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load typeSurface! SDL Error:  ") + SDL_GetError());
			}

			SDL_Surface* nameSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				(*dbResults)[i][1].c_str(),
				color
			);
			if (nameSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load nameSurface! SDL Error:  ") + SDL_GetError());
			}

			std::string method = METHOD_IMG_BASE_PATH + (*dbResults)[i][9] + ".png";
			SDL_Surface* methodSurface = PokeSurface::onLoadImg(method);
			if (methodSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load methodSurface! SDL Error:  ") + SDL_GetError());
			}

			if ((*dbResults)[i][9] == "level-up") {
				SDL_Surface* levelSurface = TTF_RenderUTF8_Blended(
					fontSurface,
					(*dbResults)[i][8].c_str(),
					offset + i == selectedIndex ? highlightColor : color
				);
				if (levelSurface == NULL) {
					throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load levelSurface! SDL Error:  ") + SDL_GetError());
				}

				levelSurface_cache.push_back(levelSurface);
			}

			std::string pp = (*dbResults)[i][3] + '/' + (*dbResults)[i][3];
			SDL_Surface* ppSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				pp.c_str(),
				offset + i == selectedIndex ? highlightColor : color
			);
			if (ppSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load ppSurface! SDL Error:  ") + SDL_GetError());
			};

			SDL_Surface* pwrSurface = TTF_RenderUTF8_Blended(
				fontSurface,
				((*dbResults)[i][5] == "NULL" ? "--" : (*dbResults)[i][5]).c_str(),
				color
			);
			if (pwrSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load pwrSurface! SDL Error:  ") + SDL_GetError());
			};

			 pwrRect = {
				typeARect.x + 40,
				typeARect.y + 70,
				pwrSurface->w,
				pwrSurface->h
			};

			std::string moveClass = 
				TYPE_IMG_BASE_PATH + 
				(*dbResults)[i][4] + 
				".png";
			SDL_Surface* classSurface = PokeSurface::onLoadImg(moveClass);
			if (classSurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load classSurface! SDL Error:  ") + SDL_GetError());
			};

			classRect = {
				classRect.x = pwrRect.x + 80,
				classRect.y = pwrRect.y,
				classRect.w = classSurface->w * 2,
				classRect.h = classSurface->h * 2
			};

			SDL_Surface* accrySurface = TTF_RenderUTF8_Blended(
				fontSurface,
				((*dbResults)[i][6] == "NULL" ? "--" : (*dbResults)[i][6]).c_str(),
				color
			);
			if (accrySurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load accrySurface! SDL Error:  ") + SDL_GetError());
			};

			accryRect = {
				accryRect.x = pwrRect.x, 
				accryRect.y = (pwrRect.y + pwrRect.h) + 10,
				accryRect.w = accrySurface->w, 
				accryRect.h = accrySurface->h
			};

			std::string summary = cleanString((*dbResults)[i][7]);
			SDL_Surface* summarySurface = TTF_RenderUTF8_Blended_Wrapped(
				fontSurface,
				summary.c_str(),
				color,
				295
			);
			if (summarySurface == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() Unable to load summarySurface! SDL Error:  ") + SDL_GetError());
			};

			summaryRect = {
				summaryRect.x = 15,
				summaryRect.y = (WINDOW_HEIGHT / 2 ) + 50,
				summaryRect.w = summarySurface->w,
				summaryRect.h = summarySurface->h
			};

			typeSurface_cache.push_back(typeSurface);
			nameSurface_cache.push_back(nameSurface);
			methodSurface_cache.push_back(methodSurface);
			ppSurface_cache.push_back(ppSurface);
			pwrSurface_cache.push_back(pwrSurface);
			classSurface_cache.push_back(classSurface);
			accrySurface_cache.push_back(accrySurface);
			summarySurface_cache.push_back(summarySurface);
		}
	} 
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}
void PokedexActivity_PokemonView_Moves::print_dbResults(){
    for (std::vector<std::string>&moves : *dbResults) {
        for (auto& col : moves) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivity_PokemonView_Moves::clearCachedSurfaces(){
	for(SDL_Surface* surface : typeSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : nameSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : methodSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : levelSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : ppSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: pwrSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: classSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: accrySurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: summarySurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	typeSurface_cache.clear();
	nameSurface_cache.clear();
	methodSurface_cache.clear();
	levelSurface_cache.clear();
	ppSurface_cache.clear();
	pwrSurface_cache.clear();
	classSurface_cache.clear();
	accrySurface_cache.clear();
	summarySurface_cache.clear();
}

void PokedexActivity_PokemonView_Moves::onActivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    pokemon = new Pokemon();
    dbResults = PokedexDB::executeSQL(&SQL_getPokeMovesDetail);
	print_dbResults();

	clearCachedSurfaces();
	if(!initSDL()){
		std::cout << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
	
    move = (*dbResults)[selectedIndex];

	needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate() {
    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate START \n";

	if(backgroundSurface)
		SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

	if(pokeIconSurface)
		SDL_FreeSurface(pokeIconSurface);
	pokeIconSurface = nullptr;

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

	for(SDL_Surface* surface : typeSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : nameSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : methodSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : levelSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface : ppSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: pwrSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: classSurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: accrySurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	for(SDL_Surface* surface: summarySurface_cache){
		if(surface){
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	typeSurface_cache.clear();
	nameSurface_cache.clear();
	methodSurface_cache.clear();
	levelSurface_cache.clear();
	ppSurface_cache.clear();
	pwrSurface_cache.clear();
	classSurface_cache.clear();
	accrySurface_cache.clear();
	summarySurface_cache.clear();

    delete pokemon;
    //pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {
}

void PokedexActivity_PokemonView_Moves::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
	if(needRedraw){
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    	std::cout << "Rendering backgroundSurface \n";
		// Render Background
		PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

    	std::cout << "Rendering pokeIconSurface \n";
		// Render Poke Icon
		PokeSurface::onDrawScaled(surf_display, pokeIconSurface, &pokeIconRect);
		
    	std::cout << "Rendering pokeNameSurface \n";
		// Render Name
		PokeSurface::onDraw(surf_display, pokeNameSurface, &pokeNameRect);

    	std::cout << "Rendering typeASurface \n";
		// render poke types
		PokeSurface::onDrawScaled(surf_display, typeASurface, &typeARect);

    	std::cout << "Rendering typeBSurface \n";
		//__builtin_trap();
		//List item types_2
		if (pokemon->getTypes()[1] != "NULL") { 
			PokeSurface::onDrawScaled(surf_display, typeBSurface, &typeBRect);
		}

		for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++) {
			move = (*dbResults)[offset + i];

			if (!renderListItems(surf_display, renderer, font, i)) {
				exit(EXIT_FAILURE);
			}
		}

		needRedraw = false;
	}
}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface* surf_display, SDL_Renderer* renderer, TTF_Font* font, int i) {
    if (offset + i == selectedIndex) {
        if (!renderItemDetails(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

    //Render List Entry Surface
	int spacing = 18; 
	listEntryRect = {
		static_cast<int>(WINDOW_WIDTH - (WINDOW_WIDTH * 0.45) - 8),
		65 + (int(i) * (itemHeight + spacing)),
		static_cast<int>(WINDOW_WIDTH * 0.45),
		itemHeight
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

    // Render Move Type
	typeRect= {
		listEntryRect.x,
		listEntryRect.y,
		static_cast<int>(typeSurface_cache[i + offset]->w * 1.5),
		static_cast<int>(typeSurface_cache[i + offset]->h * 1.5)
	};
    PokeSurface::onDrawScaled(surf_display, typeSurface_cache[i + offset], &typeRect);

    // Render Move Name
    nameRect = {
        typeRect.x + typeRect.w + 10,
        typeRect.y + 5,
        nameSurface_cache[i + offset]->w,
        nameSurface_cache[i + offset]->h
    };
    PokeSurface::onDrawScaled(surf_display, nameSurface_cache[i + offset], &nameRect);

    // Render method
    methodRect = {
        listEntryRect.x,
        (listEntryRect.y + listEntryRect.h ) - methodSurface_cache[i + offset]->h * static_cast<int>(1.5),
        static_cast<int>(methodSurface_cache[i + offset]->w * 1.5),
        static_cast<int>(methodSurface_cache[i + offset]->h * 1.5)
    };
    PokeSurface::onDrawScaled(surf_display, methodSurface_cache[i + offset], &methodRect);

    // Render level
    if (move[9] == "level-up") {
        levelRect = {
            methodRect.x + methodRect.w,
            (listEntryRect.y + listEntryRect.h ) - levelSurface_cache[i + offset]->h,
            static_cast<int>(levelSurface_cache[i + offset]->w),
            static_cast<int>(levelSurface_cache[i + offset]->h)
        };
        PokeSurface::onDrawScaled(surf_display, levelSurface_cache[i + offset], &levelRect);
    }

    // Render PP
    ppRect = {
        (listEntryRect.x + listEntryRect.w ) - ppSurface_cache[i + offset]->w,
        (listEntryRect.y + listEntryRect.h ) - ppSurface_cache[i + offset]->h,
        ppSurface_cache[i + offset]->w,
        ppSurface_cache[i + offset]->h
    };
    PokeSurface::onDraw(surf_display, ppSurface_cache[i + offset], &ppRect);

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface* surf_display, TTF_Font* font, int i) {
    	std::cout << "Rendering listItemDetails \n";
    // Render Power
    PokeSurface::onDraw(surf_display, pwrSurface_cache[i + offset], &pwrRect);

    //List item category/class
    PokeSurface::onDrawScaled(surf_display, classSurface_cache[i + offset], &classRect);

    // Render Accuracy
    PokeSurface::onDraw(surf_display, accrySurface_cache[i + offset], &accryRect);

    // Render Effect
    PokeSurface::onDraw(surf_display, summarySurface_cache[i + offset], &summaryRect);

    return true;
}

std::string PokedexActivity_PokemonView_Moves::cleanString(std::string str) {
	std::string replacement = " "; 
	std::string cleanStr = str;
	size_t pos = 0;
	while ((pos = cleanStr.find("\n", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = cleanStr.find("\r", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	pos = 0;
	while ((pos = cleanStr.find("\f", pos)) != std::string::npos) {
		cleanStr.replace(pos, 1, replacement);
		pos += replacement.length();
	}

	return cleanStr;

}

void PokedexActivity_PokemonView_Moves::onFreeze() {
    // do thing for now..
}

PokedexActivity_PokemonView_Moves* PokedexActivity_PokemonView_Moves::getInstance() {
    return &instance;
}

void PokedexActivity_PokemonView_Moves::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
        Mix_PlayChannel(1, sound_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonDown(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        Mix_PlayChannel(1, sound_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonR(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS) {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS;
            }
        }
    }
    else {
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; 
    }
}

void PokedexActivity_PokemonView_Moves::onButtonL(SDL_Keycode sym, Uint16 mod) {
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

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    Mix_PlayChannel(1, sound_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    Mix_PlayChannel(1, sound_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonSelect(SDL_Keycode sym, Uint16 mod){}

void PokedexActivity_PokemonView_Moves::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}