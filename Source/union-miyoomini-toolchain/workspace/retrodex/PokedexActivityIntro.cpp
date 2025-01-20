#include "PokedexActivityIntro.h"
#include "PokedexActivityManager.h"
#include <filesystem>
#include <fstream>

PokedexActivityIntro PokedexActivityIntro::instance;
const std::string PokedexActivityIntro::userConfigFile = "user_config";

PokedexActivityIntro::PokedexActivityIntro() {
    splashSurface = NULL;
    StartTime = SDL_GetTicks();
    logoAlpha = 0;
}

void PokedexActivityIntro::onActivate() {
    // Load Simple Logo
    std::string file = "res/img/splash.png";
    splashSurface = PokeSurface::onLoadImg(file);
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
    if (splashSurface) {
        SDL_SetTextureAlphaMod(texture, logoAlpha); // Apply the current alpha for fade-in
        SDL_Rect splashScreenRect = {
            0, 0, WINDOW_WIDTH, WINDOW_HEIGHT
        };
        PokeSurface::onDrawScaled(surf_display, splashSurface, &splashScreenRect);
    }
}

void PokedexActivityIntro::onFreeze() {
// do nothing for now..
}

PokedexActivityIntro* PokedexActivityIntro::getInstance() {
    return &instance;
}