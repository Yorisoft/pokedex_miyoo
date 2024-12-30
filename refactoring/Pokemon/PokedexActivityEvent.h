#ifndef _POKEDEXACTIVITYEVENT_H_
#define _POKEDEXACTIVITYEVENT_H_

// Window Settings
#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define MAX_VISIBLE_ITEMS 5                 // Set the maximum number of visible
// Controller inputs
#define SW_BTN_UP SDLK_UP
#define SW_BTN_DOWN SDLK_DOWN
#define SW_BTN_LEFT SDLK_LEFT
#define SW_BTN_RIGHT SDLK_RIGHT

#define SW_BTN_A SDLK_SPACE
#define SW_BTN_B SDLK_LCTRL
#define SW_BTN_X SDLK_LSHIFT
#define SW_BTN_Y SDLK_LALT

#define SW_BTN_L1 SDLK_e
#define SW_BTN_R1 SDLK_t
#define SW_BTN_L2 SDLK_TAB
#define SW_BTN_R2 SDLK_BACKSPACE

#define SW_BTN_SELECT SDLK_RCTRL
#define SW_BTN_START SDLK_RETURN
#define SW_BTN_MENU SDLK_ESCAPE
#define SW_BTN_POWER SDLK_FIRST

#include <SDL.h>
#include <SDL_ttf.h>

class PokedexActivityEvent {
public:
    PokedexActivityEvent();

    virtual ~PokedexActivityEvent();

    virtual void onEvent(SDL_Event* Event);

    virtual void onButtonUp(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonDown(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonLeft(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonRight(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonA(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonB(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonX(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonY(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonL(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonLT(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonR(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonRT(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonSelect(SDL_Keycode sym, Uint16 mod);

    virtual void onButtonStart(SDL_Keycode sym, Uint16 mod);

    virtual void onMinimize();

    virtual void onRestore();

    virtual void onExit();

    virtual void onUser(Uint8 type, int code, void* data1, void* data2);
};

#endif
