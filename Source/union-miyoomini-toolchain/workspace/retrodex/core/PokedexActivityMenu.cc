#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() :
dbResults(nullptr),
fontSurface(nullptr),
needRedraw(true),
sEffect(nullptr),
selectedIndex(0),
offset(0)
{
}

PokedexActivityMenu::~PokedexActivityMenu() {
}

bool PokedexActivityMenu::initSDL(){
	try{
		// AUDIO
		sEffect = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
		if (!sEffect) {
			std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
		}

		// FONT
		fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 46);
		if (!fontSurface) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load fontSurface! SDL Error:  ") + SDL_GetError());
		}
		
		// Background
		backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
		if (backgroundSurface == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load backgroundSurface! SDL Error:  ") + SDL_GetError());
		};
		backgroundRect.x = 0;
		backgroundRect.y = 0;
		backgroundRect.w = WINDOW_WIDTH;
		backgroundRect.h = WINDOW_HEIGHT;

		//List Item Background
		listEntrySurface_default = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_SELECTED);
		if (listEntrySurface_default == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load listEntrySurface_default! SDL Error:  ") + SDL_GetError());
		};

		listEntrySurface_selected = PokeSurface::onLoadImg(LIST_BACKGROUND_IMG_PATH_SELECTED);
		if (listEntrySurface_selected == NULL) {
			throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load listEntrySurface_selected! SDL Error:  ") + SDL_GetError());
		};

		// GAME NAME
		for (const auto& game_name : *dbResults) {
			SDL_Surface* normal = TTF_RenderUTF8_Solid(
				fontSurface, 
				game_name[2].c_str(), 
				COLOR
			);
			if (normal == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load game_name normal Surface! SDL Error:  ") + SDL_GetError());
			};

			SDL_Surface* highlight = TTF_RenderUTF8_Solid(
				fontSurface, 
				game_name[2].c_str(), 
				HIGHLIGHT_COLOR
			);
			if (highlight == NULL) {
				throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() Unable to load game_name highlight Surface! SDL Error:  ") + SDL_GetError());
			};
			cachedTextSurfaces.push_back(normal);
			cachedHighlightTextSurfaces.push_back(highlight);
		}
	}
	catch(const std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}
void PokedexActivityMenu::printMenuInfo(){
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (std::vector<std::string>& g : *dbResults) {
        for (auto& col : g) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    game = (*dbResults)[selectedIndex];

	cachedTextSurfaces.clear();
	cachedHighlightTextSurfaces.clear();

	if(!initSDL()){
		std::cout << "PokedexActivityMenu::onActivate - Error in initSDL(), SDL Error: " << std::endl;
		exit(EXIT_FAILURE);
	}
	needRedraw = true;

    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() {
    if (fontSurface) 
        TTF_CloseFont(fontSurface);
    fontSurface = nullptr;

    if(sEffect)
        Mix_FreeChunk(sEffect);
    sEffect = nullptr;

    if(backgroundSurface)
    	SDL_FreeSurface(backgroundSurface);
	backgroundSurface = nullptr;

    if(listEntrySurface_default)
    	SDL_FreeSurface(listEntrySurface_default);
	listEntrySurface_default = nullptr;

    if(listEntrySurface_selected)
    	SDL_FreeSurface(listEntrySurface_selected);
	listEntrySurface_selected = nullptr;

	for(auto& surface : cachedHighlightTextSurfaces){
		if(surface)
			SDL_FreeSurface(surface);
		surface = nullptr;
	}
	for(auto& surface : cachedTextSurfaces){
		if(surface)
			SDL_FreeSurface(surface);
		surface = nullptr;
	}

    game.clear();

    /* delete dbResults; */
    /* dbResults = nullptr; */

    /* COLOR = { }, HIGHLIGHT_COLOR = { }; */

    /* selectedIndex = 0, offset = 0, ITEM_HEIGHT = 0; */
}

void PokedexActivityMenu::onLoop() {
    //Set Game version and regional pokedex ID for PokedexDB
    game = (*dbResults)[selectedIndex];
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
	if(needRedraw){
		SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));
		//std::cout << "PokedexActivityMenu::onRender START \n";
		PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

		// List Items
		for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++) {
			if (!renderListItems(surf_display, i)) {
				exit(EXIT_FAILURE);
			}
		}

		needRedraw = false;
	}
}

bool PokedexActivityMenu::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    listEntryRect.x = 0;
    listEntryRect.y = (i * ITEM_HEIGHT);
    listEntryRect.w = surf_display->w;
    listEntryRect.h = ITEM_HEIGHT;

    int leftBorder = 15;
    gameVersionRect.x = leftBorder + (WINDOW_WIDTH/2) - (cachedTextSurfaces[offset + i]->w / 2);
    gameVersionRect.y = (i * ITEM_HEIGHT) + (listEntryRect.h / 2) - (cachedTextSurfaces[offset + i]->h / 2) - 10;
    gameVersionRect.w = cachedTextSurfaces[offset + i]->w;
    gameVersionRect.h = cachedTextSurfaces[offset + i]->h;

	if(offset + i == selectedIndex){
		PokeSurface::onDrawScaled(surf_display, listEntrySurface_selected, &listEntryRect);
		PokeSurface::onDrawScaled(surf_display, cachedHighlightTextSurfaces[offset + i], &gameVersionRect);
	}
	else {
		PokeSurface::onDrawScaled(surf_display, listEntrySurface_default, &listEntryRect);
		PokeSurface::onDrawScaled(surf_display, cachedTextSurfaces[offset + i], &gameVersionRect);
	}

    return true;
}

void PokedexActivityMenu::onFreeze() {
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}

void PokedexActivityMenu::onButtonUp(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < offset) {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
}

void PokedexActivityMenu::onButtonDown(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - 1) {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
}

void PokedexActivityMenu::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonA(SDL_Keycode sym, Uint16 mod) {
    PokedexDB::setVersionID(std::stoi(game[0]));
    PokedexDB::setGenerationID(std::stoi(game[5]));
    PokedexDB::setVersionGroupID(std::stoi(game[7]));

    PokedexActivityManager::push(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onButtonB(SDL_Keycode sym, Uint16 mod) {
}

void PokedexActivityMenu::onButtonR(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex < dbResults->size() - 3) {
        selectedIndex += 3;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS) {
            offset += 3;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS) {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
    else {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityMenu::onButtonL(SDL_Keycode sym, Uint16 mod) {
	needRedraw = true;

    if (selectedIndex >= 3) {
        selectedIndex -= 3;
        if (selectedIndex < offset) {
            offset -= 3; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0) {
                offset = 0;  // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(1, sEffect, 0);
    }
    else {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset = 0;  // Cap offset to zero
    }
}

void PokedexActivityMenu::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityMenu::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
