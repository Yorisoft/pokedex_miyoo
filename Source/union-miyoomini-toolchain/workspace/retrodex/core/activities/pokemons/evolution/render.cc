#include "PokedexActivity_PokemonView_Evolution.hpp"
#include "name_to_id.hpp"

bool PokedexActivity_PokemonView_Evolution::initSDL()
{
    try
    {
        se_up_down = Mix_LoadWAV(SOUND_UP_DOWN_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Failed to load sound se_up_down: " << Mix_GetError() << std::endl;
        }

        se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Failed to load sound se_left_right: " << Mix_GetError() << std::endl;
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Render background
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_EVOLUTION_SCREEN_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // ListEntry
        listEntryBackground_default =
            assetManager
                ->getAsset(AssetManager::MISC,
                           AssetManager::SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_DEFAULT)
                ->surface;

        listEntryBackground_selected =
            assetManager
                ->getAsset(AssetManager::MISC,
                           AssetManager::SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_SELECTED)
                ->surface;

        listEntryRect = assetManager
                            ->getAsset(AssetManager::MISC,
                                       AssetManager::SURFACE_EVOLUTION_LIST_ITEM_BACKGROUND_DEFAULT)
                            ->size;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void PokedexActivity_PokemonView_Evolution::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate START \n";

    pokemon  = new Pokemon();
    evoChain = pokemon->getEvolutionChain();
    printPokeInfo();

    assetManager = AssetManager::getInstance();

    evo = (*evoChain)[selectedIndex];

    if (!initSDL())
    {
        std::cerr
            << "PokedexActivity_PokemonView_Moves::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Evolution::onActivate END \n";
}

void PokedexActivity_PokemonView_Evolution::onRender(SDL_Surface *surf_display,
                                                     SDL_Renderer *renderer, SDL_Texture *texture,
                                                     TTF_Font *font, Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        for (int i = 0; i < 3 && static_cast<std::size_t>(offset + i) < evoChain->size(); i++)
        {
            if (i > 0 && (*evoChain)[offset + i][1] == (*evoChain)[offset + i - 1][1])
            { // < -- skip forms. for now..
                if (selectedIndex == evoChain->size() - 1)
                {
                    selectedIndex--;
                    if (selectedIndex - offset >= 3)
                    {
                        offset--;
                    }
                }
                continue;
            }

            // Render list items
            if (!renderListItems(surf_display, font, i))
            {
                exit(EXIT_FAILURE);
            }
        }
        needRedraw = false;
    }
}

bool PokedexActivity_PokemonView_Evolution::renderListItems(SDL_Surface *surf_display,
                                                            TTF_Font *font, int i)
{
    if (offset + i == selectedIndex)
    {
        if (!renderPokeInfo(surf_display, font, i))
        {
            std::cout << "Unable to load surface! SDL Error: renderPokeInfo " << SDL_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // ListEntry
    int spacing   = 15;
    listEntryRect = {static_cast<int>(WINDOW_WIDTH * 0.5),
                     50 + (i * (ITEM_HEIGHT + spacing)),
                     static_cast<int>(WINDOW_WIDTH * 0.5),
                     ITEM_HEIGHT + 10};

    if (offset + i == selectedIndex)
    {
        SDL_BlitSurface(listEntryBackground_selected, NULL, surf_display, &listEntryRect);
    }
    else
    {
        SDL_BlitSurface(listEntryBackground_default, NULL, surf_display, &listEntryRect);
    }

    // render poke sprite
    std::string poke = (*evoChain)[i + offset][2];

    SDL_Surface *pokeSurface =
        assetManager->getAsset(AssetManager::POKEMON_SPRITES, POKEMON_NAMETOID_MAP.at(poke))
            ->surface;

    pokeRect = {static_cast<int>((listEntryRect.x + listEntryRect.w) - pokeSurface->w * .65),
                listEntryRect.y + 10,
                static_cast<int>(pokeSurface->w * .5),
                static_cast<int>(pokeSurface->h * .5)};

    SDL_BlitScaled(pokeSurface, NULL, surf_display, &pokeRect);

    // Render Item ID
    std::stringstream formattedID;
    formattedID << std::setw(3) << std::setfill('0') << (*evoChain)[i + offset][1];
    std::string id = formattedID.str();

    SDL_Surface *idSurface = TTF_RenderUTF8_Solid(fontSurface, id.c_str(), {96, 96, 96});
    if (idSurface == NULL)
    {
        throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() "
                                             "Unable to load idSurface! SDL Error:  ") +

                                 SDL_GetError());
    };

    idRect = {listEntryRect.x + 60,
              listEntryRect.y + 18,
              static_cast<int>(idSurface->w),
              static_cast<int>(idSurface->h)};

    SDL_BlitSurface(idSurface, NULL, surf_display, &idRect);

    //// Render poke name
    std::string name         = (*evoChain)[i + offset][3];
    SDL_Surface *nameSurface = TTF_RenderUTF8_Solid(fontSurface, name.c_str(), {96, 96, 96});
    if (nameSurface == NULL)
    {
        throw std::runtime_error(std::string("PokedexActivity_PokemonView_Moves::initSDL() "
                                             "Unable to load idSurface! SDL Error:  ") +

                                 SDL_GetError());
    };

    nameRect = {listEntryRect.x + 10,
                idRect.y + idRect.h,
                static_cast<int>(nameSurface->w),
                static_cast<int>(nameSurface->h)};

    SDL_BlitSurface(nameSurface, NULL, surf_display, &nameRect);

    return true;
}

bool PokedexActivity_PokemonView_Evolution::renderPokeInfo(SDL_Surface *surf_display,
                                                           TTF_Font *font, int i)
{
    // render poke sprite
    std::string poke = (*evoChain)[i + offset][2];
    std::string path = SPRITE_IMG_BASE_PATH + poke + ".png";

    SDL_Surface *pokeSurface =
        assetManager->getAsset(AssetManager::POKEMON_SPRITES, POKEMON_NAMETOID_MAP.at(poke))
            ->surface;

    pokeRect.x = 50;
    pokeRect.y = 90;

    SDL_BlitSurface(pokeSurface, NULL, surf_display, &pokeRect);

    //// Render poke method( level or item)
    int evo_item_y = 0;

    // TRIGGER
    std::string triggerString = (*evoChain)[offset + i][4];
    SDL_Surface *trigger      = nullptr;
    if (triggerString == "NULL")
    {
        triggerString = "__";
        trigger       = TTF_RenderUTF8_Solid(fontSurface, triggerString.c_str(), {96, 96, 96});
    }
    else
    {
        trigger = assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(triggerString))
                      ->surface;
    }

    SDL_Rect triggerRect = {
        0, pokeRect.y + pokeRect.h, static_cast<int>(trigger->w), static_cast<int>(trigger->h)};

    SDL_BlitSurface(trigger, NULL, surf_display, &triggerRect);
    if (triggerString == "__")
        SDL_FreeSurface(trigger);

    evo_item_y = triggerRect.y + triggerRect.h;

    // LEVEL
    SDL_Surface *level      = nullptr;
    std::string levelString = (*evoChain)[offset + i][6];
    if (levelString != "NULL")
    {
        levelString = " + " + levelString;
        level       = TTF_RenderUTF8_Solid(fontSurface, levelString.c_str(), {96, 96, 96});
        if (level == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        int leftPadding    = 5;
        SDL_Rect levelRect = {triggerRect.x + triggerRect.w + leftPadding,
                              triggerRect.y,
                              static_cast<int>(level->w),
                              static_cast<int>(level->h)};

        SDL_BlitSurface(level, NULL, surf_display, &levelRect);
        SDL_FreeSurface(level);
    }

    // ITEMS
    // USE ITEM
    std::string useItemString = (*evoChain)[offset + i][5];
    if (useItemString != "NULL")
    {
        SDL_Surface *useItem =
            assetManager->getAsset(AssetManager::ITEMS, POKEMON_NAMETOID_MAP.at(useItemString))
                ->surface;

        int leftPadding   = 5;
        SDL_Rect itemRect = {triggerRect.x + triggerRect.w + leftPadding,
                             triggerRect.y,
                             static_cast<int>(useItem->w),
                             static_cast<int>(useItem->h)};

        SDL_BlitSurface(useItem, NULL, surf_display, &itemRect);
    }

    // TRADE ITEM
    std::string heldItemString = (*evoChain)[offset + i][9];
    if (heldItemString != "NULL")
    {
        SDL_Surface *heldItem =
            assetManager->getAsset(AssetManager::ITEMS, POKEMON_NAMETOID_MAP.at(heldItemString))
                ->surface;

        int leftPadding   = 5;
        SDL_Rect itemRect = {triggerRect.x + triggerRect.w + leftPadding,
                             triggerRect.y,
                             static_cast<int>(heldItem->w * 1.5),
                             static_cast<int>(heldItem->h * 1.5)};

        SDL_BlitSurface(heldItem, NULL, surf_display, &itemRect);
    }

    // GENDER
    std::string genderIDString = (*evoChain)[offset + i][7];
    if (genderIDString != "NULL")
    {
        SDL_Surface *gender =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(genderIDString))
                ->surface;

        int leftPadding = 5;
        SDL_Rect genderRect;
        genderRect.x = triggerRect.x;
        genderRect.y = evo_item_y;
        genderRect.h = static_cast<int>(gender->w);
        genderRect.w = static_cast<int>(gender->h);

        SDL_BlitSurface(gender, NULL, surf_display, &genderRect);

        evo_item_y = genderRect.y + genderRect.h;
    }

    // TIME
    std::string timeString = (*evoChain)[offset + i][10];
    if (timeString != "NULL")
    {
        SDL_Surface *time =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(timeString))
                ->surface;

        SDL_Rect timeRect;
        timeRect.x = pokeRect.x + pokeRect.w;
        timeRect.y = pokeRect.y + pokeRect.h;
        timeRect.h = static_cast<int>(time->w * 1.5);
        timeRect.w = static_cast<int>(time->h * 1.5);

        SDL_BlitSurface(time, NULL, surf_display, &timeRect);
    }

    // HAPPINESS
    std::string happinessLvlString = (*evoChain)[offset + i][13];
    if (happinessLvlString != "NULL")
    {
        std::string happinessIconID = "happiness";
        SDL_Surface *happinessIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(happinessIconID))
                ->surface;

        happinessLvlString = " + " + happinessLvlString;
        SDL_Surface *happinessLvl =
            TTF_RenderUTF8_Solid(fontSurface, happinessLvlString.c_str(), {96, 96, 96});
        if (happinessLvl == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect happinessIconRect;
        happinessIconRect.x = triggerRect.x;
        happinessIconRect.y = evo_item_y;
        happinessIconRect.w = static_cast<int>(happinessIcon->w);
        happinessIconRect.h = static_cast<int>(happinessIcon->h);

        SDL_Rect happinessLvlRect;
        happinessLvlRect.x = happinessIconRect.x + happinessIconRect.w;
        happinessLvlRect.y = happinessIconRect.y;
        happinessLvlRect.w = static_cast<int>(happinessLvl->w);
        happinessLvlRect.h = static_cast<int>(happinessLvl->h);

        SDL_BlitSurface(happinessIcon, NULL, surf_display, &happinessIconRect);
        SDL_BlitSurface(happinessLvl, NULL, surf_display, &happinessLvlRect);
        SDL_FreeSurface(happinessLvl);
    }

    // BEAUTY
    std::string beauty = (*evoChain)[offset + i][14];
    if (beauty != "NULL")
    {
        beauty                     = " + " + beauty;
        std::string beautyIconName = "beauty";
        SDL_Surface *beautyIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(beautyIconName))
                ->surface;

        SDL_Surface *beautyLvl = TTF_RenderUTF8_Solid(fontSurface, beauty.c_str(), {96, 96, 96});
        if (beautyLvl == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect beautyIconRect;
        beautyIconRect.x = triggerRect.x;
        beautyIconRect.y = evo_item_y;
        beautyIconRect.w = static_cast<int>(beautyIcon->w * 2);
        beautyIconRect.h = static_cast<int>(beautyIcon->h * 2);

        SDL_Rect beautyLvlRect;
        beautyLvlRect.x = beautyIconRect.x + beautyIconRect.w;
        beautyLvlRect.y = beautyIconRect.y;
        beautyLvlRect.w = static_cast<int>(beautyLvl->w);
        beautyLvlRect.h = static_cast<int>(beautyLvl->h);

        SDL_BlitSurface(beautyIcon, NULL, surf_display, &beautyIconRect);
        SDL_BlitSurface(beautyLvl, NULL, surf_display, &beautyLvlRect);
        SDL_FreeSurface(beautyLvl);
    }

    // AFFECTION
    std::string affection = (*evoChain)[offset + i][14];
    if (affection != "NULL")
    {
        affection                     = " + " + affection;
        std::string affectionIconName = "affection";
        SDL_Surface *affectionIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(affectionIconName))
                ->surface;

        SDL_Surface *affectionLvl =
            TTF_RenderUTF8_Solid(fontSurface, affection.c_str(), {96, 96, 96});
        if (affectionLvl == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect affectionIconRect;
        affectionIconRect.x = triggerRect.x;
        affectionIconRect.y = evo_item_y;
        affectionIconRect.w = static_cast<int>(affectionIcon->w * 2);
        affectionIconRect.h = static_cast<int>(affectionIcon->h * 2);

        SDL_Rect affectionLvlRect;
        affectionLvlRect.x = affectionIconRect.x + affectionIconRect.w;
        affectionLvlRect.y = affectionIconRect.y;
        affectionLvlRect.w = static_cast<int>(affectionLvl->w);
        affectionLvlRect.h = static_cast<int>(affectionLvl->h);

        SDL_BlitSurface(affectionIcon, NULL, surf_display, &affectionIconRect);
        SDL_BlitSurface(affectionLvl, NULL, surf_display, &affectionLvlRect);
        SDL_FreeSurface(affectionLvl);
    }

    // KNOWN MOVE
    std::string knownMoveName = (*evoChain)[offset + i][11];
    if (knownMoveName != "NULL")
    {
        std::string knownMoveIconName = "tutor";
        SDL_Surface *knownMoveIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(knownMoveIconName))
                ->surface;

        knownMoveName = " + " + knownMoveName;
        SDL_Surface *knownMove =
            TTF_RenderUTF8_Solid(fontSurface, knownMoveName.c_str(), {96, 96, 96});
        if (knownMove == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect knownMoveIconRect;
        knownMoveIconRect.x = triggerRect.x;
        knownMoveIconRect.y = evo_item_y;
        knownMoveIconRect.w = static_cast<int>(knownMoveIcon->w * 2);
        knownMoveIconRect.h = static_cast<int>(knownMoveIcon->h * 2);

        SDL_Rect knownMoveRect;
        knownMoveRect.x = knownMoveIconRect.x + knownMoveIconRect.w;
        knownMoveRect.y = knownMoveIconRect.y;
        knownMoveRect.w = static_cast<int>(knownMove->w);
        knownMoveRect.h = static_cast<int>(knownMove->h);

        SDL_BlitSurface(knownMoveIcon, NULL, surf_display, &knownMoveIconRect);
        SDL_BlitSurface(knownMove, NULL, surf_display, &knownMoveRect);
        SDL_FreeSurface(knownMove);
    }

    // MOVE TYPE
    std::string moveTypeName = (*evoChain)[offset + i][12];
    if (moveTypeName != "NULL")
    {
        std::string moveTypeIconName = "tutor";
        SDL_Surface *moveTypeIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(moveTypeIconName))
                ->surface;

        moveTypeName = " + " + moveTypeName;
        SDL_Surface *moveType =
            assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(moveTypeName))
                ->surface;

        /* TTF_RenderUTF8_Solid(fontSurface, moveTypeName.c_str(), {96, 96, 96}); */
        /* if (moveType == NULL) */
        /* { */
        /*     std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
         */
        /*     exit(EXIT_FAILURE); */
        /* }; */

        SDL_Rect moveTypeIconRect;
        moveTypeIconRect.x = triggerRect.x;
        moveTypeIconRect.y = evo_item_y;
        moveTypeIconRect.w = static_cast<int>(moveTypeIcon->w * 2);
        moveTypeIconRect.h = static_cast<int>(moveTypeIcon->h * 2);

        SDL_Rect moveTypeRect;
        moveTypeRect.x = moveTypeIconRect.x + moveTypeIconRect.w;
        moveTypeRect.y = moveTypeIconRect.y;
        moveTypeRect.w = static_cast<int>(moveType->w);
        moveTypeRect.h = static_cast<int>(moveType->h);

        SDL_BlitSurface(moveTypeIcon, NULL, surf_display, &moveTypeIconRect);
        SDL_BlitSurface(moveType, NULL, surf_display, &moveTypeRect);
    }

    // LOCATION
    std::string location = (*evoChain)[offset + i][8];
    if (location != "NULL")
    {
        std::string locationIconName = "location";
        SDL_Surface *locationIconSurface =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(locationIconName))
                ->surface;

        SDL_Surface *locationSurface =
            TTF_RenderUTF8_Solid(fontSurface, location.c_str(), {96, 96, 96});
        if (locationSurface == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect locationIconRect;
        locationIconRect.x = triggerRect.x;
        locationIconRect.y = evo_item_y;
        locationIconRect.w = static_cast<int>(locationIconSurface->w * 2);
        locationIconRect.h = static_cast<int>(locationIconSurface->h * 2);

        SDL_Rect locationRect;
        locationRect.x = locationIconRect.x + locationIconRect.w;
        locationRect.y = locationIconRect.y;
        locationRect.w = static_cast<int>(locationSurface->w);
        locationRect.h = static_cast<int>(locationSurface->h);

        SDL_BlitSurface(locationIconSurface, NULL, surf_display, &locationIconRect);
        SDL_BlitSurface(locationSurface, NULL, surf_display, &locationRect);
        SDL_FreeSurface(locationSurface);
    }

    // RELATIVE STATS
    std::string relativeStats = (*evoChain)[offset + i][16];
    if (relativeStats != "NULL")
    {
        std::string atkIconName = "stats-atk";
        SDL_Surface *atkIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(atkIconName))
                ->surface;

        std::string defIconName = "stats-def";
        SDL_Surface *defIcon =
            assetManager->getAsset(AssetManager::MISC, POKEMON_NAMETOID_MAP.at(defIconName))
                ->surface;

        if (relativeStats == "1")
            relativeStats = "Attack > Deffense";
        if (relativeStats == "-1")
            relativeStats = "Attack < Deffense";
        if (relativeStats == "0")
            relativeStats = "Attack = Deffense";

        SDL_Surface *relativeStat =
            TTF_RenderUTF8_Solid(fontSurface, relativeStats.c_str(), {96, 96, 96});
        if (relativeStat == NULL)
        {
            std::cerr << "Failed to load surface: SDL_GetError:" << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        };

        SDL_Rect atkIconRect;
        atkIconRect.x = triggerRect.x;
        atkIconRect.y = evo_item_y;
        atkIconRect.w = static_cast<int>(atkIcon->w * 0.7);
        atkIconRect.h = static_cast<int>(atkIcon->h * 0.7);

        SDL_Rect defIconRect;
        defIconRect.x = atkIconRect.x + atkIconRect.w + 60;
        defIconRect.y = evo_item_y;
        defIconRect.w = static_cast<int>(defIcon->w * 0.7);
        defIconRect.h = static_cast<int>(defIcon->h * 0.7);

        SDL_Rect relativeStatRect;
        relativeStatRect.x = triggerRect.x;
        relativeStatRect.y = atkIconRect.y + atkIconRect.h;
        relativeStatRect.w = static_cast<int>(relativeStat->w);
        relativeStatRect.h = static_cast<int>(relativeStat->h);

        SDL_BlitSurface(atkIcon, NULL, surf_display, &atkIconRect);
        SDL_BlitSurface(defIcon, NULL, surf_display, &defIconRect);
        SDL_BlitSurface(relativeStat, NULL, surf_display, &relativeStatRect);
        SDL_FreeSurface(relativeStat);

        evo_item_y = relativeStatRect.y + relativeStatRect.h;
    }

    return true;
}
