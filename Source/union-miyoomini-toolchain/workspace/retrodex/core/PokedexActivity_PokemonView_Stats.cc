#include "PokedexActivity_PokemonView_Stats.h"
#include "PokedexActivityManager.h"
#include "utils/name_to_id.h"

PokedexActivity_PokemonView_Stats PokedexActivity_PokemonView_Stats::instance;

PokedexActivity_PokemonView_Stats::PokedexActivity_PokemonView_Stats()
    : pokemon(nullptr), dbResults(nullptr), backgroundSurface(nullptr), pokeSprite(nullptr),
      pokeID(nullptr), pokeName(nullptr), abilitySurface(nullptr), h_abilitySurface(nullptr),
      se_left_right(nullptr), needRedraw(true) {};

PokedexActivity_PokemonView_Stats::~PokedexActivity_PokemonView_Stats()
{
    // in order to play sounds asynchrounously with activity,
    // we cant call Mix_FreeChunk immediately after playing.. should probably make seperate class
    // for this.
    if (se_left_right)
        Mix_FreeChunk(se_left_right);
    se_left_right = nullptr;
}

void PokedexActivity_PokemonView_Stats::printPokeInfo()
{
    std::vector<unsigned short> stats = pokemon->getBasicStats();

    std::cout << "ID: " << pokemon->getID() << '\n';
    std::cout << "Name: " << pokemon->getName() << '\n';
    std::cout << "Types: " << pokemon->getTypes()[0] << " | " << pokemon->getTypes()[1] << '\n';
    std::cout << "Genus: " << pokemon->getGenus() << '\n';

    std::cout << "Height: " << pokemon->getHeight() << '\"' << '\n';
    std::cout << "Weight: " << pokemon->getWeight() << " lbs." << '\n';
    std::cout << "Flavor Text: " << pokemon->getFlavorText() << '\n';
    std::cout << "HP: " << stats[0] << '\n';
    std::cout << "Attack: " << stats[1] << '\n';
    std::cout << "Defense: " << stats[2] << '\n';
    std::cout << "Special Attack: " << stats[3] << '\n';
    std::cout << "Special Deffense: " << stats[4] << '\n';
    std::cout << "Speed: " << stats[5] << '\n';
}

bool PokedexActivity_PokemonView_Stats::initSDL()
{
    std::cout << "PokedexActivity_PokemonView_Stats::initSDL END \n";

    try
    {
        se_left_right = Mix_LoadWAV(SOUND_LEFT_RIGHT_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Warning: PokedexActivity_PokemonView_Stats::initSDL() Unable to load "
                         "se_left_right mix! SDL Error:  "
                      << +Mix_GetError();
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Background Surface
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_STATS_SCREEN_BACKGROUND);

        backgroundSurface = backgroundAsset->surface;
        backgroundRect    = backgroundAsset->size;

        // Pokemon Sprite
        AssetManager::t_asset *pokemonSpriteAsset =
            assetManager->getAsset(AssetManager::POKEMON_SPRITES,
                                   POKEMON_NAMETOID_MAP.at(PokedexDB::getPokemonIdentifier()));

        pokeSprite     = pokemonSpriteAsset->surface;
        pokeSpriteRect = pokemonSpriteAsset->size;

        pokeSpriteRect.x = 50;
        pokeSpriteRect.y = 100;

        // ID
        // make it a 3 digit
        std::stringstream formattedID;
        formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
        std::string pokeIDString = formattedID.str();

        pokeID = TTF_RenderUTF8_Solid(fontSurface, pokeIDString.c_str(), {96, 96, 96});
        if (!pokeID)
        {
            std::cout << "Unable to load idSurface surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        pokeIDRect.x = 10;
        pokeIDRect.y = 60;
        pokeIDRect.w = pokeID->w;
        pokeIDRect.h = pokeID->h;

        // Pokemon Name
        pokeName = TTF_RenderUTF8_Solid(fontSurface, pokemon->getName().c_str(), {96, 96, 96});
        if (!pokeName)
        {
            std::cout << "Unable to load pokeName surface!"
                      << " SDL_Error:  " << TTF_GetError();
        }

        pokeNameRect.x = pokeIDRect.x + pokeIDRect.w + 10;
        pokeNameRect.y = pokeIDRect.y;
        pokeNameRect.w = pokeName->w;
        pokeNameRect.h = pokeName->h;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;

    std::cout << "PokedexActivity_PokemonView_Stats::initSDL END \n";
}

void PokedexActivity_PokemonView_Stats::onActivate()
{
    std::cout << "PokedexActivity_PokemonView_Stats::onActivate START \n";

    // create new pokemon object
    pokemon = new Pokemon();
    printPokeInfo();

    statNames = PokedexDB::executeSQL(&SQL_getStatNames);

    assetManager = AssetManager::getInstance();

    if (!initSDL())
    {
        std::cout
            << "PokedexActivity_PokemonView_Stats::onActivate - Error in initSDL(), SDL Error: "
            << std::endl;
        exit(EXIT_FAILURE);
    }

    needRedraw = true;

    std::cout << "PokedexActivity_PokemonView_Stats::onActivate END \n";
}

void PokedexActivity_PokemonView_Stats::onDeactivate()
{
    delete pokemon;
    pokemon = nullptr;
}

void PokedexActivity_PokemonView_Stats::onLoop() {}

void PokedexActivity_PokemonView_Stats::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                                 SDL_Texture *texture, TTF_Font *font,
                                                 Mix_Chunk *sEffect)
{
    if (needRedraw)
    {
        // Clear the display surface
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        try
        {
            // Render background
            SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

            // Render Pokemon sprites
            SDL_BlitSurface(pokeSprite, NULL, surf_display, &pokeSpriteRect);

            // Render Pokemon ID
            SDL_BlitSurface(pokeID, NULL, surf_display, &pokeIDRect);

            // Render Pokemon Name
            SDL_BlitSurface(pokeName, NULL, surf_display, &pokeNameRect);

            // Pokemon Stats
            // When ready, Evasion && Accuracy can be included as part of stats
            // To do so use statNames
            // Using the Basic stats for now.
            std::vector<unsigned short> stats = pokemon->getBasicStats();
            for (size_t i = 0; i < (*statNames).size() - 2; i++) // Skipping Accurancy && Evasion
            {
                SDL_Surface *statName =
                    TTF_RenderUTF8_Solid(fontSurface, (*statNames)[i][2].c_str(), {248, 248, 248});
                if (!statName)
                {
                    std::cout << "Unable to load statName surface!"
                              << " SDL_Error:  " << TTF_GetError();
                }

                SDL_Surface *stat = TTF_RenderUTF8_Solid(
                    fontSurface, std::to_string(stats[i]).c_str(), {96, 96, 96});
                if (!stat)
                {
                    std::cout << "Unable to load stat surface!"
                              << " SDL_Error:  " << TTF_GetError();
                }

                int topBorder = 80, spacing = 14;
                statsNameRect.x = (WINDOW_WIDTH / 2) + 20;
                statsNameRect.y = ((statName->h + 5) * i) + topBorder;
                statsNameRect.w = statName->w;
                statsNameRect.h = statName->h;

                statsRect.x = (WINDOW_WIDTH - stat->w) - 10;
                statsRect.y = (38 * i) + 80;
                statsRect.w = stat->w;
                statsRect.h = stat->h;

                SDL_BlitSurface(statName, NULL, surf_display, &statsNameRect);

                SDL_BlitSurface(stat, NULL, surf_display, &statsRect);
            }

            std::vector<std::vector<std::string>> *abilities = pokemon->getAbilities();
            if (!abilities->empty())
            {
                std::string ability = (*abilities)[0][0] + "    " + (*abilities)[0][1];
                abilitySurface =
                    TTF_RenderUTF8_Blended_Wrapped(fontSurface, ability.c_str(), {96, 96, 96}, 520);
                if (!abilitySurface)
                {
                    std::cout << "Unable to load stat surface!"
                              << " SDL_Error:  " << TTF_GetError();
                };

                int topBorder = 80, spacing = 14;
                abilityRect.x = 130;
                abilityRect.y = WINDOW_HEIGHT / 2 + 80;
                abilityRect.w = abilitySurface->w;
                abilityRect.h = abilitySurface->h;

                SDL_BlitSurface(abilitySurface, NULL, surf_display, &abilityRect);

                if (abilities->size() > 1)
                {
                    ability          = (*abilities)[1][0] + "    " + (*abilities)[1][1];
                    h_abilitySurface = TTF_RenderUTF8_Blended_Wrapped(fontSurface, ability.c_str(),
                                                                      {96, 96, 96}, 620);
                    if (h_abilitySurface == NULL)
                    {
                        std::cout << "Unable to load stat surface!"
                                  << " SDL_Error:  " << TTF_GetError();
                    };

                    h_abilityRect.x = abilityRect.x;
                    h_abilityRect.y = WINDOW_HEIGHT - 80;
                    h_abilityRect.w = h_abilitySurface->w;
                    h_abilityRect.h = h_abilitySurface->h;

                    SDL_BlitSurface(h_abilitySurface, NULL, surf_display, &h_abilityRect);
                }
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        needRedraw = false;
    }
}

void PokedexActivity_PokemonView_Stats::onFreeze() {}

PokedexActivity_PokemonView_Stats *PokedexActivity_PokemonView_Stats::getInstance()
{
    return &instance;
}

void PokedexActivity_PokemonView_Stats::onButtonUp(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonDown(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonLeft(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_INFO);
}

void PokedexActivity_PokemonView_Stats::onButtonRight(SDL_Keycode sym, Uint16 mod)
{
    // Play the sound effect
    Mix_PlayChannel(1, se_left_right, 0);

    PokedexActivityManager::replace(APPSTATE_POKEMON_VIEW_MOVES);
    // PokedexActivityManager::push(APPSTATE_POKEMON_VIEW_MOVES);
}

void PokedexActivity_PokemonView_Stats::onButtonA(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonB(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::back();
}

void PokedexActivity_PokemonView_Stats::onButtonR(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonL(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonSelect(SDL_Keycode sym, Uint16 mod) {}

void PokedexActivity_PokemonView_Stats::onButtonStart(SDL_Keycode sym, Uint16 mod)
{
    PokedexActivityManager::push(APPSTATE_POKEDEX_SETTING);
}
