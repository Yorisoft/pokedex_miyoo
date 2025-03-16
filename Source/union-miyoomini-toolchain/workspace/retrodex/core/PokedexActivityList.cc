#include "PokedexActivityList.h"
#include "PokedexActivityManager.h"
#include "utils/name_to_id.h"

PokedexActivityList PokedexActivityList::instance;

PokedexActivityList::PokedexActivityList()
    : selectedIndex(0), offset(0), needRedraw(true), dbResults(nullptr), backgroundSurface(nullptr),
      listBackgroundSurface_default(nullptr), listBackgroundSurface_selected(nullptr),
      se_up_down(nullptr), se_on_start(nullptr), se_on_exit(nullptr), assetManager(nullptr)
{
}

PokedexActivityList::~PokedexActivityList()
{
    // Cant delete in onDeactivate or auido will be cut short
    //  deleting here is not best practice, this will only ever be called once,
    //  onActivate/Deactivate will be called many times.
    if (se_on_start)
        Mix_FreeChunk(se_on_start);
    se_on_start = nullptr;

    if (se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;

    if (se_on_exit)
        Mix_FreeChunk(se_on_exit);
    se_on_exit = nullptr;
}

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

void PokedexActivityList::print_dbResults()
{
    for (auto &row : *dbResults)
    {
        for (auto &col : row)
        {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivityList::onActivate()
{
    std::cout << "PokedexActivityList::onActivate START \n";

    // Pokemon List DB Results
    dbResults = PokedexDB::executeSQL(&SQL_getNameAndID);
    pokemon   = (*dbResults)[selectedIndex];
    print_dbResults();

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cerr << "PokedexActivityList::onActivate - Error in initSDL(), SDL Error: "
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    Mix_PlayChannel(-1, se_on_start, 0);

    needRedraw = true;

    std::cout << "PokedexActivityList::onActivate END \n";
}

void PokedexActivityList::onDeactivate()
{
    pokemon.clear();

    selectedIndex = 0, offset = 0;
}

void PokedexActivityList::onLoop() { pokemon = (*dbResults)[selectedIndex]; }

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
             i < MAX_VISIBLE_ITEMS && static_cast<std::size_t>(offset + i) < dbResults->size(); i++)
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

void PokedexActivityList::onFreeze()
{
    // do thing for now..
}

PokedexActivityList *PokedexActivityList::getInstance() { return &instance; }

void PokedexActivityList::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        if (selectedIndex < offset)
        {
            offset--;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
    }
}

void PokedexActivityList::onButtonLeft(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonRight(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonA(SDL_Keycode sym, Uint16 mod)
{
    ////Set pokemon identifier for PokedexDB
    PokedexDB::setPokemonID(std::stoi(pokemon[0]));
    PokedexDB::setPokemonIdentifier(pokemon[1]);

    ////Call next activity
    PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivityList::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(-1, se_on_exit, 0);
    // need to find a way to call Mix_CloseChunk() without stopping the audio immediately

    ////Set pokemon identifier for PokedexDB
    PokedexActivityManager::back();
}

void PokedexActivityList::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS)
    {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += MAX_VISIBLE_ITEMS;
            // Ensure offset doesn't go out of bounds
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS)
            {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately
    }
    else
    {
        // If we exceed the last item, set selectedIndex to the last item visible
        selectedIndex = dbResults->size() - 1;
        offset        = dbResults->size() - MAX_VISIBLE_ITEMS; // Cap to last visible items
    }
}

void PokedexActivityList::onButtonL(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex >= MAX_VISIBLE_ITEMS)
    {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset)
        {
            offset -= MAX_VISIBLE_ITEMS; // Reduce offset accordingly
            // Ensure offset doesn't go negative
            if (offset < 0)
            {
                offset = 0; // Cap offset to zero
            }
        }
        // Play the sound effect
        Mix_PlayChannel(-1, se_up_down, 0);
        // need to find a way to call Mix_CloseChunk() without stopping the audio immediately
    }
    else
    {
        selectedIndex = 0; // Ensure selectedIndex doesn't go below zero
        offset        = 0; // Cap offset to zero
    }
}

void PokedexActivityList::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivityList::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
