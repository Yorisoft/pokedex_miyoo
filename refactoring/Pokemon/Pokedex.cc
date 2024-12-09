// Window Settings
#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define MAX_VISIBLE_ITEMS 5                 // Set the maximum number of visible

#include<iostream>
#include<vector>
#include<string>
#include"sqlite/sqlite3.h"
#include"Pokedex.h"
#include"Pokemon.h"
#include"PokedexDB.h"
#include"SQLstatements.h"

Pokedex::Pokedex() {
    window = NULL;
    screen = NULL;
    texture = NULL;
    renderer = NULL;
    font = NULL;


    running = true;
}

int Pokedex::onExecute() {
    if (onInit() == false) {
        return -1;
    }

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            onEvent(&event);
        }

        onLoop();
        onRender();
    }

    onCleanup(); 

    return 0;
}

bool Pokedex::onInit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
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

    this->screen = SDL_CreateRGBSurface(
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        DEPTH,
        0, 0, 0, 0
    );

    if (TTF_Init() == -1) {
        std::cout << "SDL could not initialize TTF_Init: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    this->font = TTF_OpenFont("res/font/Pokemon_GB.ttf", 20);
    if (this->font == NULL) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(
        this->screen,
        &(this->screen)->clip_rect,
        SDL_MapRGB((this->screen)->format, 0x00, 0x00, 0x00)
    );

    /*if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }*/

	return true;
}

void Pokedex::onEvent(SDL_Event* event) {
    if (event->type == SDL_QUIT) {
        running = false;
    }
}

void Pokedex::onLoop() {
}

void Pokedex::onRender() {
}

void Pokedex::onCleanup() {
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    //Mix_Quit();
}

int main(int argc, char* argv[]) {
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
	//std::string pokemonName= "charmander";
	//Pokemon* pokemon = new Pokemon(&pokemonName);

	//// for testing.
	//std::cout << "This is pokemon object 'name' member variable: " << pokemon->getName() << std::endl;
	//std::cin.get();

	//return 0;
}