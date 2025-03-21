#include "PokedexActivitySetting.h"
#include "PokedexActivityManager.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <variant>

PokedexActivitySetting PokedexActivitySetting::instance;

const std::string PokedexActivitySetting::userConfigFile = "user_config";

PokedexActivitySetting::PokedexActivitySetting()
    : backgroundSurface(nullptr), listEntrySurface_default(nullptr),
      listEntrySurface_selected(nullptr), fontSurface(nullptr), optionItems(nullptr),
      assetManager(nullptr), needRedraw(true), selectedSettingIndex(0), selectedOptionIndex(0),
      offset(0)
{
}

PokedexActivitySetting::~PokedexActivitySetting() {}

bool PokedexActivitySetting::initSDL()
{
    try
    {
        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // BACKGROUND
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_SETTINGS_SCREEN_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // List item background selected
        listEntrySurface_default = SDL_CreateRGBSurfaceWithFormat(
            0, WINDOW_WIDTH, ITEM_HEIGHT, DEPTH, SDL_PIXELFORMAT_RGBA32);
        if (!listEntrySurface_default)
        {
            throw std::runtime_error(std::string("PokedexActivitySetting::initSDL() Unable to load "
                                                 "backgroundSurface_default! SDL Error:  ") +
                                     SDL_GetError());
        }

        listEntrySurface_selected =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_SETTINGS_LIST_ITEM_BACKGROUND)
                ->surface;

        listEntryRect =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_SETTINGS_LIST_ITEM_BACKGROUND)
                ->size;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void PokedexActivitySetting::onActivate()
{
    std::cout << "PokedexActivitySetting::onActivate START \n";

    /// initialize setting and setting options START
    // SETTINGS
    settings = new std::vector<std::string>();
    settings->push_back("LANGUAGE");
    settings->push_back("AUDIO");

    // SETTING OPTIONS: languages
    languages = PokedexDB::executeSQL(&SQL_getLanguageVersion);

    // TODO - COMMENT OUT WHEN DONE TESTING
    // languages->insert(languages->begin(), { "0", "SELECT" });

    // SETTING OPTIONS: audioOptions
    audioOptions.push_back({"0", "OFF"});
    audioOptions.push_back({"1", "ON"});

    // load all setting options into one vec
    optionItems = new std::vector<std::vector<std::vector<std::string>>>();
    optionItems->push_back(*languages);
    optionItems->push_back(audioOptions);

    /// initialize setting and setting options END

    if (!std::filesystem::exists(userConfigFile))
    {
        for (std::string &setting : *settings)
        {
            userSettingMap[setting] = 1;
        }
        setUserConfig(userConfigFile);
    }
    loadUserConfig(userConfigFile);

    /// set selected setting/settingOption START
    setting            = (*settings)[selectedSettingIndex];
    settingOptions     = ((*optionItems)[selectedSettingIndex]);
    std::string target = std::to_string(userSettingMap[setting]);

    auto it = std::find_if(
        settingOptions.begin(), 
        settingOptions.end(), 
        [&target](const std::vector<std::string>& vec) { // < -- chatGPT provided lambda :)
            // Check if the target matches the first element in the sub-vector
            return !vec.empty() && vec[0] == target;
        }
    );

    if (it != settingOptions.end())
    {
        selectedOptionIndex = std::distance(settingOptions.begin(), it);
    }

    /// set selected setting/settingOption END
    assetManager = AssetManager::getInstance();

    needRedraw = true;

    if (!initSDL())
    {
        std::cout << "PokedexActivitySetting::onActivate - Error in initSDL(), SDL Error: "
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "PokedexActivitySetting::onActivate END \n";
}

void PokedexActivitySetting::loadUserConfig(const std::string &file_name)
{
    std::ifstream inputFile;
    std::istringstream iss;
    std::string line;

    inputFile.open(file_name);
    if (inputFile.fail())
    {
        std::cout << "Could not open file: " << file_name << std::endl
                  << "Please provide another file name or path" << std::endl;
    }
    std::cout << "Opened file: " << file_name << '\n';

    while (std::getline(inputFile, line))
    {
        iss.clear();
        iss.str(line);

        char delim;
        std::string key;
        int value;
        std::string tempValue;
        if (iss >> key >> delim >> value)
        {
            userSettingMap[key] = value;
        }
    }

    inputFile.close();
}
void PokedexActivitySetting::setUserConfig(const std::string &file_name)
{
    std::ofstream outputFile;
    std::ostringstream oss;
    std::string line;

    outputFile.open(file_name);
    if (outputFile.fail())
    {
        std::cout << "Could not open file: " << file_name << std::endl
                  << "Please provide another file name or path" << std::endl;
    }
    std::cout << "Writing to file: " << file_name << std::endl;

    for (const auto &[key, value] : userSettingMap)
    {
        outputFile << key << " = " << value;
        // if (std::holds_alternative<int>(value)) {
        //     outputFile << std::get<int>(value);
        // }
        // else if (std::holds_alternative<std::string>(value)) {
        //     outputFile << std::get<std::string>(value);
        // }
        outputFile << '\n';
    }

    // Close the file
    outputFile.close();
    std::cout << "Finished writing to file: " << file_name << std::endl;
}

void PokedexActivitySetting::onDeactivate()
{
    delete settings;
    settings = nullptr;

    delete optionItems;
    optionItems = nullptr;

    audioOptions.clear();
    // fontPath.clear();

    // selectedSettingIndex = 0,
    //     offset = 0;
}

void PokedexActivitySetting::onLoop()
{
    /// set selected setting/settingOption START
    setting        = (*settings)[selectedSettingIndex];
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
    if (it != settingOptions.end())
    {
        selectedOptionIndex = std::distance(settingOptions.begin(), it);
    }

    /// set selected setting/settingOption END
}
void PokedexActivitySetting::onFreeze() {}

void PokedexActivitySetting::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                      SDL_Texture *texture, TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        // Render _PokemonView_Location Items
        // Render background
        PokeSurface::onDrawScaled(surf_display, backgroundSurface, &backgroundRect);

        //// Render List Items
        // MAX_ITEMS = 7
        for (int i = 0; i < 7 && static_cast<std::size_t>(offset + i) < settings->size(); i++)
        {
            if (!renderListItems(surf_display, font, i))
            {
                exit(EXIT_FAILURE);
            }
        }
        needRedraw = false;
    }
}

bool PokedexActivitySetting::renderListItems(SDL_Surface *surf_display, TTF_Font *font, int i)
{
    // Render ListEntry Background
    int topBorderW  = 168;
    listEntryRect.x = 42;
    listEntryRect.y = (i * ITEM_HEIGHT) + topBorderW;
    listEntryRect.w = static_cast<int>(WINDOW_WIDTH * .87);
    listEntryRect.h = ITEM_HEIGHT;

    if (offset + i == selectedSettingIndex)
    {
        // Render ListEntry Background
        SDL_BlitSurface(listEntrySurface_selected, NULL, surf_display, &listEntryRect);

        // Render Setting Name
        SDL_Surface *optionNameSurface_selected =
            TTF_RenderUTF8_Blended(fontSurface, (*settings)[i + offset].c_str(), HIGHLIGHTED_COLOR);
        if (!optionNameSurface_selected)
        {
            std::cerr << "Warning: PokedexActivitySetting::renderListItems() Unable to load "
                         "optionNameSurface_selected Surface ! SDL Error:  "
                      << TTF_GetError() << std::endl;
        };

        settingNameRect.x = listEntryRect.x + 40;
        settingNameRect.y =
            listEntryRect.y + (listEntryRect.h / 2) - (optionNameSurface_selected->h / 2);
        settingNameRect.w = optionNameSurface_selected->w;
        settingNameRect.h = optionNameSurface_selected->h;

        SDL_BlitSurface(optionNameSurface_selected, NULL, surf_display, &settingNameRect);
        SDL_FreeSurface(optionNameSurface_selected);
    }
    else
    {
        // Render ListEntry Background
        SDL_BlitSurface(listEntrySurface_default, NULL, surf_display, &listEntryRect);

        // Render Setting Name
        SDL_Surface *optionNameSurface_default =
            TTF_RenderUTF8_Blended(fontSurface, (*settings)[i + offset].c_str(), COLOR);
        if (optionNameSurface_default == NULL)
        {
            std::cerr << "Warning: PokedexActivitySetting::renderListItems() Unable to load "
                         "optionNameSurface_selected Surface ! SDL Error:  "
                      << TTF_GetError() << std::endl;
        };

        settingNameRect.x = listEntryRect.x + 40;
        settingNameRect.y =
            listEntryRect.y + (listEntryRect.h / 2) - (optionNameSurface_default->h / 2);
        settingNameRect.w = optionNameSurface_default->w;
        settingNameRect.h = optionNameSurface_default->h;

        SDL_BlitSurface(optionNameSurface_default, NULL, surf_display, &settingNameRect);
        SDL_FreeSurface(optionNameSurface_default);
    }

    /////////////////////////////////////////////////////////////////////////////
    // //// Render List Items list
    // MAX_ITEMS = 1
    for (int j = i;
         j == i && static_cast<std::size_t>(offset + j) < (*optionItems)[offset + i].size();
         j++)
    {
        if (!renderSettingOptions(surf_display, &settingNameRect, font, j))
        {
            exit(EXIT_FAILURE);
        }
    }
    return true;
}
bool PokedexActivitySetting::renderSettingOptions(SDL_Surface *surf_display, SDL_Rect *setting_rect,
                                                  TTF_Font *font, int i)
{
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
    if (it != currentOptions.end())
    {
        size_t index                = std::distance(currentOptions.begin(), it);
        std::string selectedSetting = currentOptions[index][1];

        if (offset + i == selectedSettingIndex)
        {
            SDL_Surface *settingOptionsSurface_selected =
                TTF_RenderUTF8_Blended(fontSurface, selectedSetting.c_str(), HIGHLIGHTED_COLOR);
            if (settingOptionsSurface_selected == NULL)
            {
                std::cerr
                    << "Warning: PokedexActivitySetting::renderSettingOptions() Unable to load "
                       "settingOptionsSurface_default Surface ! SDL Error:  "
                    << TTF_GetError() << std::endl;
            };

            settingOptionRect.x = (WINDOW_WIDTH / 2) + 100;
            settingOptionRect.y = setting_rect->y;
            settingOptionRect.w = settingOptionsSurface_selected->w;
            settingOptionRect.h = settingOptionsSurface_selected->h;

            SDL_BlitSurface(settingOptionsSurface_selected, NULL, surf_display, &settingOptionRect);
            SDL_FreeSurface(settingOptionsSurface_selected);
        }
        else
        {
            SDL_Surface *settingOptionsSurface_default =
                TTF_RenderUTF8_Blended(fontSurface, selectedSetting.c_str(), COLOR);
            if (settingOptionsSurface_default == NULL)
            {
                std::cerr
                    << "Warning: PokedexActivitySetting::renderSettingOptions() Unable to load "
                       "settingOptionsSurface_default Surface ! SDL Error:  "
                    << TTF_GetError() << std::endl;
            };

            settingOptionRect.x = (WINDOW_WIDTH / 2) + 100;
            settingOptionRect.y = setting_rect->y;
            settingOptionRect.w = settingOptionsSurface_default->w;
            settingOptionRect.h = settingOptionsSurface_default->h;

            SDL_BlitSurface(settingOptionsSurface_default, NULL, surf_display, &settingOptionRect);
            SDL_FreeSurface(settingOptionsSurface_default);
        }
    }
    return true;
}

PokedexActivitySetting *PokedexActivitySetting::getInstance() { return &instance; }

void PokedexActivitySetting::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedSettingIndex > 0)
    {
        selectedSettingIndex--;
        if (selectedSettingIndex < offset)
        {
            offset--;
        }
    }
}
void PokedexActivitySetting::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedSettingIndex < settings->size() - 1)
    {
        selectedSettingIndex++;
        if (selectedSettingIndex - offset >= 7)
        {
            offset++;
        }
    }
}
void PokedexActivitySetting::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedOptionIndex > 0)
    {
        userSettingMap[setting] = std::stoi(settingOptions[selectedOptionIndex - 1][0]);
    }
}
void PokedexActivitySetting::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedOptionIndex < settingOptions.size() - 1)
    {
        userSettingMap[setting] = std::stoi(settingOptions[selectedOptionIndex + 1][0]);
    }
}
void PokedexActivitySetting::onButtonA(SDL_Keycode sym, Uint16 mod)
{
    // print user selected configs to file
    setUserConfig(userConfigFile);

    // set glabal variables based on user config
    PokedexDB::setLanguageID(userSettingMap["LANGUAGE"]);

    // PokedexDB::setAudioID(userSettingMap["AUDIO"]);
    if (userSettingMap["AUDIO"] == 0)
    {
        Mix_CloseAudio();
    }
    else
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
        Mix_Volume(-1, 32);
    }

    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}
void PokedexActivitySetting::onButtonR(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonL(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}
void PokedexActivitySetting::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}