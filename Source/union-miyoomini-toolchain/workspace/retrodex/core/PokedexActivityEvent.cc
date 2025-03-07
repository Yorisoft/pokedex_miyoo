#include "PokedexActivityEvent.h"
#include <iostream>

PokedexActivityEvent::PokedexActivityEvent() {
}

PokedexActivityEvent::~PokedexActivityEvent() {
}

void PokedexActivityEvent::onEvent(SDL_Event* event) {
	if (event->type == SDL_QUIT || event->key.keysym.sym == SW_BTN_MENU || event->type == SDL_SYSWMEVENT ){
		onExit();
	}
	else if (event->type == SDL_KEYDOWN) {
		switch(event->key.keysym.sym){
		// DPAD
		case SW_BTN_UP:
			onButtonUp(SW_BTN_UP, event->key.keysym.mod);
			break;
		case SW_BTN_DOWN:
			onButtonDown(SW_BTN_DOWN, event->key.keysym.mod);
			break;
		case SW_BTN_LEFT:
			onButtonLeft(SW_BTN_LEFT, event->key.keysym.mod);
			break;
		case SW_BTN_RIGHT:
			onButtonRight(SW_BTN_RIGHT, event->key.keysym.mod);
			break;
		// FACE BUTTON
		case SW_BTN_A:
			onButtonA(SW_BTN_A, event->key.keysym.mod);
			break;
		case SW_BTN_B:
			onButtonB(SW_BTN_B, event->key.keysym.mod);
			break;
		case SW_BTN_X:
			onButtonX(SW_BTN_X, event->key.keysym.mod);
			break;
		case SW_BTN_Y:
			onButtonY(SW_BTN_Y, event->key.keysym.mod);
			break;
		// BACK BUTTONS
		case SW_BTN_R1:
			onButtonR(SW_BTN_R1, event->key.keysym.mod);
			break;
		case SW_BTN_L1:
			onButtonL(SW_BTN_L1, event->key.keysym.mod);
			break;
		case SW_BTN_R2:
			onButtonRT(SW_BTN_R2, event->key.keysym.mod);
			break;
		case SW_BTN_L2:
			onButtonLT(SW_BTN_L2, event->key.keysym.mod);
			break;
		// MENU BUTTONS
		case SW_BTN_START:
			onButtonStart(SW_BTN_START, event->key.keysym.mod);
			break;
		case SW_BTN_SELECT:
			onButtonSelect(SW_BTN_SELECT, event->key.keysym.mod);
			break;
		default:
			break;
		}
	}
}

void PokedexActivityEvent::onKeyHold(const Uint8* currentKeyStates, SDL_Event* event) {
	// D-PAD
	if (currentKeyStates[SDL_SCANCODE_UP]) {
		onButtonUp(SW_BTN_UP, event->key.keysym.mod);
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN]) {
		onButtonDown(SW_BTN_DOWN, event->key.keysym.mod);
	}
	/* if (currentKeyStates[SDL_SCANCODE_LEFT]) { */
	/* 	onButtonLeft(SW_BTN_LEFT, event->key.keysym.mod); */
	/* } */
	/* if (currentKeyStates[SDL_SCANCODE_RIGHT]) { */
	/* 	onButtonRight(SW_BTN_RIGHT, event->key.keysym.mod); */
	/* } */
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