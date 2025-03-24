#include "PokedexActivity_PokemonView_Info.hpp"
#include "name_to_id.hpp"

bool PokedexActivity_PokemonView_Info::initSDL()
{
    try
    {
        // make it a 3 digit
        std::stringstream formattedID;
        formattedID << std::setw(3) << std::setfill('0') << pokemon->getID();
        std::string pokeID = formattedID.str();

        std::string pokeCryPath = SOUND_EFFECT_CRY_PATH + pokeID + ' ' +
                                  ".wav"; // <- empty char is standin for form variant
        se_poke_cry = Mix_LoadWAV(pokeCryPath.c_str());
        if (!se_poke_cry)
        {
            std::cerr << "Failed to load sound pokeCry: " << Mix_GetError() << std::endl;
        }
        se_left_right = Mix_LoadWAV(SOUND_EFFECT_LEFT_RIGHT_PATH.c_str());
        if (!se_left_right)
        {
            std::cerr << "Failed to load sound se_left_right: " << Mix_GetError() << std::endl;
        }

        fontSurface =
            assetManager->getAsset(AssetManager::FONT, AssetManager::FONT_POKEMON_DPPT_M)->font;

        // Background Surface
        AssetManager::t_asset *backgroundAsset = assetManager->getAsset(
            AssetManager::MISC, AssetManager::SURFACE_INFO_SCREEN_BACKGROUND);

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

        // Pokemon Types
        std::vector<std::string> pokeTypes = pokemon->getTypes();
        AssetManager::t_asset *typeSpriteAsset =
            assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokeTypes[0]));

        typeA     = typeSpriteAsset->surface;
        typeARect = typeSpriteAsset->size;

        typeARect.x = WINDOW_WIDTH - 15 - (typeA->w) * 2;
        typeARect.y = 155;

        if (pokeTypes[1] != "NULL")
        {
            // Pokemnon Type 2
            typeSpriteAsset =
                assetManager->getAsset(AssetManager::TYPES, POKEMON_NAMETOID_MAP.at(pokeTypes[1]));

            typeB     = typeSpriteAsset->surface;
            typeBRect = typeSpriteAsset->size;

            typeBRect.x = typeARect.x + typeARect.w + 5;
            typeBRect.y = typeARect.y;
        }

        // Pokemon ID
        id = TTF_RenderUTF8_Blended(fontSurface, pokeID.c_str(), {96, 96, 96});
        if (!id)
        {
            std::cout << "Unable to load id surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        idRect.x = WINDOW_WIDTH / 2 + 125;
        idRect.y = 65;
        idRect.w = id->w;
        idRect.h = id->h;

        // Pokemon Name
        std::string pokeName = pokemon->getName();
        nameSurface          = TTF_RenderUTF8_Blended(fontSurface, pokeName.c_str(), {96, 96, 96});
        if (!nameSurface)
        {
            std::cout << "Unable to load name surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        nameRect.x = idRect.x;
        nameRect.y = idRect.y + idRect.h + 15;
        nameRect.w = static_cast<int>(nameSurface->w);
        nameRect.h = static_cast<int>(nameSurface->h);

        // Pokemon Height && Weight
        std::string pokeHeight = pokemon->getHeight();
        std::string pokeWeight = pokemon->getWeight();

        height = TTF_RenderUTF8_Blended(fontSurface, pokeHeight.c_str(), {96, 96, 96});
        if (!height)
        {
            std::cout << "Unable to load height surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        weight = TTF_RenderUTF8_Blended(fontSurface, pokeWeight.c_str(), {96, 96, 96});
        if (weight == NULL)
        {
            std::cout << "Unable to load weight surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        heightRect.x = WINDOW_WIDTH / 2 + 125;
        heightRect.y = WINDOW_HEIGHT / 2 - 35;
        heightRect.w = height->w;
        heightRect.h = height->h;

        weightRect.x = heightRect.x;
        weightRect.y = 10 + heightRect.y + heightRect.h;
        weightRect.w = weight->w;
        weightRect.h = weight->h;

        // Pokemon Gender Rate
        std::vector<double> *genderRates = pokemon->getGenderRates();
        std::stringstream iss;
        iss << (*genderRates)[1] << "/" << (*genderRates)[0];
        std::string genderRatesStr = iss.str();

        gender = TTF_RenderUTF8_Blended(fontSurface, genderRatesStr.c_str(), {96, 96, 96});
        if (!gender)
        {
            std::cout << "Unable to load gender surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        genderRect.x = weightRect.x;
        genderRect.y = 15 + weightRect.y + weightRect.h;
        genderRect.w = gender->w;
        genderRect.h = gender->h;

        // Pokemon Genus
        std::string pokeGenus = pokemon->getGenus();
        genus = TTF_RenderUTF8_Blended(fontSurface, pokeGenus.c_str(), {96, 96, 96});
        if (!genus)
        {
            std::cout << "Unable to load genus surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        genusRect.x = 10;
        genusRect.y = 60;
        genusRect.w = genus->w;
        genusRect.h = genus->h;

        // Pokemon Flavor Text
        std::string pokeFlavorText = pokemon->getFlavorText();
        flavorText =
            TTF_RenderUTF8_Blended_Wrapped(fontSurface, pokeFlavorText.c_str(), {96, 96, 96}, 620);
        if (flavorText == NULL)
        {
            std::cout << "Unable to load flavorText surface!"
                      << " SDL_Error:  " << TTF_GetError();
        };

        fTextRect.x = 25;
        fTextRect.y = WINDOW_HEIGHT / 2 + 100;
        fTextRect.w = flavorText->w;
        fTextRect.h = flavorText->h;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void PokedexActivity_PokemonView_Info::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                                SDL_Texture *texture, TTF_Font *font,
                                                Mix_Chunk *sEffect)
{
    // std::cout << "PokedexActivityMenu::onRender START \n";
    if (needRedraw)
    {
        // Clear the display surface
        SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

        SDL_BlitSurface(backgroundSurface, NULL, surf_display, &backgroundRect);

        if (!renderSprites(surf_display))
        {
            std::cout << "Unable to render item sprites! SDL Error: " << SDL_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!renderNameID(surf_display, font))
        {
            std::cout << "Unable to render item name and ID! SDL Error: " << SDL_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!renderHW(surf_display, font))
        {
            std::cout << "Unable to render item height and weight! SDL Error: " << SDL_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!renderFlavorText(surf_display, font))
        {
            std::cout << "Unable to render item flavor text! SDL Error: " << SDL_GetError()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
        needRedraw = false;
    }
}

bool PokedexActivity_PokemonView_Info::renderSprites(SDL_Surface *surf_display)
{
    // Render Pokemon Sprite
    SDL_BlitSurface(pokeSprite, NULL, surf_display, &pokeSpriteRect);

    // Render Pokemon Type 1
    SDL_BlitSurface(typeA, NULL, surf_display, &typeARect);

    // Render Pokemon Type 2
    if (pokemon->getTypes()[1] != "NULL")
    {
        SDL_BlitSurface(typeB, NULL, surf_display, &typeBRect);
    }

    return true;
}

bool PokedexActivity_PokemonView_Info::renderNameID(SDL_Surface *surf_display, TTF_Font *font)
{
    // Render Item ID
    SDL_BlitSurface(id, NULL, surf_display, &idRect);

    // Render Pokemon Name
    SDL_BlitSurface(nameSurface, NULL, surf_display, &nameRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderHW(SDL_Surface *surf_display, TTF_Font *font)
{

    // Render Pokemon Height & Weight
    SDL_BlitSurface(height, NULL, surf_display, &heightRect);
    SDL_BlitSurface(weight, NULL, surf_display, &weightRect);

    // Render Pokemon Gender Rates
    SDL_BlitSurface(gender, NULL, surf_display, &genderRect);

    return true;
}

bool PokedexActivity_PokemonView_Info::renderFlavorText(SDL_Surface *surf_display, TTF_Font *font)
{
    // Render Pokemon Genus
    SDL_BlitSurface(genus, NULL, surf_display, &genusRect);

    // Render Pokemon Flavor Text
    SDL_BlitSurface(flavorText, NULL, surf_display, &fTextRect);

    return true;
}
