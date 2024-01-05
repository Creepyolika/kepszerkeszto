#include <SDL.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/structs.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/img_processing.h"

#include "../inc/render.h"

static SDL_Texture* TEXTURE = NULL;

void Update_ImageTexture() {

    SDL_Surface* surface = SDL_CreateRGBSurface(0, IMAGE.size.w, IMAGE.size.h, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);
    Check_SDL((intptr_t) surface, SDL_PTR, "Update_ImageTexture()/surface", true);
    
    void** pixel_data = process();
    uint16_t color = 0;
    for (int x = 0; x < IMAGE.size.w; x++) {
        for (int y = 0; y < IMAGE.size.h; y++) {

            int pixelPos = y * surface->pitch + x * 3;
            int src_pos = y*IMAGE.pitch + x*IMAGE.color_depth;

            for (int c=0; c<3; c+=1) {
                color = 0;
                for (int s=0; s<IMAGE.color_depth; s+=1) {
                    color = color << s*8;
                    color += ((uint8_t*) pixel_data[c])[src_pos+s];
                }
                ((uint8_t*)surface->pixels)[pixelPos + c] = color*RENDER.ColorScale;
            }
        }
    }

    TEXTURE = SDL_CreateTextureFromSurface(RENDER.renderer, surface);
    Check_SDL((intptr_t) TEXTURE, SDL_PTR, "TEXTURES[0]", true);
    SDL_FreeSurface(surface);
}

void Update_ViewPortRENDER() {

    double scale = ViewPort.ZoomScale;

    SDL_Rect dest;
    SDL_Rect src = {.x = 0, .y = 0, .w = IMAGE.size.w, .h = IMAGE.size.h};

    dest.w = ViewPort.BaseScaleImgSize.w * scale;
    dest.h = ViewPort.BaseScaleImgSize.h * scale;

    int xkozepre = ViewPort.CenteredImgPos.x;
    int ykozepre = ViewPort.CenteredImgPos.y;
    
    if (dest.w > ViewPort.size.w) {
        dest.x = xkozepre + ViewPort.offset.x;
        if (dest.x < ViewPort.size.w - dest.w) {
            dest.x = ViewPort.size.w - dest.w;

        } else if (dest.x > 0) {
            dest.x = 0;
        }
        ViewPort.offset.x = dest.x - xkozepre;
    } else {
        dest.x = xkozepre;
    }

    if (dest.h > ViewPort.size.h) {
        dest.y = ykozepre + ViewPort.offset.y;
        if (dest.y < ViewPort.size.h - dest.h) {
            dest.y = ViewPort.size.h - dest.h;
        } else if (dest.y > 0) {
            dest.y = 0;
        }
        ViewPort.offset.y = dest.y - ykozepre;
    } else {
        dest.y = ykozepre;
    }

    double pixel_scale = (double) IMAGE.size.w / dest.w;

    if (dest.w > ViewPort.size.w) {
        src.x = -1*dest.x * pixel_scale;
        src.w = ((double) ViewPort.size.w / dest.w) * IMAGE.size.w;

        dest.x = 0;
        dest.w = ViewPort.size.w;
    }

    if (dest.h > ViewPort.size.h) {
        src.y = -1*dest.y * pixel_scale;
        src.h = ((double) ViewPort.size.h / dest.h) * IMAGE.size.h;

        dest.y = 0;
        dest.h = ViewPort.size.h;
    }

    Check_SDL(SDL_RenderCopy(RENDER.renderer, TEXTURE, &src, &dest), FUNC_RES, "Update_ViewPort()/SDL_RenderCopy", true);
}

void ViewPort_Quit() {
    SDL_DestroyTexture(TEXTURE);
}