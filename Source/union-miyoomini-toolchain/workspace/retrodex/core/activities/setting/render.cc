#include "PokedexActivitySetting.h"
#include <algorithm>

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
            std::cerr << "Warning: PokedexActivitySetting::initSDL() Unable to load "
                         "optionNameSurface_selected Surface ! SDL Error:  "
                      << TTF_GetError() << std::endl;
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
