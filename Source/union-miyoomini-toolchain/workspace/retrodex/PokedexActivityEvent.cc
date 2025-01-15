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
        switch (event->key.keysym.sym) {
        case SW_BTN_UP:
            if (currentKeyStates[SDL_SCANCODE_UP]) {
                onButtonUp(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_DOWN:
            if (currentKeyStates[SDL_SCANCODE_DOWN]) {
                onButtonDown(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_LEFT:
            if (currentKeyStates[SDL_SCANCODE_LEFT]) {
                onButtonLeft(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_RIGHT:
            if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
                onButtonRight(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_A:
            if (currentKeyStates[SDL_SCANCODE_SPACE]) {
                onButtonA(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_B:
            if (currentKeyStates[SDL_SCANCODE_LCTRL]) {
                onButtonB(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_X:
            if (currentKeyStates[SDL_SCANCODE_SPACE]) {
                onButtonX(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_Y:
            if (currentKeyStates[SDL_SCANCODE_LCTRL]) {
                onButtonY(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_L1:
            if (currentKeyStates[SDL_SCANCODE_E]) {
                onButtonL(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_R1:
            if (currentKeyStates[SDL_SCANCODE_T]) {
                onButtonR(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_L2:
            if (currentKeyStates[SDL_SCANCODE_TAB]) {
                onButtonLT(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_R2:
            if (currentKeyStates[SDL_SCANCODE_BACKSPACE]) {
                onButtonRT(event->key.keysym.sym, event->key.keysym.mod);
            }
            break;
        case SW_BTN_SELECT:
            onButtonSelect(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SW_BTN_START:
            onButtonStart(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SDL_SYSWMEVENT:
        case SDLK_ESCAPE:
        case SDL_QUIT:
            onExit();
            break;
        default:
            onUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
            break;  
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