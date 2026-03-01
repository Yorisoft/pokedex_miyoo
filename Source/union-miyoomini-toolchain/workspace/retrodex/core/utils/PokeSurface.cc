#include "PokeSurface.hpp"

PokeSurface::PokeSurface() {}

SDL_Surface *PokeSurface::onLoadImg(std::string &file)
{
    SDL_Surface *tempSurface      = NULL;
    SDL_Surface *optimizedSurface = NULL;

    tempSurface = IMG_Load(file.c_str());
    if (tempSurface == NULL)
    {
        std::cout << "Unable to load image! File: " << file << ".  SDL Error: " << IMG_GetError()
                  << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create RGB565 surface pre-filled with magenta (color key)
    SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
        0, tempSurface->w, tempSurface->h, 16, SDL_PIXELFORMAT_RGB565);
    
    // Fill with magenta
    SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 255, 0, 255));
    
    // Blit the original onto it (alpha pixels won't overwrite magenta)
    SDL_BlitSurface(tempSurface, NULL, rgb565, NULL);
    
    // Set magenta as color key
    SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 255, 0, 255));
    
    SDL_FreeSurface(tempSurface);
    return rgb565;

    // if (optimizedSurface == NULL)
    // {
    //     std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // return optimizedSurface;
}

SDL_Surface *PokeSurface::onLoadImg(const std::string &file)
{
    SDL_Surface *tempSurface      = NULL;
    SDL_Surface *optimizedSurface = NULL;

    tempSurface = IMG_Load(file.c_str());
    if (tempSurface == NULL)
    {
        std::cout << "Unable to load image! File: " << file << ".  SDL Error: " << IMG_GetError()
                  << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

   // Create RGB565 surface pre-filled with magenta (color key)
   SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
       0, tempSurface->w, tempSurface->h, 16, SDL_PIXELFORMAT_RGB565);
   
   // Fill with magenta
   SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   // Blit the original onto it (alpha pixels won't overwrite magenta)
   SDL_BlitSurface(tempSurface, NULL, rgb565, NULL);
   
   // Set magenta as color key
   SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   SDL_FreeSurface(tempSurface);
   return rgb565;

    // if (optimizedSurface == NULL)
    // {
    //     std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // return optimizedSurface;
}

SDL_Surface* PokeSurface::onLoadImgScaled(const std::string &file, int targetW, int targetH)
{
    SDL_Surface* tempSurface = IMG_Load(file.c_str());
    if (!tempSurface) { /* error handling */ }

    // Create intermediate RGB565
    SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
        0, tempSurface->w, tempSurface->h, 16, SDL_PIXELFORMAT_RGB565);
    SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 255, 0, 255));
    SDL_BlitSurface(tempSurface, NULL, rgb565, NULL);
    SDL_FreeSurface(tempSurface);

    // If no scaling needed
    if (rgb565->w == targetW && rgb565->h == targetH) {
        SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 255, 0, 255));
        return rgb565;
    }

    // Scale to target
    SDL_Surface* scaled = SDL_CreateRGBSurfaceWithFormat(
        0, targetW, targetH, 16, SDL_PIXELFORMAT_RGB565);
    SDL_FillRect(scaled, NULL, SDL_MapRGB(scaled->format, 255, 0, 255));
    SDL_BlitScaled(rgb565, NULL, scaled, NULL);
    SDL_FreeSurface(rgb565);

    SDL_SetColorKey(scaled, SDL_TRUE, SDL_MapRGB(scaled->format, 255, 0, 255));
    return scaled;
}

SDL_Surface* PokeSurface::renderTextWrappedRGB565(TTF_Font* font, const char* text,
                                                   SDL_Color color, int wrapLength)
{
    // Render with Blended_Wrapped (creates RGBA32)
    SDL_Surface* blended = TTF_RenderUTF8_Blended_Wrapped(font, text, color, wrapLength);
    if (!blended) return nullptr;
    // Create RGB565 surface pre-filled with BLACK (color key)
    SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
        0, blended->w, blended->h, 16, SDL_PIXELFORMAT_RGB565);
    // Use BLACK as background instead of magenta
    SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 0, 0, 0));
    
    SDL_BlitSurface(blended, NULL, rgb565, NULL);
    SDL_FreeSurface(blended);
    // Set BLACK as color key
    SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 0, 0, 0));
    return rgb565;
}

SDL_Surface *PokeSurface::onLoadBMP(std::string &file)
{
    SDL_Surface *tempSurface      = NULL;
    SDL_Surface *optimizedSurface = NULL;

    tempSurface = SDL_LoadBMP(file.c_str());
    if (tempSurface == NULL)
    {
        std::cout << "Unable to load image! File: " << file << ".  SDL Error: " << IMG_GetError()
                  << std::endl;
        exit(EXIT_FAILURE);
    }

   // Create RGB565 surface pre-filled with magenta (color key)
   SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
       0, tempSurface->w, tempSurface->h, 16, SDL_PIXELFORMAT_RGB565);
   
   // Fill with magenta
   SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   // Blit the original onto it (alpha pixels won't overwrite magenta)
   SDL_BlitSurface(tempSurface, NULL, rgb565, NULL);
   SDL_FreeSurface(tempSurface);
   
   // Set magenta as color key
   SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   return rgb565;


    // if (optimizedSurface == NULL)
    // {
    //     std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // return optimizedSurface;
}

SDL_Surface *PokeSurface::onLoadBMP(const std::string &file)
{
    SDL_Surface *tempSurface      = NULL;
    SDL_Surface *optimizedSurface = NULL;

    tempSurface = SDL_LoadBMP(file.c_str());
    if (tempSurface == NULL)
    {
        std::cout << "Unable to load image! File: " << file << ".  SDL Error: " << IMG_GetError()
                  << std::endl;
        exit(EXIT_FAILURE);
    }

   // Create RGB565 surface pre-filled with magenta (color key)
   SDL_Surface* rgb565 = SDL_CreateRGBSurfaceWithFormat(
       0, tempSurface->w, tempSurface->h, 16, SDL_PIXELFORMAT_RGB565);
   
   // Fill with magenta
   SDL_FillRect(rgb565, NULL, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   // Blit the original onto it (alpha pixels won't overwrite magenta)
   SDL_BlitSurface(tempSurface, NULL, rgb565, NULL);
   SDL_FreeSurface(tempSurface);
   
   // Set magenta as color key
   SDL_SetColorKey(rgb565, SDL_TRUE, SDL_MapRGB(rgb565->format, 255, 0, 255));
   
   return rgb565;


    // if (optimizedSurface == NULL)
    // {
    //     std::cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // return optimizedSurface;
}

bool PokeSurface::onDraw(SDL_Surface *destinationSurface, SDL_Surface *sourceSurface,
                         SDL_Rect *rect)
{
    if (destinationSurface == NULL || sourceSurface == NULL)
    {
        std::cout << "Missing Source or Desitination Surface! SDL Error: " << SDL_GetError()
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_BlitSurface(sourceSurface, NULL, destinationSurface, rect);

    return true;
}

bool PokeSurface::onDrawScaled(SDL_Surface *destinationSurface, SDL_Surface *sourceSurface,
                               SDL_Rect *rect)
{
    if (destinationSurface == NULL || sourceSurface == NULL)
    {
        return false;
    }
    SDL_BlitScaled(sourceSurface, NULL, destinationSurface, rect);

    return true;
}