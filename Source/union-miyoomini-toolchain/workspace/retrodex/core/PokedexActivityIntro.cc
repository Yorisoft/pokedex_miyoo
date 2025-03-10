#include "PokedexActivityIntro.h"
#include "PokedexActivityManager.h"
#include <filesystem>
#include <fstream>

PokedexActivityIntro PokedexActivityIntro::instance;
const std::string PokedexActivityIntro::userConfigFile = "user_config";

PokedexActivityIntro::PokedexActivityIntro() {
    splashSurface = NULL;
    fileSurface = NULL;
    StartTime = SDL_GetTicks();
    logoAlpha = 0;
}

void PokedexActivityIntro::onActivate() {
	// Asset Manager
	assetManager = AssetManager::getInstance();

    // Load Simple Logo
	//==================================SPLASH ART==================================
	SDL_Surface* tempSurface = IMG_Load(SPLASH_ART_PATH.c_str());
	if (!tempSurface)
	{
        std::cout << "Unable to load image! File: " << SPLASH_ART_PATH << ".  SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
	}

	splashSurface = SDL_CreateRGBSurfaceWithFormat(
		0, 
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		tempSurface->format->BitsPerPixel,
		tempSurface->format->format
	);
	if (!splashSurface)
	{
        std::cout << "Unable to load image! File: splashSurface.  SDL Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
	}

	SDL_Rect splashRect = {
		0,
		0,
		splashSurface->w,
		splashSurface->h,
	};
    if (SDL_BlitScaled(tempSurface, NULL, splashSurface, &splashRect) != 0) {
        std::cout << "Unable to scale surface: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
    }

	SDL_FreeSurface(tempSurface);
	//=====================================================================
}

void PokedexActivityIntro::onDeactivate() {
    if (splashSurface) {
        SDL_FreeSurface(splashSurface);
        splashSurface = NULL;
    }
}

void PokedexActivityIntro::onLoop() {
    // Get the current time in milliseconds
    Uint32 currentTime = SDL_GetTicks();

    // Calculate elapsed time since last FPS calculation
    Uint32 elapsedTime = currentTime - StartTime;

    // Update logoAlpha value. Should be 255 after 3 seconds. 
    if (elapsedTime <= 3000) {
        logoAlpha = (255 * elapsedTime) / 3000; 
    }
    else {
        logoAlpha = 255;
    }

    if (static_cast<Uint32>(StartTime + 4000) < SDL_GetTicks()) {
		// Load all assets
		if(!assetManager->isDoneLoading()){
			assetManager->loadAssets(); 

			// Font
			TTF_Font* fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 14);
			if (fontSurface == NULL) {
				std::cout << "Unable to load image! fontSurface.  SDL Error: " << IMG_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}

            fileSurface = TTF_RenderUTF8_Blended_Wrapped(
                fontSurface, 
				assetManager->getFile().c_str(), 
				COLOR, 
				WINDOW_WIDTH
			);
            if (!fileSurface)
            {
				std::cout << "Unable to load image! fileSurface.  SDL Error: " << IMG_GetError() << std::endl;
				exit(EXIT_FAILURE);
			}
		}	
		else {
			// check if user config exit.
			// create if not. POKEDEX_SETTINGS handles creation of config file.
			if (!std::filesystem::exists(userConfigFile)) {
				PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
			}
			else {
				loadUserConfig(userConfigFile);
				 //set glabal variables based on user config
				PokedexDB::setLanguageID(userSettingMap["LANGUAGE"]);

				if (userSettingMap["AUDIO"] == 0) {
					Mix_CloseAudio();
				}
				else {
					if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
						printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					}
					Mix_Volume(-1, 32);
				}
				 
				 //call next activity
				PokedexActivityManager::push(APPSTATE_POKEDEX_MENU);
			}
		}
    }
}

void PokedexActivityIntro::loadUserConfig(const std::string& file_name) {
    std::ifstream inputFile;
    std::istringstream iss;
    std::string line;

    inputFile.open(file_name);
    if (inputFile.fail()) {
        std::cout << "Could not open file: " << file_name << std::endl
            << "Please provide another file name or path" << std::endl;
    }
    std::cout << "Opened file: " << file_name << '\n';

    while (std::getline(inputFile, line)) {
        iss.clear();
        iss.str(line);

        char delim;
        std::string key;
        int value;
        std::string tempValue;
        if (iss >> key >> delim >> value) {
            userSettingMap[key] = value;
        }
    }

    inputFile.close();
}

void PokedexActivityIntro::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    // Clear the display surface
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

	SDL_SetTextureAlphaMod(texture, logoAlpha); // Apply the current alpha for fade-in

	SDL_Rect splashRect = {
		0,
		0,
		splashSurface->w,
		splashSurface->h,
	};
	SDL_BlitSurface(splashSurface, NULL, surf_display, &splashRect);

    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - StartTime;

	// Start rendering progress bar after 4 seconds
    if (static_cast<Uint32>(StartTime + 4000) < SDL_GetTicks()) {
		// Draw Asset Loading Progress Bar
		SDL_Color green = {0, 255, 0, 255};
		Uint32 greenColor = SDL_MapRGB(surf_display->format, green.r, green.g, green.b);
		SDL_Rect barRect= {
			0, 
			WINDOW_HEIGHT - WINDOW_HEIGHT/4, 
			static_cast<int>(WINDOW_WIDTH * assetManager->getCurrentProgress() / 100),
			5
		};
		SDL_FillRect(surf_display, &barRect, greenColor);

		/* SDL_Rect fileRect = { */
		/* 	WINDOW_WIDTH/2 - fileSurface->w/2, */
		/* 	barRect.y + barRect.h, */
		/* 	fileSurface->w, */
		/* 	fileSurface->h, */
		/* }; */
		/* SDL_BlitSurface(fileSurface, NULL, surf_display, &fileRect); */
		/* SDL_FreeSurface(fileSurface); */
		/* fileSurface = nullptr; */
	}
}

void PokedexActivityIntro::onFreeze() {
// do nothing for now..
}

PokedexActivityIntro* PokedexActivityIntro::getInstance() {
    return &instance;
}