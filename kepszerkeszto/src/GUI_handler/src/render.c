#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/generic_fuggvenyek.h"

#include "../inc/seged_fuggvenyek.h"
#include "../inc/controll.h"
#include "../inc/viewport.h"
#include "../inc/input_window.h"

#include "../inc/render.h"

GUI_RENDER RENDER;
GUI_Controll CONTROLL;
GUI_ViewPort ViewPort;

void RENDER_INIT() {
    SDL_Surface* ascii = SDL_LoadBMP(StringAssembler("%sascii_extended.bmp", ASSET_PATH));
    release_remembered_addr();
    Check_SDL((intptr_t) ascii, SDL_PTR, "ASCII surface", true);

    SetAscii(ascii);
}
 
void RENDER_EditorraValt() {
    inputWindow_Quit();

    RENDER.ColorScale = 255.0 / IMAGE.maxval;

    INIT_Controll();
}

void RENDER_Quit() {
    if(kep_beolvasva) {
        ViewPort_Quit();
        controll_Quit();
        return;
    }
    FreeAscii();
    inputWindow_Quit();
}