#include "PokedexActivityEvent.h"

PokedexActivityEvent::PokedexActivityEvent() {
}

PokedexActivityEvent::~PokedexActivityEvent() {
    //Do nothing
}

void PokedexActivityEvent::onEvent(SDL_Event* event) {
    switch (event->key.keysym.sym) {
        case SW_BTN_A:
        case SW_BTN_B:
        case SW_BTN_RIGHT:
        case SW_BTN_LEFT:
        case SW_BTN_UP:
        case SW_BTN_DOWN:
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