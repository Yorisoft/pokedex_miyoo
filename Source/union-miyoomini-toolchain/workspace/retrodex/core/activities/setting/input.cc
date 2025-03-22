#include "PokedexActivityManager.h"
#include "PokedexActivitySetting.h"

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
