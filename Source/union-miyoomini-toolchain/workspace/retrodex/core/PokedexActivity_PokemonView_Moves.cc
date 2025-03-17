#include "PokedexActivity_PokemonView_Moves.h"
#include "PokedexActivityManager.h"
#include "utils/name_to_id.h"

PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Moves::instance;

PokedexActivity_PokemonView_Moves::PokedexActivity_PokemonView_Moves()
    : dbResults(nullptr), pokemon(nullptr), needRedraw(true), backgroundSurface(nullptr),
      listEntrySurface(nullptr), pokeIcon(nullptr), pokeName(nullptr), typeA(nullptr),
      typeB(nullptr), se_left_right(nullptr), se_up_down(nullptr), fontSurface(nullptr),
      selectedIndex(0), offset(0)
{
}

PokedexActivity_PokemonView_Moves::~PokedexActivity_PokemonView_Moves()
{
    if (se_up_down)
        Mix_FreeChunk(se_up_down);
    se_up_down = nullptr;

    if (se_left_right)
        Mix_FreeChunk(se_left_right);
    se_left_right = nullptr;
}

bool PokedexActivity_PokemonView_Moves::initSDL()
{
    try
    {
        se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
        if (!se_up_down)
        {
            std::cerr << "Warning: PokedexActivity_PokemonView_Moves::initSDL() Unable to load "
                         "sound_up_down mix! SDL Error:  "
                      << +Mix_GetError() << std::endl;
        }

        se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Warning: PokedexActivity_PokemonView_Moves::initSDL() Unable to load "
                         "sound_left_right mix! SDL Error:  "
                      << +Mix_GetError() << std::endl;
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Background Surface
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_MOVES_SCREEN_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // List Item
        listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
            0, static_cast<int>(WINDOW_WIDTH * 0.9), ITEM_HEIGHT, DEPTH, SDL_PIXELFORMAT_RGBA32);
        if (!listEntrySurface)
        {
            throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() "
                                                 "Unable to load listEntrySurface! SDL Error:  ") +
                                     SDL_GetError());
        }

        // Pokemon Icon
        AssetManager::t_asset *pokemonSpriteAsset = assetManager->getAsset(
            AssetManager::POKEMON_ICON, POKEMON_NAMETOID_MAP.at(PokedexDB::getPokemonIdentifier()));

        pokeIcon     = pokemonSpriteAsset->surface;
        pokeIconRect = pokemonSpriteAsset->size;

        pokeIconRect.x = 0;
        pokeIconRect.y = 55;

        // Pokemon Name
        pokeName = TTF_RenderUTF8_Blended(fontSurface, pokemon->getName().c_str(), COLOR);
        if (pokeName == NULL)
        {
            throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() "
                                                 "Unable to load pokeNameSurface! SDL Error:  ") +
                                     SDL_GetError());
        }

        pokeNameRect = {(pokeIconRect.x + pokeIconRect.w), pokeIconRect.y + 10, pokeName->w,
                        pokeName->h};

        // Pokemon Types
        AssetManager::t_asset *typeSpriteAsset = assetManager->getAsset(
            AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokemon->getTypes()[0]));

        typeA     = typeSpriteAsset->surface;
        typeARect = typeSpriteAsset->size;

        typeARect.x = pokeIconRect.x + pokeIconRect.w;
        typeARect.y = (pokeNameRect.y + pokeNameRect.h) + 5;

        if (pokemon->getTypes().size() > 1)
        { // Pokemnon Type 2
            typeSpriteAsset = assetManager->getAsset(
                AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokemon->getTypes()[1]));

            typeB     = typeSpriteAsset->surface;
            typeBRect = typeSpriteAsset->size;

            typeBRect.x = typeARect.x + typeARect.w + 5;
            typeBRect.y = typeARect.y;
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}
void PokedexActivity_PokemonView_Moves::print_dbResults()
{
    for (std::vector<std::string> &moves : *dbResults)
    {
        for (auto &col : moves)
        {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }
}

void PokedexActivity_PokemonView_Moves::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Moves::onActivate START \n";

    pokemon   = new Pokemon();
    dbResults = PokedexDB::executeSQL(&SQL_getPokeMovesDetail);
    print_dbResults();

    move = (*dbResults)[selectedIndex];

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cout
            << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Moves::onActivate END \n";
}

void PokedexActivity_PokemonView_Moves::onDeactivate()
{
    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate START \n";

    delete pokemon;
    pokemon = nullptr;

    selectedIndex = 0, offset = 0;

    std::cout << "PokedexActivity_PokemonView_Moves::onDeactivate END \n";
}

void PokedexActivity_PokemonView_Moves::onLoop() {}

void PokedexActivity_PokemonView_Moves::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                                 SDL_Texture *texture, TTF_Font *font,
                                                 Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        // Render Background
        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        // Render Poke Icon
        SDL_BlitSurface(pokeIcon, NULL, surf_display, &pokeIconRect);

        // Render Name
        SDL_BlitSurface(pokeName, NULL, surf_display, &pokeNameRect);

        // render poke types
        SDL_BlitSurface(typeA, NULL, surf_display, &typeARect);

        // List item types_2
        if (typeB)
        {
            SDL_BlitSurface(typeB, NULL, surf_display, &typeBRect);
        }

        for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < dbResults->size(); i++)
        {
            move = (*dbResults)[offset + i];

            if (!renderListItems(surf_display, renderer, font, i))
            {
                exit(EXIT_FAILURE);
            }
        }

        needRedraw = false;
    }
}

bool PokedexActivity_PokemonView_Moves::renderListItems(SDL_Surface *surf_display,
                                                        SDL_Renderer *renderer, TTF_Font *font,
                                                        int i)
{
    if (offset + i == selectedIndex)
    {
        if (!renderItemDetails(surf_display, font, i))
        {
            exit(EXIT_FAILURE);
        }
    }

    // Render List Entry Surface
    int spacing   = 18;
    listEntryRect = {static_cast<int>(WINDOW_WIDTH - (WINDOW_WIDTH * 0.45) - 8),
                     65 + (int(i) * (ITEM_HEIGHT + spacing)), static_cast<int>(WINDOW_WIDTH * 0.45),
                     ITEM_HEIGHT};
    SDL_BlitSurface(listEntrySurface, NULL, surf_display, &listEntryRect);

    if (offset + i == selectedIndex)
    {
        // Set the render draw color
        // Would prefer a method like this.
        // Texture gets updates by screen surface,
        // Render gets updated by texture. Any changes to either will be overwritten
        /* SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); */
        /* SDL_RenderDrawRect(renderer, &listEntryRect); */

        // Draw red border using SDL_FillRect
        // TODO: replaces with surface from image
        const int BORDER_WIDTH = 2;
        SDL_Rect borderRects[] = {
            {listEntryRect.x, listEntryRect.y, listEntryRect.w, BORDER_WIDTH}, // Top
            {listEntryRect.x, listEntryRect.y + listEntryRect.h - BORDER_WIDTH, listEntryRect.w,
             BORDER_WIDTH},                                                    // Bottom
            {listEntryRect.x, listEntryRect.y, BORDER_WIDTH, listEntryRect.h}, // Left
            {listEntryRect.x + listEntryRect.w - BORDER_WIDTH, listEntryRect.y, BORDER_WIDTH,
             listEntryRect.h} // Right
        };

        for (const auto &rect : borderRects)
        {
            SDL_FillRect(surf_display, &rect, SDL_MapRGB(surf_display->format, 255, 0, 0));
        }
    }

    // Render Move Type
    SDL_Surface *type =
        assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(move[2]))->surface;

    typeRect = {listEntryRect.x, listEntryRect.y, static_cast<int>(type->w * 1.5),
                static_cast<int>(type->h * 1.5)};

    SDL_BlitSurface(type, NULL, surf_display, &typeRect);
    /* PokeSurface::onDrawScaled(surf_display, typeSurface_cache[i + offset], &typeRect); */

    // Render Move Name
    SDL_Surface *moveName = TTF_RenderText_Solid(fontSurface, move[1].c_str(), COLOR);

    nameRect = {typeRect.x + typeRect.w + 10, typeRect.y + 5, moveName->w, moveName->h};
    SDL_BlitSurface(moveName, NULL, surf_display, &nameRect);

    // Render method
    SDL_Surface *method =
        assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(move[9]))->surface;

    methodRect = {listEntryRect.x, (listEntryRect.y + listEntryRect.h) - method->h, method->w,
                  method->h};
    SDL_BlitSurface(method, NULL, surf_display, &methodRect);

    // Render level
    if (move[9] == "level-up")
    {
        SDL_Surface *learnLevel = TTF_RenderText_Solid(fontSurface, move[8].c_str(), COLOR);
        levelRect               = {methodRect.x + methodRect.w,
                                   (listEntryRect.y + listEntryRect.h) - learnLevel->h,
                                   static_cast<int>(method->w), static_cast<int>(method->h)};

        SDL_BlitSurface(learnLevel, NULL, surf_display, &levelRect);
    }

    // Render PP
    SDL_Surface *pp = TTF_RenderText_Solid(fontSurface, move[3].c_str(), COLOR);
    ppRect          = {(listEntryRect.x + listEntryRect.w) - pp->w,
                       (listEntryRect.y + listEntryRect.h) - pp->h, pp->w, pp->h};

    SDL_BlitSurface(pp, NULL, surf_display, &ppRect);

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface *surf_display, TTF_Font *font,
                                                          int i)
{
    // Render Power
    SDL_Surface *power = TTF_RenderText_Solid(fontSurface, move[5].c_str(), COLOR);
    pwrRect            = {typeARect.x + 40, typeARect.y + 70, power->w, power->h};

    SDL_BlitSurface(power, NULL, surf_display, &pwrRect);

    // List item Category
    SDL_Surface *category =
        assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(move[4]))->surface;
    classRect = {pwrRect.x + 70, pwrRect.y, category->w, category->h};

    SDL_BlitSurface(category, NULL, surf_display, &classRect);

    // Render Accuracy
    SDL_Surface *accuracy = TTF_RenderText_Solid(fontSurface, move[6].c_str(), COLOR);
    accryRect             = {pwrRect.x, (pwrRect.y + pwrRect.h) + 10, accuracy->w, accuracy->h};

    SDL_BlitSurface(accuracy, NULL, surf_display, &accryRect);

    // Render Effect
    std::string summaryText = cleanString((*dbResults)[offset + i][7]);
    SDL_Surface *summary =
        TTF_RenderUTF8_Solid_Wrapped(fontSurface, summaryText.c_str(), COLOR, 256);
    summaryRect = {15, (WINDOW_HEIGHT / 2) + 50, summary->w, summary->h};

    SDL_BlitSurface(summary, NULL, surf_display, &summaryRect);

    return true;
}

std::string PokedexActivity_PokemonView_Moves::cleanString(std::string str)
{
    std::string replacement = " ";
    std::string cleanStr    = str;
    size_t pos              = 0;
    while ((pos = cleanStr.find("\n", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    pos = 0;
    while ((pos = cleanStr.find("\r", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    pos = 0;
    while ((pos = cleanStr.find("\f", pos)) != std::string::npos)
    {
        cleanStr.replace(pos, 1, replacement);
        pos += replacement.length();
    }

    return cleanStr;
}

void PokedexActivity_PokemonView_Moves::onFreeze()
{
    // do thing for now..
}

PokedexActivity_PokemonView_Moves *PokedexActivity_PokemonView_Moves::getInstance()
{
    return &instance;
}

void PokedexActivity_PokemonView_Moves::onButtonUp(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex > 0)
    {
        selectedIndex--;
        if (selectedIndex < offset)
        {
            offset--;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonDown(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - 1)
    {
        selectedIndex++;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset++;
        }
        Mix_PlayChannel(1, se_up_down, 0);
    }
}

void PokedexActivity_PokemonView_Moves::onButtonR(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex < dbResults->size() - MAX_VISIBLE_ITEMS)
    {
        selectedIndex += MAX_VISIBLE_ITEMS;
        if (selectedIndex - offset >= MAX_VISIBLE_ITEMS)
        {
            offset += MAX_VISIBLE_ITEMS;
            if (offset > dbResults->size() - MAX_VISIBLE_ITEMS)
            {
                offset = dbResults->size() - MAX_VISIBLE_ITEMS;
            }
        }
    }
    else
    {
        selectedIndex = dbResults->size() - 1;
        offset        = dbResults->size() - MAX_VISIBLE_ITEMS;
    }
}

void PokedexActivity_PokemonView_Moves::onButtonL(SDL_Keycode sym, Uint16 mod)
{
    needRedraw = true;

    if (selectedIndex >= MAX_VISIBLE_ITEMS)
    {
        selectedIndex -= MAX_VISIBLE_ITEMS;
        if (selectedIndex < offset)
        {
            offset -= MAX_VISIBLE_ITEMS;
            if (offset < 0)
            {
                offset = 0;
            }
        }
    }
    else
    {
        selectedIndex = 0;
        offset        = 0;
    }
}

void PokedexActivity_PokemonView_Moves::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_STATS);
}

void PokedexActivity_PokemonView_Moves::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    Mix_PlayChannel(1, se_left_right, 0);
    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_LOCATION);
}

void PokedexActivity_PokemonView_Moves::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Moves::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Moves::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}