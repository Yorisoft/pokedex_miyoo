#include "PokedexActivitySetting.hpp"
#include "PokedexActivityManager.hpp"
#include "SQLStatementStore.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>

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

void PokedexActivitySetting::onActivate()
{
    std::cout << "PokedexActivitySetting::onActivate START \n";

    /// initialize setting and setting options START
    // SETTINGS
    settings = new std::vector<std::string>();
    settings->push_back("LANGUAGE");
    settings->push_back("AUDIO");

    // SETTING OPTIONS: languages
    SQLStatementStore *sql = SQLStatementStore::getInstance();
    std::string query      = sql->getQuery("get_language_versions");
    languages              = PokedexDB::executeSQL(&query);

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

PokedexActivitySetting *PokedexActivitySetting::getInstance() { return &instance; }