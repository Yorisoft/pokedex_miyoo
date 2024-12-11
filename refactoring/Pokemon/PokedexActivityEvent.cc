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
    switch (event->key.keysym.sym) {
        case SW_BTN_A:
        case SW_BTN_B:
        case SW_BTN_RIGHT:
        case SW_BTN_LEFT:
        case SW_BTN_UP:
            onKeyUp(event->key.keysym.sym, event->key.keysym.mod);
        case SW_BTN_DOWN:
            onKeyDown(event->key.keysym.sym, event->key.keysym.mod);
        case SW_BTN_R1:
        case SW_BTN_L1:
        case SW_BTN_R2:
        case SW_BTN_L2:
            onKeyDown(event->key.keysym.sym, event->key.keysym.mod);
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

void PokedexActivityEvent::onKeyDown(SDL_Keycode sym,Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onKeyUp(SDL_Keycode sym,Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonLeft(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonRight(SDL_Keycode sym, Uint16 mod) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonA(int mX, int mY) {
    //Pure virtual, do nothing
}

void PokedexActivityEvent::onButtonB(int mX, int mY) {
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