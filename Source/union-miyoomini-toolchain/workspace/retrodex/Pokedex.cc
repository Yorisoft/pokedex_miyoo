// Window Settings
#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define MAX_VISIBLE_ITEMS 5                 // Set the maximum number of visible

#include<iostream>
#include<vector>
#include<string>
#include<iomanip>
#include<sqlite/sqlite3.h>
#include"Pokedex.h"

int main(int argc, char* argv[]) {
    std::cout << "main: start" << std::endl;
/* 
    BASIC GAME LOOP
	Initialize();

	while (true) {
		Events();
		Loop();
		Render();
	}

	Cleanup();
*/
	Pokedex pokedexApp;

	return pokedexApp.onExecute();

    std::cout << "main: end" << std::endl;
}

Pokedex::Pokedex() {
    window = NULL;
    screen = NULL;
    screenTest = NULL;
    texture = NULL;
    renderer = NULL;
    font = NULL;
    sEffect = NULL;

    // Variables for FPS calculation
    frameCount = 0;
    lastTime = SDL_GetTicks();
    fps = 0.0f;

    running = true;
}

int Pokedex::onExecute() {
    std::cout << "onExecute: start" << std::endl;
    if (onSDLInit() == false || onInit() == false) {
        return -1;
    }

    SDL_Event event;

    while (running) {
        onEvent(&event);
        onLoop();
        onRender();
    }

    onCleanup(); 

    std::cout << "onExecute: end" << std::endl;
    return 0;
}


bool Pokedex::onSDLInit() {
    std::cout << "onSDLInit: start" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
        exit(EXIT_FAILURE);
    }

    this->window = SDL_CreateWindow(
        "Pokedex",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!this->window) {
        std::cout << "Failed to create window: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!this->renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    this->texture = SDL_CreateTexture(
        this->renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
    if (!this->texture) {
        std::cout << "Failed to create texture: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    
    this->screen = SDL_CreateRGBSurface(
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        DEPTH,
        0, 0, 0, 0
    );
    if (!this->screen) {
        std::cout << "Failed to create optimized surface, screen, from tempSurface: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1) {
        std::cout << "SDL could not initialize TTF_Init: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    this->font = TTF_OpenFont("res/font/pokemon-dppt/pokemon-dppt.ttf", 34);
    if (this->font == NULL) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(
        this->screen,
        &(this->screen)->clip_rect,
        SDL_MapRGB((this->screen)->format, 0x00, 0x00, 0x00)
    );

    std::cout << "onSDLInit: end" << std::endl;

    return true;
}

bool Pokedex::onInit() {
    std::cout << "onInit: start" << std::endl;
    PokedexActivityManager::setActiveState(APPSTATE_INTRO);

    std::cout << "onInit: end" << std::endl;
	return true;
}

void Pokedex::onEvent(SDL_Event* event) {
    //std::cout << "onEvent: start" << std::endl;

    PokedexActivityEvent::onEvent(event);
    PokedexActivityManager::onEvent(event);

    //std::cout << "onEvent: end" << std::endl;
}

void Pokedex::onLoop() {
    //std::cout << "onLoop: start" << std::endl;

    // Calculate and print FPS
	// Uncomment this line during debug. Causes large log files otherwise.
    //calculateFPS(frameCount, lastTime, fps);

    PokedexActivityManager::onLoop();
    //std::cout << "onLoop: end" << std::endl;
}

void Pokedex::onRender() {
    ////std::cout << "onRender: start" << std::endl;
    SDL_RenderClear(renderer);
    PokedexActivityManager::onRender(screen, renderer, texture, font, sEffect);

    SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //std::cout << "onRender: end" << std::endl;
}

void Pokedex::calculateFPS(Uint32& frameCount, Uint32& lastTime, float& fps) {
    // Increment the frame count
    frameCount++;

    // Get the current time in milliseconds
    Uint32 currentTime = SDL_GetTicks();

    // Calculate elapsed time since last FPS calculation
    Uint32 elapsedTime = currentTime - lastTime;

    // Update FPS every 1 second
    if (elapsedTime >= 1000) {
        fps = frameCount / (elapsedTime / 1000.0f);
        frameCount = 0;
        lastTime = currentTime;
    }

    const int width = 100; // Total width allocated for FPS output
    std::cout << std::fixed << std::setprecision(2);

    // Right-align the output by setting fill to spaces and width
    std::cout << "FPS: " << fps << "\r";
}

void Pokedex::onExit() {
    running = false;
}

void Pokedex::onCleanup() {
    std::cout << "onCleanUp: start" << std::endl;

    SDL_FreeSurface(screen);
    SDL_FreeSurface(screenTest);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(sEffect);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    //Mix_Quit();

    std::cout << "onCleanUp: end" << std::endl;
}