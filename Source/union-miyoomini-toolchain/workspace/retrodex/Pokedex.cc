// Window Settings
#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define MAX_VISIBLE_ITEMS 5                 // Set the maximum number of visible

#include<iostream>
#include<iomanip>
#include<sqlite/sqlite3.h>
#include"Pokedex.h"

int main(int argc, char* argv[]) {
    std::cout << "main: START" << std::endl;
 
 /* BASIC GAME LOOP
	Initialize();

	while (true) {
		Events();
		Loop();
		Render();
	}
 	Cleanup(); */

	Pokedex pokedexApp;

	return pokedexApp.onExecute();

    std::cout << "main: END" << std::endl;
}

Pokedex::Pokedex() {
    // Variables for FPS calculation
    frameCount = 0;
    lastTime = SDL_GetTicks();
    fps = 0.0f;

    running = true;

    window = NULL;
    renderer = NULL;
    texture = NULL;
    screen = NULL;
    font = NULL;
    sEffect = NULL;
}

int Pokedex::onExecute() {
    std::cout << "Pokedex::onExecute: START" << std::endl;

    if (onSDLInit() == false || onInit() == false) {
        return -1;
    }

	Uint32 prev_ButtonPressTick;
	static SDL_Event event;
    while (running) {
		while(SDL_PollEvent(&event)){
			prev_ButtonPressTick = SDL_GetTicks();

			onEvent(&event);
		}

		Uint32 cur_ButtonPressTick = SDL_GetTicks();
		Uint32 elapsedTime = cur_ButtonPressTick - prev_ButtonPressTick;
		if(elapsedTime >= 100){
			//SDL_PumpEvents();
			static const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			PokedexActivityManager::onKeyHold(currentKeyStates, &event);

			prev_ButtonPressTick = cur_ButtonPressTick;
		}

        onLoop();
        onRender();
    }
    onCleanup(); 

    std::cout << "Pokedex::onExecute: END" << std::endl;

    return 0;
}

bool Pokedex::onSDLInit() {
    std::cout << "Pokedex::onSDLInit: START" << std::endl;

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

    this->font = TTF_OpenFont("res/assets/font/pokemon-dppt/pokemon-dppt.ttf", 34);
    if (this->font == NULL) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(
        this->screen,
        &(this->screen)->clip_rect,
        SDL_MapRGB((this->screen)->format, 0x00, 0x00, 0x00)
    );

    std::cout << "Pokedex::onSDLInit: END" << std::endl;

    return true;
}

bool Pokedex::onInit() {
    std::cout << "Pokedex::onInit: START" << std::endl;
    PokedexActivityManager::setActiveState(APPSTATE_INTRO);

    std::cout << "Pokedex::onInit: END" << std::endl;
	return true;
}

void Pokedex::onEvent(SDL_Event* event) {
    PokedexActivityEvent::onEvent(event);
    PokedexActivityManager::onEvent(event);
}

void Pokedex::onLoop() {
    PokedexActivityManager::onLoop();

    // Calculate and print FPS
	// Uncomment this line during debug. Not ready to roll out.
    // calculateFPS();
}

void Pokedex::onRender() {
    SDL_RenderClear(renderer);

    PokedexActivityManager::onRender(screen, renderer, texture, font, sEffect);
    SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Calculate and print FPS
	// Uncomment this line during debug. Not ready to roll out.
	// renderFPS();
}

void Pokedex::calculateFPS() {
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - lastTime;

    if (elapsedTime >= 1000) {
        fps = frameCount / (elapsedTime / 1000.0f);
        frameCount = 0;
        lastTime = currentTime;
    }
}

void Pokedex::renderFPS(){
	std::stringstream iss;
	iss << std::fixed << std::setprecision(2);
    iss << fps;

    SDL_Surface* fpsSurface = TTF_RenderUTF8_Blended(
        font,
        iss.str().c_str(),
        { 0, 128, 0 }
    );
    if (fpsSurface == NULL) {
        std::cout << "Unable to render text! SDL Error: fpsSurface " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    };
    SDL_Rect fpsRect;
    fpsRect.x = WINDOW_WIDTH - fpsSurface->w;
    fpsRect.y = 0;
    fpsRect.w = fpsSurface->w;
    fpsRect.h = fpsSurface->h;

    PokeSurface::onDraw(screen, fpsSurface, &fpsRect);
    SDL_FreeSurface(fpsSurface);
}

void Pokedex::onExit() {
    std::cout << "Pokedex::onExit: START" << std::endl;

    running = false;

    std::cout << "Pokedex::onExit: END" << std::endl;
}

void Pokedex::onCleanup() {
    std::cout << "Pokedex::onCleanUp: START" << std::endl;

	TTF_CloseFont(font);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(sEffect);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
	Mix_Quit();
	SDL_VideoQuit();

    SDL_Quit();
	
    std::cout << "Pokedex::onCleanUp: END" << std::endl;
}