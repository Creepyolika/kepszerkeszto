#include <SDL.h>

#include "../../../inc/main.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/structs.h"

#include "../inc/controll.h"
#include "../inc/render.h"

static SDL_Window* window = NULL;

void Window_UpdateSize() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    RENDER.WindowSize.w = w; RENDER.WindowSize.h = h;
}

//  Ablak létrehozása
void Window_INIT(int init_w, int init_h, int min_w, int min_h, Uint32 flags) {

    window = SDL_CreateWindow("Kepszerkeszto",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        init_w, init_h,
        flags);
    Check_SDL((intptr_t) window, SDL_PTR, "SDL_CreateWindow", true);

    SDL_SetWindowMinimumSize(window, min_w, min_h);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Check_SDL((intptr_t) renderer, SDL_PTR, "SDL_Renderer", true);
    RENDER.renderer = renderer;
    
    Window_UpdateSize();
}

void Window_destroy() {
    SDL_DestroyWindow(window);
    window = NULL;
}
