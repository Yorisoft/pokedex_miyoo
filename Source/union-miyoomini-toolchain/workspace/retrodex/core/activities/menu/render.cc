#include "PokedexActivityMenu.hpp"

bool PokedexActivityMenu::initSDL()
{
    try
    {
        // AUDIO
        se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
        if (!se_up_down)
        {
            std::cerr << "Failed to load sound sound_up_down: " << Mix_GetError() << std::endl;
        }

        // FONT
        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_L)->font;

        // Background
        backgroundSurface =
            assetManager->getAsset(AssetManager::MISC, AssetManager::SURFACE_MAIN_MENU_BACKGROUND)
                ->surface;
        backgroundRect =
            assetManager->getAsset(AssetManager::MISC, AssetManager::SURFACE_MAIN_MENU_BACKGROUND)
                ->size;

        // List Item Background
        listEntrySurface_default =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_DEFAULT)
                ->surface;
        listEntrySurface_selected =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_SELECTED)
                ->surface;

        listEntryRect =
            assetManager
                ->getAsset(AssetManager::MISC, AssetManager::SURFACE_MENU_ITEM_BACKGROUND_DEFAULT)
                ->size;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void PokedexActivityMenu::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                   SDL_Texture *texture, TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        // List Items
        for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++)
        {
            if (!renderListItems(surf_display, i))
            {
                exit(EXIT_FAILURE);
            }
        }

        // List Items
        for (int i = 0; i < MAX_VISIBLE_ITEMS && (offset + i) < dbResults->size(); i++)
        {
            if (!renderListItems(surf_display, i))
            {
                exit(EXIT_FAILURE);
            }
        }

        needRedraw = false;
    }
}

bool PokedexActivityMenu::renderListItems(SDL_Surface *surf_display, int i)
{
    // List item background
    SDL_Surface *version =
        TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[offset + i][2].c_str(), COLOR);
    if (version == NULL)
    {
        std::cout << "Unable to load surface!"
                  << "name: " << version << " path: " << version
                  << " SDL_Error:  " << TTF_GetError();
    }

    SDL_Surface *version_highlight =
        TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[offset + i][2].c_str(), HIGHLIGHT_COLOR);
    if (version_highlight == NULL)
    {
        std::cout << "Unable to load surface!"
                  << "name: " << version_highlight << " path: " << version_highlight
                  << " SDL_Error:  " << TTF_GetError();
    }

    int leftBorder = 15;
    versionRect.x  = leftBorder + (WINDOW_WIDTH / 2) - (version->w / 2);
    versionRect.y  = (i * ITEM_HEIGHT) + (listEntryRect.h / 2) - (version->h / 2) - 10;
    versionRect.w  = version->w;
    versionRect.h  = version->h;

    listEntryRect.y = (i * ITEM_HEIGHT);

    if (offset + i == selectedIndex)
    {
        SDL_BlitSurface(listEntrySurface_selected, NULL, surf_display, &listEntryRect);
        SDL_BlitSurface(version_highlight, NULL, surf_display, &versionRect);
    }
    else
    {
        SDL_BlitSurface(listEntrySurface_default, NULL, surf_display, &listEntryRect);
        SDL_BlitSurface(version, NULL, surf_display, &versionRect);
    }

    SDL_FreeSurface(version);
    SDL_FreeSurface(version_highlight);

    return true;
}
