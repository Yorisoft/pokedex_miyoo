#include "PokedexActivityEvent.h"
#include <iostream>

PokedexActivityEvent::PokedexActivityEvent() {
}

PokedexActivityEvent::~PokedexActivityEvent() {
}

void PokedexActivityEvent::onEvent(SDL_Event* event) {
	SDL_Keycode key = event->key.keysym.sym;

	switch (event->type) {
	case SDL_QUIT:
	case SDLK_ESCAPE:
		onExit();
	break;
	case SDL_KEYDOWN:
		std::cout << "Keydown" << std::endl;
		if(keystates[key] != RELEASED){
			keystates[key] = REPEATING;
			onKeyHold(key, event->key.keysym.mod);
			std::cout << "called onKeyHold" << std::endl;
		}
		else{
			keystates[key] = PRESSED;
			onKeyPress(key, event->key.keysym.mod);
			currentTime = SDL_GetTicks();
			std::cout << "called onKeyPress" << std::endl;
		}
	break;
	case SDL_KEYUP:
		std::cout << "KeyUp" << std::endl;
		keystates[key] = RELEASED;
		onKeyRelease(key, event->key.keysym.mod);
		currentTime = SDL_GetTicks();
		std::cout << "called onKeyRelease" << std::endl;
	break;
	default:
		onUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
		break;  
	}
}

// this will be called when a key is pressed
// only runs once when the key is pressed
void PokedexActivityEvent::onKeyPress(SDL_Keycode key, Uint16 mod) {
	std::cout << "Key: " << key << std::endl;
	std::cout << "SW_BTN_UP: " << SW_BTN_UP << std::endl;
	
	switch(key){
	case SW_BTN_UP:
		onButtonUp(key, mod);
		std::cout << "Called up" << std::endl;
		break;
	case SW_BTN_DOWN:
		onButtonDown(key, mod);
		std::cout << "Called down" << std::endl;
		break;
	case SW_BTN_A:
		onButtonA(key, mod);
		std::cout << "Called Button A" << std::endl;
		break;
	case SW_BTN_B:
		onButtonB(key, mod);
		break;
	default:
		break;
	}
}

// this will be called when a key is released
// only runs once when the key is released
void PokedexActivityEvent::onKeyRelease(SDL_Keycode key, Uint16 mod) {
    // Exactly when the key is released
}

// this will be called when a key is held down
// runs every frame when the key is held down
// or you can use the timer to check if the key is held down for a certain amount of time
void PokedexActivityEvent::onKeyHold(SDL_Keycode key, Uint16 mod) {
	//currentTime = SDL_GetTicks();
    currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - lastTime; 
	
	std::cout << "Elapsed Time: " << elapsedTime << std::endl;

	if(elapsedTime >= 100){
		// examples
		// A btn run every frame
		if (key == SW_BTN_UP)
			onButtonUp(key, mod);

		// examples
		// A btn run every frame
		if (key == SW_BTN_DOWN)
			onButtonDown(key, mod);

		lastTime = currentTime;
	}

    // examples
    // A btn run every frame
    if (key == SW_BTN_A) {
        // do something
    }

    // B btn run every other frame
    if (key == SW_BTN_B) {
        if (elapsedTime % 2 == 0) {
            // do something
        }
    }

    // X btn run continuously after 500
    if (key == SW_BTN_X) {
        if (elapsedTime > 500) {
            // do something
        }
    }

    // Y btn run continously after 1000 but register every 10 frame
    if (key == SW_BTN_Y) {
        if (elapsedTime > 1000 && elapsedTime % 10 == 0) {
            // do something
        }
    }
}


void PokedexActivityEvent::onButtonUp(SDL_Keycode sym,Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonDown(SDL_Keycode sym,Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonA(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonB(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonX(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonY(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonL(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonR(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonLT(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonRT(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonSelect(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonStart(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onMinimize() {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onRestore() {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onExit() {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onUser(Uint8 type, int code, void* data1, void* data2) {
    //Pure virtual, do nothing
}