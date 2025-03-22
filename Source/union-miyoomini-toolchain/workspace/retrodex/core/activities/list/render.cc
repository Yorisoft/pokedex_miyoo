#include "PokedexActivityList.h"
#include "name_to_id.h"

bool PokedexActivityList::initSDL()
{
    try
    {
        // Sound Effects
        se_on_start = Mix_LoadWAV(SOUND_EFFECT_ON_START_PATH.c_str());
        if (!se_on_start)
        {
            std::cerr << "Failed to load sound se_on_start: " << Mix_GetError() << std::endl;
        }

        se_on_exit = Mix_LoadWAV(SOUND_EFFECT_ON_EXIT_PATH.c_str());
        if (!se_on_exit)
        {
            std::cerr << "Failed to load sound se_on_exit: " << Mix_GetError() << std::endl;
        }

        se_up_down = Mix_LoadWAV(SOUND_EFFECT_UP_DOWN.c_str());
        if (!se_up_down)
        {
            std::cerr << "Failed to load sound se_up_down: " << Mix_GetError() << std::endl;
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Background Surface
        AssetManager::t_asset *backgroundAsset =
            assetManager->getAsset(AssetManager::MISC, AssetManager::SURFACE_POKEDEX_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // List Item Background Surface
        listBackgroundSurface_default =
            assetManager
                ->getAsset(AssetManager::MISC,
                           AssetManager::SURFACE_POKEDEX_ITEM_BACKGROUND_DEFAULT)
                ->surface;

        listBackgroundSurface_selected =
            assetManager
                ->getAsset(AssetManager::MISC,
                           AssetManager::SURFACE_POKEDEX_ITEM_BACKGROUND_SELECTED)
                ->surface;

        listBackgroundRect = assetManager
                                 ->getAsset(AssetManager::MISC,
                                            AssetManager::SURFACE_POKEDEX_ITEM_BACKGROUND_DEFAULT)
                                 ->size;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void PokedexActivityList::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                   SDL_Texture *texture, TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        // Clear the display surface
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        // Render List Items
        for (int i = 0;
             i < MAX_VISIBLE_ITEMS && static_cast<std::size_t>(offset + i) < dbResults->size();
             i++)
        {
            // Render list items
            if (!renderListItems(surf_display, font, i))
            {
                exit(EXIT_FAILURE);
            }
        }
        needRedraw = false;
    }
}

bool PokedexActivityList::renderListItems(SDL_Surface *surf_display, TTF_Font *font, int i)
{
    // List item background
    listBackgroundRect = renderItemBackground(surf_display, i);

    if (offset + i == selectedIndex)
    {
        // List item sprites/icons
        if (!renderItemSprites(surf_display, i))
        {
            std::cout << "Unable to render item sprites! SDL Error: " << TTF_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // List pokemon id
    if (!renderItemEntry(surf_display, &listBackgroundRect, font, i))
    {
        std::cout << "Error in renderItemEntry! SDL Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    return true;
}

SDL_Rect PokedexActivityList::renderItemBackground(SDL_Surface *surf_display, int i)
{
    listBackgroundRect.x = static_cast<int>(WINDOW_WIDTH - (WINDOW_WIDTH * .5));
    listBackgroundRect.y = (i * ITEM_HEIGHT + 70);

    if (offset + i == selectedIndex)
    {
        SDL_BlitSurface(listBackgroundSurface_selected, NULL, surf_display, &listBackgroundRect);
    }
    else
    {
        SDL_BlitSurface(listBackgroundSurface_default, NULL, surf_display, &listBackgroundRect);
    }

    return listBackgroundRect;
}

bool PokedexActivityList::renderItemSprites(SDL_Surface *surf_display, int i)
{
    int index = offset + i;

    // pokemon Sprite
    AssetManager::t_asset *pokemonSpriteAsset =
        assetManager->getAsset(AssetManager::POKEMON_SPRITES, POKEMON_NAMETOID_MAP.at(pokemon[1]));

    pokemonSprite = pokemonSpriteAsset->surface;
    pokeEntryRect = pokemonSpriteAsset->size;

    pokeEntryRect.x = 50;
    pokeEntryRect.y = 100;

    SDL_BlitSurface(pokemonSprite, NULL, surf_display, &pokeEntryRect);

    // typeA Sprite
    AssetManager::t_asset *typeSpriteAsset =
        assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokemon[3]));

    typeA     = typeSpriteAsset->surface;
    typeARect = typeSpriteAsset->size;

    typeARect.x = 20;
    typeARect.y = pokeEntryRect.h + 220;

    SDL_BlitSurface(typeA, NULL, surf_display, &typeARect);

    // typeB Sprite
    if (pokemon[4] != "NULL")
    {
        typeSpriteAsset =
            assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokemon[4]));

        typeB     = typeSpriteAsset->surface;
        typeBRect = typeSpriteAsset->size;

        // pokemon Sprite
        typeBRect.x = 20 + typeARect.w;
        typeBRect.y = pokeEntryRect.h + 220;

        SDL_BlitSurface(typeB, NULL, surf_display, &typeBRect);
    }

    return true;
}

bool PokedexActivityList::renderItemEntry(SDL_Surface *surf_display, SDL_Rect *rect, TTF_Font *font,
                                          int i)
{
    int index = offset + i;

    if (offset + i == selectedIndex)
    {
        SDL_Surface *pokeID_highlight =
            TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[index][0].c_str(), HIGHLIGHT_COLOR);
        if (pokeID_highlight == NULL)
        {
            std::cout << "Unable to load pokeID_highlight surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        SDL_Surface *pokeName_highlight =
            TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[index][2].c_str(), HIGHLIGHT_COLOR);
        if (pokeName_highlight == NULL)
        {
            std::cout << "Unable to load pokeName_highlight surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        pokeIDRect.x = rect->x + 60;
        pokeIDRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokeID_highlight->h / 2) -
                       5; // -5 to stay above white line in background
        pokeIDRect.w = pokeID_highlight->w;
        pokeIDRect.h = pokeID_highlight->h;

        pokeNameRect.x = pokeIDRect.x + (rect->w / 2) - (pokeName_highlight->w / 2);
        pokeNameRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokeName_highlight->h / 2) -
                         5; // -5 to stay above white line in background
        pokeNameRect.w = pokeName_highlight->w;
        pokeNameRect.h = pokeName_highlight->h;

        SDL_BlitSurface(pokeID_highlight, NULL, surf_display, &pokeIDRect);
        SDL_BlitSurface(pokeName_highlight, NULL, surf_display, &pokeNameRect);

        SDL_FreeSurface(pokeID_highlight);
        SDL_FreeSurface(pokeName_highlight);
    }
    else
    {
        SDL_Surface *pokeID =
            TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[index][0].c_str(), COLOR);
        if (pokeID == NULL)
        {
            std::cout << "Unable to load pokeID surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        SDL_Surface *pokeName =
            TTF_RenderUTF8_Solid(fontSurface, (*dbResults)[index][2].c_str(), COLOR);
        if (pokeName == NULL)
        {
            std::cout << "Unable to load pokeName surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        pokeIDRect.x = rect->x + 60;
        pokeIDRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokeID->h / 2) -
                       5; // -5 to stay above white line in background
        pokeIDRect.w = pokeID->w;
        pokeIDRect.h = pokeID->h;

        pokeNameRect.x = pokeIDRect.x + (rect->w / 2) - (pokeName->w / 2);
        pokeNameRect.y = (i * ITEM_HEIGHT + 70) + (rect->h / 2) - (pokeName->h / 2) -
                         5; // -5 to stay above white line in background
        pokeNameRect.w = pokeName->w;
        pokeNameRect.h = pokeName->h;

        SDL_BlitSurface(pokeID, NULL, surf_display, &pokeIDRect);
        SDL_BlitSurface(pokeName, NULL, surf_display, &pokeNameRect);

        SDL_FreeSurface(pokeID);
        SDL_FreeSurface(pokeName);
    }

    return true;
}
