#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;
bool PokedexActivityMenu::needRedraw = true;

PokedexActivityMenu::PokedexActivityMenu() :
    dbResults(nullptr),
    fontSurface(nullptr),
    sEffect(nullptr),
    selectedIndex(0),
    offset(0),
    itemHeight(0)
{
}

PokedexActivityMenu::~PokedexActivityMenu() {
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";

	PokedexActivityMenu::needRedraw = true;

    color = { 248, 248, 248 }, highlightColor = { 255, 0, 0 };
    
    itemHeight = static_cast<int>(WINDOW_HEIGHT / 5);

    fontPath = "res/assets/font/pokemon-dppt/pokemon-dppt.ttf";

	
    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (std::vector<std::string>& game : *dbResults) {
        for (auto& col : game) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
    game = (*dbResults)[selectedIndex];

    fontSurface = TTF_OpenFont(fontPath.c_str(), 46);
    if (!fontSurface) {
        std::cerr << "PokedexActivityMenu::onActivate: Failed to load font: " << TTF_GetError() << std::endl;
    }

    sEffect = Mix_LoadWAV("res/assets/sound_effects/up_down.wav");
    if (!sEffect) {
        std::cerr << "Failed to load sound sEffect: " << Mix_GetError() << std::endl;
    }

	///////////////////////////
	
	std::string 
		BACKGROUND_IMG_PATH = "res/assets/misc/menu_background.png",
		LIST_BACKGROUND_IMG_PATH = "res/assets/misc/menu_item_background_";

    // Background
    backgroundSurface = PokeSurface::onLoadImg(BACKGROUND_IMG_PATH);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: listBackgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = WINDOW_WIDTH;
    backgroundRect.h = WINDOW_HEIGHT;

    //List item background
    std::string listBackgroundImageFile_default = LIST_BACKGROUND_IMG_PATH + "default.png";
    listEntrySurface_default = PokeSurface::onLoadImg(listBackgroundImageFile_default);
    if (listEntrySurface_default == NULL) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    std::string listBackgroundImageFile_selected = LIST_BACKGROUND_IMG_PATH + "selected.png";
    listEntrySurface_selected = PokeSurface::onLoadImg(listBackgroundImageFile_selected);
    if (listEntrySurface_selected == NULL) {
        std::cout << "Unable to render text! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

	cachedTextSurfaces.clear();
	cachedHighlightTextSurfaces.clear();
	for (const auto& game_name : *dbResults) {
		SDL_Surface* normal = TTF_RenderUTF8_Solid(fontSurface, game_name[2].c_str(), color);
		if (normal == NULL) {
			std::cout << "Unable to render text! SDL Error: SDL_Surface Version Name Normal " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};

		SDL_Surface* highlight = TTF_RenderUTF8_Solid(fontSurface, game_name[2].c_str(), highlightColor);
		if (highlight == NULL) {
			std::cout << "Unable to render text! SDL Error: SDL_Surface Version Name Highlight " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		};
		cachedTextSurfaces.push_back(normal);
		cachedHighlightTextSurfaces.push_back(highlight);
	}
	///////

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

    delete dbResults;
    dbResults = nullptr;

    color = { }, highlightColor = { };

    game.clear();
    fontPath.clear();

    selectedIndex = 0, offset = 0, itemHeight = 0;
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

		PokedexActivityMenu::needRedraw = false;
	}
}

bool PokedexActivityMenu::renderListItems(SDL_Surface* surf_display, int i) {
    //List item background
    listEntryRect.x = 0;
    listEntryRect.y = (i * itemHeight);
    listEntryRect.w = surf_display->w;
    listEntryRect.h = itemHeight;

    int leftBorder = 15;
    gameVersionRect.x = leftBorder + (WINDOW_WIDTH/2) - (cachedTextSurfaces[offset + i]->w / 2);
    gameVersionRect.y = (i * itemHeight) + (listEntryRect.h / 2) - (cachedTextSurfaces[offset + i]->h / 2) - 10;
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
	PokedexActivityMenu::needRedraw = true;

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
	PokedexActivityMenu::needRedraw = true;

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
	PokedexActivityMenu::needRedraw = true;

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
	PokedexActivityMenu::needRedraw = true;

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
