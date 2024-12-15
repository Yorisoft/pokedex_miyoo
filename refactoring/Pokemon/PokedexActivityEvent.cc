#include "PokedexActivityEvent.h"

PokedexActivityEvent::PokedexActivityEvent() {
}

PokedexActivityEvent::~PokedexActivityEvent() {
    //Do nothing
}

void PokedexActivityEvent::onEvent(SDL_Event* event) {
    if (event->type == SDL_QUIT) {
        onExit();
    }
    else if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SW_BTN_A:
            onButtonA(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SW_BTN_B:
            onButtonB(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SW_BTN_RIGHT:
            break;
        case SW_BTN_LEFT:
            break;
        case SW_BTN_UP:
            onButtonUp(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SW_BTN_DOWN:
            onButtonDown(event->key.keysym.sym, event->key.keysym.mod);
            break;
        case SW_BTN_R1:
            break;
        case SW_BTN_L1:
            break;
        case SW_BTN_R2:
            break;
        case SW_BTN_L2:
            break;
        case SDL_SYSWMEVENT:
            break;
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

void PokedexActivityEvent::onButtonX(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonY(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonL(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonLT(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonR(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonRT(int mX, int mY) {
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