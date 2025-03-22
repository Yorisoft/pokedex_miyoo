#include "PokedexActivityIntro.h"

bool PokedexActivityIntro::initSDL()
{
    try
    {
        //==================================FONT==================================
        fontSurface = TTF_OpenFont(FONT_PATH.c_str(), 18);
        if (fontSurface == NULL)
        {
            throw std::runtime_error(
                std::string(
                    "PokedexActivityIntro::initSDL() Unable to load fontSurface! SDL Error:  ") +
                SDL_GetError());
        }

        //==================================SPLASH ART==================================
        SDL_Surface *tempSurface = IMG_Load(SPLASH_ART_PATH.c_str());
        if (!tempSurface)
        {
            throw std::runtime_error(
                std::string("PokedexActivityIntro::initSDL() Unable to load tempSurface! path: " +
                            SPLASH_ART_PATH + "\n SDL Error:  ") +
                IMG_GetError());
        }

        splashSurface = SDL_CreateRGBSurfaceWithFormat(0,
                                                       WINDOW_WIDTH,
                                                       WINDOW_HEIGHT,
                                                       tempSurface->format->BitsPerPixel,
                                                       tempSurface->format->format);
        if (!splashSurface)
        {
            throw std::runtime_error(std::string("PokedexActivityIntro::initSDL() Unable to load "
                                                 "splashSurface!\n SDL Error:  ") +
                                     IMG_GetError());
        }

        SDL_Rect splashRect = {
            0,
            0,
            splashSurface->w,
            splashSurface->h,
        };
        if (SDL_BlitScaled(tempSurface, NULL, splashSurface, &splashRect) != 0)
        {
            throw std::runtime_error(std::string("PokedexActivityIntro::initSDL() Unable to scale "
                                                 "splashSurface!\n SDL Error:  ") +
                                     SDL_GetError());
        }
        SDL_FreeSurface(tempSurface);
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

void PokedexActivityIntro::onRender(SDL_Surface *surf_display, SDL_Renderer *renderer,
                                    SDL_Texture *texture, TTF_Font *font, Mix_Chunk *sEffect)
{
    SDL_FillRect(surf_display, NULL, SDL_MapRGBA(surf_display->format, 0, 0, 0, 0));

    SDL_SetTextureAlphaMod(texture, logoAlpha);

    SDL_Rect splashRect = {
        0,
        0,
        splashSurface->w,
        splashSurface->h,
    };
    SDL_BlitSurface(splashSurface, NULL, surf_display, &splashRect);

    // Start rendering progress bar after 4 seconds
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - StartTime;
    if (static_cast<Uint32>(StartTime + 4000) < SDL_GetTicks())
    {
        // Draw Asset Loading Progress Bar
        SDL_Color green   = {0, 255, 0, 255};
        Uint32 greenColor = SDL_MapRGB(surf_display->format, green.r, green.g, green.b);

        SDL_Rect barRect = {
            0,
            WINDOW_HEIGHT - WINDOW_HEIGHT / 10,
            static_cast<int>(WINDOW_WIDTH * assetManager->getCurrentProgress() / 100),
            5};
        SDL_FillRect(surf_display, &barRect, greenColor);

        if (fileSurface != nullptr)
        {
            SDL_Rect fileRect = {
                0,
                barRect.y + 10,
                fileSurface->w,
                fileSurface->h,
            };

            // Use SDL_BlitSurface instead of BlitScaled if no scaling is needed
            SDL_BlitSurface(fileSurface, NULL, surf_display, &fileRect);
            SDL_FreeSurface(fileSurface);
            fileSurface = nullptr;
        }
    }
}
