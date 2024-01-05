#define INPUT_C

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/structs.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/input_handler.h"
#include "../../../inc/generic_fuggvenyek.h"

#include "../inc/seged_fuggvenyek.h"
#include "../inc/render.h"

static SDL_Texture* TEXTURES[4] = {NULL, NULL, NULL, NULL};
static char* error_str = NULL;

void Update_inputWindow() {
        
    double scale = RENDER.WindowSize.h / 480.0;

    if (error_str != NULL) {
        TEXTURES[3] = CreateTexture_TextLine(RENDER.renderer, error_str);
        Check_SDL((intptr_t) TEXTURES[3], SDL_PTR, "input_window.c/TEXTURES[3]", true);
    }

    SDL_DestroyTexture(TEXTURES[2]);
    TEXTURES[2] = CreateTexture_TextLine(RENDER.renderer, input);
    Check_SDL((intptr_t) TEXTURES[2], SDL_PTR, "input_window.c/TEXTURES[2]", true);


    int w, h;
    SDL_Rect dest;

    for (int i=0; i<4; i+=1) {
        if (TEXTURES[i] != NULL) {
            SDL_QueryTexture(TEXTURES[i], NULL, NULL, &w, &h);
            dest.w = w*scales[i]*scale;
            dest.h = h*scales[i]*scale;

            dest.x = (RENDER.WindowSize.w-dest.w)/2;
            dest.y = ypos[i]*scale;

            SDL_RenderCopyEx(RENDER.renderer, TEXTURES[i], NULL, &dest, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

void INIT_inputWindow() {

    TEXTURES[0] = CreateTexture_TextLine(RENDER.renderer, "Kepszerkeszto");
    TEXTURES[1] = CreateTexture_TextLine(RENDER.renderer, "Add meg a file nevet vagy helyet");
    TEXTURES[2] = CreateTexture_TextLine(RENDER.renderer, "");

    for (int i=0; i<3; i+=1) {
        Check_SDL((intptr_t) TEXTURES[i], SDL_PTR, StringAssembler("input_window.c/TEXTURES[%d]", i), true);
    }
    release_remembered_addr();

    Update_inputWindow();
}

void SetInputGUIError(char* str) {
    error_str = str;
}

void inputWindow_Quit() {
    for (int i=0; i<4; i+=1) {
        if(TEXTURES[i]) {
            SDL_DestroyTexture(TEXTURES[i]);
        }
    }
}