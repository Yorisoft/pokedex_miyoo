#include "PokedexActivityEvent.h"

PokedexActivityEvent::PokedexActivityEvent() {
}

PokedexActivityEvent::~PokedexActivityEvent() {
    //Do nothing
}

void PokedexActivityEvent::onEvent(SDL_Event* event) {
    static const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (event->type == SDL_QUIT) {
        onExit();
    }
    else if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SW_BTN_SELECT) {
            onButtonSelect(event->key.keysym.sym, event->key.keysym.mod);
        }
        else if (event->key.keysym.sym == SW_BTN_START) {
            onButtonStart(event->key.keysym.sym, event->key.keysym.mod);
        }
    }

    if (currentKeyStates[SDL_SCANCODE_UP]) {
        onButtonUp(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        onButtonDown(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        onButtonLeft(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        onButtonRight(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        onButtonA(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_LCTRL]) {
        onButtonB(event->key.keysym.sym, event->key.keysym.mod);
    }
    /*if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        onButtonX(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_LCTRL]) {
        onButtonY(event->key.keysym.sym, event->key.keysym.mod);
    }*/
    if (currentKeyStates[SDL_SCANCODE_E]) {
        onButtonL(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_T]) {
        onButtonR(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_TAB]) {
        onButtonLT(event->key.keysym.sym, event->key.keysym.mod);
    }
    if (currentKeyStates[SDL_SCANCODE_BACKSPACE]) {
        onButtonRT(event->key.keysym.sym, event->key.keysym.mod);
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