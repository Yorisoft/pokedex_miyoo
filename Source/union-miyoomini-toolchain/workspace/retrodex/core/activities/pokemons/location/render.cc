#include "PokedexActivity_PokemonView_Location.h"
#include "name_to_id.h"

bool PokedexActivity_PokemonView_Location::initSDL()
{
    std::cout << "PokedexActivity_PokemonView_Location::initSDL START \n";
    try
    {
        se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                         "sound_up_down mix! SDL Error:  "
                      << +Mix_GetError() << std::endl;
        }

        se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                         "sound_left_right mix! SDL Error:  "
                      << +Mix_GetError() << std::endl;
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Background
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_LOCATION_SCREEN_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // List Entry background
        listEntrySurface = SDL_CreateRGBSurfaceWithFormat(
            0, static_cast<int>(WINDOW_WIDTH * 0.9), ITEM_HEIGHT, DEPTH, SDL_PIXELFORMAT_RGBA32);
        if (!listEntrySurface)
        {
            throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() "
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
            throw std::runtime_error(std::string("PokedexActivity_PokemonView_Location::initSDL() "
                                                 "Unable to load pokeNameSurface! SDL Error:  ") +
                                     SDL_GetError());
        };
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

    std::cout << "PokedexActivity_PokemonView_Location::initSDL END \n";
    return true;
}

void PokedexActivity_PokemonView_Location::onRender(SDL_Surface *surf_display,
                                                    SDL_Renderer *renderer, SDL_Texture *texture,
                                                    TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        try
        {
            SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));
            // Render _PokemonView_Location Items
            // Render background
            SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

            // Render icon
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

            // Render _PokemonView_Location Items
            for (int i = 0; i < MAX_VISIBLE_ITEMS && offset + i < routes->size(); i++)
            {
                route = (*routes)[offset + i];

                // Render selected Item details
                if (offset + i == selectedIndex)
                {
                    if (!renderItemDetails(surf_display, font, i))
                    {
                        throw std::runtime_error(
                            std::string("PokedexActivity_PokemonView_Location::onRender() - Failed "
                                        "at renderItemDetails! SDL Error:  ") +
                            SDL_GetError());
                    }
                }

                // Render list items
                if (!renderListItems(surf_display, renderer, font, i))
                {
                    throw std::runtime_error(
                        std::string("PokedexActivity_PokemonView_Location::onRender() Unable to "
                                    "load renderListItems! SDL Error:  ") +
                        SDL_GetError());
                }
            }

            needRedraw = false;
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

bool PokedexActivity_PokemonView_Location::renderItemDetails(SDL_Surface *surf_display,
                                                             TTF_Font *font, int i)
{
    std::string location;

    //================================== Render levels - min
    SDL_Surface *minLevel = TTF_RenderUTF8_Solid(fontSurface, route[3].c_str(), COLOR);
    if (!minLevel)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "minLevel Surface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };
    minLevelRect = {155, 175, minLevel->w, minLevel->h};

    SDL_BlitSurface(minLevel, NULL, surf_display, &minLevelRect);
    SDL_FreeSurface(minLevel);

    //================================== Render levels - max
    SDL_Surface *maxLevel = TTF_RenderUTF8_Solid(fontSurface, route[4].c_str(), COLOR);
    if (maxLevel == NULL)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "maxLevel Surface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };

    maxLevelRect = {
        minLevelRect.x, (minLevelRect.y + minLevelRect.h) + 10, maxLevel->w, maxLevel->h};

    SDL_BlitSurface(maxLevel, NULL, surf_display, &maxLevelRect);
    SDL_FreeSurface(maxLevel);

    //================================== Render location
    location = route[1];
    std::string subLocation;
    if ((subLocation = route[6]) != "NULL")
    {
        location += '\n' + subLocation;
    }

    SDL_Surface *detailLocationName =
        TTF_RenderUTF8_Blended_Wrapped(fontSurface, location.c_str(), COLOR, 295);
    if (detailLocationName == NULL)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "detailLocationName Surface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };

    detailLocationRect = {
        15, (WINDOW_HEIGHT / 2) + 60, detailLocationName->w, detailLocationName->h};
    SDL_BlitSurface(detailLocationName, NULL, surf_display, &detailLocationRect);
    SDL_FreeSurface(detailLocationName);

    //==================================  Render method
    std::string method = (*routes)[i][2];
    method             = method == "walk" ? "grass" : method;
    method             = method == "gift-egg" ? "egg" : method;
    method             = method == "surf" ? "water" : method;

    AssetManager::t_asset *methodIconAsset =
        assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(method));
    SDL_Surface *methodSurface = methodIconAsset->surface;

    int bottomBorder = 37, border = 10;
    methodRect = {detailLocationRect.x,
                  WINDOW_HEIGHT - methodSurface->h - bottomBorder,
                  static_cast<int>(methodSurface->w),
                  static_cast<int>(methodSurface->h)};

    SDL_BlitSurface(methodSurface, NULL, surf_display, &methodRect);

    //================================== Render item condition
    if (route[7] != "NULL")
    {
        AssetManager::t_asset *conditionAsset =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(route[7]));
        SDL_Surface *condition = conditionAsset->surface;

        conditionRect = {methodRect.x + methodRect.w + border,
                         WINDOW_HEIGHT - condition->h - bottomBorder,
                         static_cast<int>(condition->w),
                         static_cast<int>(condition->h)};

        SDL_BlitSurface(condition, NULL, surf_display, &conditionRect);
    }

    //==================================  Render rate
    std::string rateString = route[5] + '%';
    SDL_Surface *rate      = TTF_RenderUTF8_Solid(fontSurface, rateString.c_str(), COLOR);
    if (rate == NULL)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "rateSurface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };
    rateRect = {250, WINDOW_HEIGHT - rate->h - 20, rate->w, rate->h};

    SDL_BlitSurface(rate, NULL, surf_display, &rateRect);
    SDL_FreeSurface(rate);

    return true;
}

bool PokedexActivity_PokemonView_Location::renderListItems(SDL_Surface *surf_display,
                                                           SDL_Renderer *renderer, TTF_Font *font,
                                                           int i)
{
    // Render list item background/surface
    int spacing   = 18;
    listEntryRect = {static_cast<int>(surf_display->w - (surf_display->w * 0.45) - 8),
                     65 + (i * (ITEM_HEIGHT + spacing)),
                     static_cast<int>(surf_display->w * 0.45),
                     ITEM_HEIGHT};

    SDL_BlitSurface(listEntrySurface, NULL, surf_display, &listEntryRect);
    /* PokeSurface::onDrawScaled(surf_display, listEntrySurface, &listEntryRect); */

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

    // Render name
    std::string locationString = route[1];
    for (int i = 0; i < locationString.size(); i++)
    {
        locationString[i] = std::toupper(locationString[i]);
    }
    SDL_Surface *locationName = TTF_RenderUTF8_Blended(fontSurface, locationString.c_str(), COLOR);
    if (locationName == NULL)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "rateSurface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };

    locationNameRect = {listEntryRect.x + (listEntryRect.w / 2) - (locationName->w / 2),
                        listEntryRect.y,
                        static_cast<int>(locationName->w),
                        static_cast<int>(locationName->h)};

    SDL_BlitSurface(locationName, NULL, surf_display, &locationNameRect);
    SDL_FreeSurface(locationName);

    // Render  condition
    if (route[7] != "NULL")
    {
        AssetManager::t_asset *conditionAsset =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(route[7]));
        SDL_Surface *condition = conditionAsset->surface;

        double scaling = 1.5;
        conditionRect  = {listEntryRect.x,
                          listEntryRect.y,
                          static_cast<int>(condition->w),
                          static_cast<int>(condition->h)};

        SDL_BlitSurface(condition, NULL, surf_display, &conditionRect);
    }

    // Render method
    std::string method = route[2];
    method             = method == "walk" ? "grass" : method;
    method             = method == "gift-egg" ? "egg" : method;
    method             = method == "surf" ? "water" : method;

    AssetManager::t_asset *methodIconAsset =
        assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(method));
    SDL_Surface *methodSurface = methodIconAsset->surface;

    methodRect = {listEntryRect.x,
                  (listEntryRect.y + listEntryRect.h) - static_cast<int>(methodSurface->h),
                  static_cast<int>(methodSurface->w),
                  static_cast<int>(methodSurface->h)};

    SDL_BlitSurface(methodSurface, NULL, surf_display, &methodRect);

    // Render rate
    std::string rateString = route[5] + '%';
    SDL_Surface *rate      = TTF_RenderUTF8_Solid(fontSurface, rateString.c_str(), COLOR);
    if (rate == NULL)
    {
        std::cerr << "Warning: PokedexActivity_PokemonView_Location::initSDL() Unable to load "
                     "rateSurface ! SDL Error:  "
                  << TTF_GetError() << std::endl;
    };

    rateRect = {(listEntryRect.x + listEntryRect.w) - rate->w,
                (listEntryRect.y + listEntryRect.h) - rate->h,
                rate->w,
                rate->h};

    SDL_BlitSurface(rate, NULL, surf_display, &rateRect);
    SDL_FreeSurface(rate);

    return true;
}
