#include "PokedexActivitySetting.h"
#include "PokedexActivityManager.h"
#include <algorithm>
#include <fstream>
#include <variant>
#include <filesystem>

PokedexActivitySetting PokedexActivitySetting::instance;

const std::string PokedexActivitySetting::userConfigFile = "user_config";

PokedexActivitySetting::PokedexActivitySetting() :
    backgroundSurface(nullptr),
    fontSurface(nullptr),
    listEntrySurface(nullptr),
    optionItems(nullptr),
    optionNameSurface(nullptr),
    settingOptionsSurface(nullptr),
    fontPath("res/assets/font/pokemon-frlg/pokemon-frlg.ttf"),
    selectedSettingIndex(0),
    selectedOptionIndex(0),
    offset(0),
    itemHeight(static_cast<int>(WINDOW_HEIGHT / 9))
{
    color = { 0, 0, 0 }, highlightColor = { 248, 184, 112 };

}

PokedexActivitySetting::~PokedexActivitySetting() {
}

void PokedexActivitySetting::onActivate() {
    std::cout << "PokedexActivitySetting::onActivate START \n";

    /// initialize setting and setting options START
    // SETTINGS
    settings = new std::vector<std::string>();
    settings->push_back("LANGUAGE");
    settings->push_back("AUDIO");

    // SETTING OPTIONS: languages
    languages = PokedexDB::executeSQL(&SQL_getLanguageVersion);

    // TODO - COMMENT OUT WHEN DONE TESTING
    //languages->insert(languages->begin(), { "0", "SELECT" });

    // SETTING OPTIONS: audioOptions
    audioOptions.push_back({ "0", "OFF" });
    audioOptions.push_back({ "1", "ON" });

    // load all setting options into one vec
    optionItems = new std::vector<std::vector<std::vector<std::string>>>();
    optionItems->push_back(*languages);
    optionItems->push_back(audioOptions);

/// initialize setting and setting options END 

    if (!std::filesystem::exists(userConfigFile)) {
        for (std::string& setting : *settings) {
            userSettingMap[setting] = 1;
        }
        setUserConfig(userConfigFile);
    }
    loadUserConfig(userConfigFile);


/// set selected setting/settingOption START 
    setting = (*settings)[selectedSettingIndex];
    settingOptions = ((*optionItems)[selectedSettingIndex]);
    std::string target = std::to_string(userSettingMap[setting]);

    auto it = std::find_if(
        settingOptions.begin(), 
        settingOptions.end(), 
        [&target](const std::vector<std::string>& vec) { // < -- chatGPT provided lambda :)
            // Check if the target matches the first element in the sub-vector
            return !vec.empty() && vec[0] == target;
        }
    );

    if (it != settingOptions.end()) {
        selectedOptionIndex = std::distance(settingOptions.begin(), it);
    }

/// set selected setting/settingOption END

    std::cout << "PokedexActivitySetting::onActivate END \n";
}

void PokedexActivitySetting::loadUserConfig(const std::string& file_name) {
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
void PokedexActivitySetting::setUserConfig(const std::string& file_name) {
    std::ofstream outputFile;
    std::ostringstream oss;
    std::string line;

    outputFile.open(file_name);
    if (outputFile.fail()) {
        std::cout << "Could not open file: " << file_name << std::endl
            << "Please provide another file name or path" << std::endl;
    }
    std::cout << "Writing to file: " << file_name << std::endl;

    
    for (const auto& [key, value] : userSettingMap) {
        outputFile << key << " = " << value;
        //if (std::holds_alternative<int>(value)) {
        //    outputFile << std::get<int>(value);
        //}
        //else if (std::holds_alternative<std::string>(value)) {
        //    outputFile << std::get<std::string>(value);
        //}
        outputFile << '\n';  
    }

    // Close the file
    outputFile.close();
    std::cout << "Finished writing to file: " << file_name << std::endl;
    
}

void PokedexActivitySetting::onDeactivate() {
    delete settings;
    delete optionItems;
    audioOptions.clear();
    //fontPath.clear();

    //selectedSettingIndex = 0,
    //    offset = 0;
}
void PokedexActivitySetting::onLoop() {
/// set selected setting/settingOption START 
    setting = (*settings)[selectedSettingIndex];
    settingOptions = ((*optionItems)[selectedSettingIndex]);

    std::string target = std::to_string(userSettingMap[setting]);
    auto it = std::find_if(
        settingOptions.begin(), 
        settingOptions.end(), 
        [&target](const std::vector<std::string>& vec) { // < -- chatGPT provided lambda :)
            // Check if the target matches the first element in the sub-vector
            return !vec.empty() && vec[0] == target;
        }
    );
    if (it != settingOptions.end()) {
        selectedOptionIndex = std::distance(settingOptions.begin(), it);
    }

/// set selected setting/settingOption END
}
void PokedexActivitySetting::onFreeze() {}
void PokedexActivitySetting::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, Mix_Chunk* sEffect) {
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    // Render _PokemonView_Location Items
    //Render background
    std::string backgroundImageFile = "res/assets/misc/settings_background.png";
    backgroundSurface = PokeSurface::onLoadImg(backgroundImageFile);
    if (backgroundSurface == NULL) {
        std::cout << "Unable to load surface! SDL Error: backgroundSurface " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = surf_display->w;
    backgroundRect.h = surf_display->h;

    PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);
    SDL_FreeSurface(backgroundSurface);

    //// Render List Items
    // MAX_ITEMS = 7
    for (int i = 0; i < 7 && static_cast<std::size_t>(offset + i) < settings->size(); i++) {
        if (!renderListItems(surf_display, font, i)) {
            exit(EXIT_FAILURE);
        }
    }

}

bool PokedexActivitySetting::renderListItems(SDL_Surface* surf_display, TTF_Font* font, int i) {
    if (offset + i == selectedSettingIndex) {
        std::string ListackgroundImageFile = "res/assets/misc/setting_item_background.png";
        listEntrySurface = PokeSurface::onLoadImg(ListackgroundImageFile);
        if (listEntrySurface == NULL) {
            std::cout << "Unable to render surface! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };
    }
    else {
        //List item background
        listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
            0,
            surf_display->w,
            itemHeight,
            DEPTH,
            SDL_PIXELFORMAT_RGBA32
        );
        if (!listEntrySurface) {
            std::cout << "Unable to render surface! SDL Error: listEntrySurface " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    int topBorderW = 168;
    SDL_Rect listEntryRect;
    listEntryRect.x = 42;
    listEntryRect.y = (i * itemHeight) + topBorderW;
    listEntryRect.w = static_cast<int>(surf_display->w * .87);
    listEntryRect.h = itemHeight;
    PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect);
    SDL_FreeSurface(listEntrySurface);

    /////////////////////////////////////////////////////////////////////////////

    // Render Setting Name
    optionNameSurface = TTF_RenderUTF8_Blended(
        font,
        (*settings)[offset + i].c_str(),
        offset + i == selectedSettingIndex ? highlightColor : color
    );
    if (optionNameSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: optionNameSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect settingNameRect;
    settingNameRect.x = listEntryRect.x + 40;
    settingNameRect.y = listEntryRect.y + (listEntryRect.h/2) - (optionNameSurface->h/2);
    settingNameRect.w = optionNameSurface->w;
    settingNameRect.h = optionNameSurface->h;
    PokeSurface::onDraw(surf_display, optionNameSurface, &settingNameRect);
    SDL_FreeSurface(optionNameSurface);

    /////////////////////////////////////////////////////////////////////////////
    // //// Render List Items list 
    // MAX_ITEMS = 1
    for (int j = i; j == i && static_cast<std::size_t>(offset + j) < (*optionItems)[offset + i].size(); j++) {
        if (!renderSettingOptions(surf_display, &settingNameRect, font, j)) {
            exit(EXIT_FAILURE);
        }
    }

    return true;
}
bool PokedexActivitySetting::renderSettingOptions(SDL_Surface* surf_display, SDL_Rect* setting_rect, TTF_Font* font, int i) {
    // Render Setting Option
    std::string target = std::to_string(userSettingMap[(*settings)[offset + i]]);
    const std::vector<std::vector<std::string>> currentOptions = (*optionItems)[offset + i];
    auto it = std::find_if(
        currentOptions.begin(), 
        currentOptions.end(), 
        [&target](const std::vector<std::string>& vec) { // < -- chatGPT provided lambda :)
            // Check if the target matches the first element in the sub-vector
            return !vec.empty() && vec[0] == target;
        }
    );
    if (it != currentOptions.end()) {
        size_t index = std::distance(currentOptions.begin(), it);
        std::string selectedSetting = currentOptions[index][1];

        settingOptionsSurface = TTF_RenderUTF8_Blended(
            font,
            selectedSetting.c_str(),
            offset + i == selectedSettingIndex ? highlightColor : color
        );
        if (settingOptionsSurface == NULL) {
            std::cout << "Unable to render text! SDL Error: settingOptionsSurface " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect settingOptionRect;
        settingOptionRect.x = (WINDOW_WIDTH/2) + 100;
        settingOptionRect.y = setting_rect->y;
        settingOptionRect.w = settingOptionsSurface->w;
        settingOptionRect.h = settingOptionsSurface->h;
        PokeSurface::onDraw(surf_display, settingOptionsSurface, &settingOptionRect);
        SDL_FreeSurface(settingOptionsSurface);
    }
    return true;
}

PokedexActivitySetting* PokedexActivitySetting::getInstance() {
    return &instance;
}

void PokedexActivitySetting::onButtonUp(SDL_Keycode sym, Uint16 mod) {
    if (selectedSettingIndex > 0) {
        selectedSettingIndex--;
        if (selectedSettingIndex < offset) {
            offset--;
        }
    }
}
void PokedexActivitySetting::onButtonDown(SDL_Keycode sym, Uint16 mod) {
    if (selectedSettingIndex < settings->size() - 1) {
        selectedSettingIndex++;
        if (selectedSettingIndex - offset >= 7) {
            offset++;
        }
    }
}
void PokedexActivitySetting::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    if (selectedOptionIndex > 0) {
        userSettingMap[setting] = std::stoi(settingOptions[selectedOptionIndex - 1][0]);
    }
}
void PokedexActivitySetting::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    if (selectedOptionIndex < settingOptions.size() - 1) {
        userSettingMap[setting] = std::stoi(settingOptions[selectedOptionIndex + 1][0]);
    }
}
void PokedexActivitySetting::onButtonA(SDL_Keycode sym, Uint16 mod) {
    // print user selected configs to file
    setUserConfig(userConfigFile);

    // set glabal variables based on user config
    PokedexDB::setLanguageID(userSettingMap["LANGUAGE"]);

    //PokedexDB::setAudioID(userSettingMap["AUDIO"]);
    if (userSettingMap["AUDIO"] == 0) {
        Mix_CloseAudio();
    }
    else {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
        Mix_Volume(-1, 32);
    }

    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonB(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonR(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonL(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    PokedexActivityManager::back();
}