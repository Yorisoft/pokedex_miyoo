#include "PokedexActivity_PokemonView_Moves.hpp"
#include "name_to_id.hpp"

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

        pokeNameRect = {
            (pokeIconRect.x + pokeIconRect.w), pokeIconRect.y + 10, pokeName->w, pokeName->h};

        // Pokemon Types
        std::vector<std::string> pokeTypes = pokemon->getTypes();
        AssetManager::t_asset *typeSpriteAsset =
            assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokeTypes[0]));

        typeA     = typeSpriteAsset->surface;
        typeARect = typeSpriteAsset->size;

        typeARect.x = pokeIconRect.x + pokeIconRect.w;
        typeARect.y = (pokeNameRect.y + pokeNameRect.h) + 5;

        if (pokeTypes[1] != "NULL")
        { // Pokemnon Type 2
            typeSpriteAsset =
                assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokeTypes[1]));

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
                     65 + (int(i) * (ITEM_HEIGHT + spacing)),
                     static_cast<int>(WINDOW_WIDTH * 0.45),
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
            {listEntryRect.x,
             listEntryRect.y + listEntryRect.h - BORDER_WIDTH,
             listEntryRect.w,
             BORDER_WIDTH},                                                    // Bottom
            {listEntryRect.x, listEntryRect.y, BORDER_WIDTH, listEntryRect.h}, // Left
            {listEntryRect.x + listEntryRect.w - BORDER_WIDTH,
             listEntryRect.y,
             BORDER_WIDTH,
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

    typeRect = {listEntryRect.x,
                listEntryRect.y,
                static_cast<int>(type->w * 1.5),
                static_cast<int>(type->h * 1.5)};

    SDL_BlitSurface(type, NULL, surf_display, &typeRect);
    /* PokeSurface::onDrawScaled(surf_display, typeSurface_cache[i + offset], &typeRect); */

    // Render Move Name
    SDL_Surface *moveName = TTF_RenderUTF8_Blended(fontSurface, move[1].c_str(), COLOR);

    nameRect = {typeRect.x + typeRect.w + 10, typeRect.y + 5, moveName->w, moveName->h};
    SDL_BlitSurface(moveName, NULL, surf_display, &nameRect);

    // Render method
    SDL_Surface *method =
        assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(move[9]))->surface;

    methodRect = {
        listEntryRect.x, (listEntryRect.y + listEntryRect.h) - method->h, method->w, method->h};
    SDL_BlitSurface(method, NULL, surf_display, &methodRect);

    // Render level
    if (move[9] == "level-up")
    {
        SDL_Surface *learnLevel = TTF_RenderUTF8_Blended(fontSurface, move[8].c_str(), COLOR);
        levelRect               = {methodRect.x + methodRect.w,
                                   (listEntryRect.y + listEntryRect.h) - learnLevel->h,
                                   static_cast<int>(method->w),
                                   static_cast<int>(method->h)};

        SDL_BlitSurface(learnLevel, NULL, surf_display, &levelRect);
    }

    // Render PP
    SDL_Surface *pp = TTF_RenderUTF8_Blended(fontSurface, move[3].c_str(), COLOR);
    ppRect          = {(listEntryRect.x + listEntryRect.w) - pp->w,
                       (listEntryRect.y + listEntryRect.h) - pp->h,
                       pp->w,
                       pp->h};

    SDL_BlitSurface(pp, NULL, surf_display, &ppRect);

    return true;
}

bool PokedexActivity_PokemonView_Moves::renderItemDetails(SDL_Surface *surf_display, TTF_Font *font,
                                                          int i)
{
    // Render Power
    SDL_Surface *power = TTF_RenderUTF8_Blended(fontSurface, move[5].c_str(), COLOR);
    pwrRect            = {typeARect.x + 40, typeARect.y + 70, power->w, power->h};

    SDL_BlitSurface(power, NULL, surf_display, &pwrRect);

    // List item Category
    SDL_Surface *category =
        assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(move[4]))->surface;
    classRect = {pwrRect.x + 70, pwrRect.y, category->w, category->h};

    SDL_BlitSurface(category, NULL, surf_display, &classRect);

    // Render Accuracy
    SDL_Surface *accuracy = TTF_RenderUTF8_Blended(fontSurface, move[6].c_str(), COLOR);
    accryRect             = {pwrRect.x, (pwrRect.y + pwrRect.h) + 10, accuracy->w, accuracy->h};

    SDL_BlitSurface(accuracy, NULL, surf_display, &accryRect);

    // Render Effect
    std::string summaryText = cleanString((*dbResults)[offset + i][7]);
    SDL_Surface *summary =
        TTF_RenderUTF8_Blended_Wrapped(fontSurface, summaryText.c_str(), COLOR, 256);
    summaryRect = {15, (WINDOW_HEIGHT / 2) + 50, summary->w, summary->h};

    SDL_BlitSurface(summary, NULL, surf_display, &summaryRect);

    return true;
}
