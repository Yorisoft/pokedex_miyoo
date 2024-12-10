#ifndef _CEVENT_H_
#define _CEVENT_H_
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
#include <SDL_image.h>                   	

class PokedexActivityEvent {
public:
    PokedexActivityEvent();

    virtual ~PokedexActivityEvent();

    virtual void onEvent(SDL_Event* Event);

    virtual void onKeyDown(SDL_Keycode sym, Uint16 mod);

    virtual void onMinimize();

    virtual void onRestore();

    virtual void onExit();

    virtual void onUser(Uint8 type, int code, void* data1, void* data2);
};

#endif
