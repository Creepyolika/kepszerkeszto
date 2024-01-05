#define CONTROLL_C

#include <SDL.h>
#include <stdbool.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/structs.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/generic_fuggvenyek.h"
#include "../../../inc/RAM_handler.h"

#include "../inc/seged_fuggvenyek.h"
#include "../inc/render.h"

#include "../inc/controll.h"

static SDL_Surface* CONTROLL_SURFACE = NULL;
static SDL_Surface* SliderChar_surface = NULL;

static SDL_Texture* TEXTURE = NULL;

static size_16 ControllSize;
static uint16_t SliderBoxHeight;

static SDL_Rect CharDST;
static SDL_Rect LineDST;
static SDL_Rect ValDST;

static void CreateEmptySliderBox(uint16_t slider) {

    SDL_Rect dst = {.x = 0, .y = SliderBoxHeight * slider,
                    .w = ControllSize.w, .h = SliderBoxHeight};

    Check_SDL(SDL_FillRect(CONTROLL_SURFACE, &dst, 
        SDL_MapRGB(CONTROLL_SURFACE->format, 
        CONTROLL_BGND[0], CONTROLL_BGND[1], CONTROLL_BGND[2])),
        FUNC_RES, "CreeateEmptySliderBox()/SDL_FillRect", true);

    LineDST.y = ((SliderBoxHeight - LineDST.h) - LineDST.h * 10) + dst.y;

    Check_SDL(SDL_FillRect(CONTROLL_SURFACE, &LineDST,
    SDL_MapRGB(CONTROLL_SURFACE->format,
    SLIDER_COLOR[0], SLIDER_COLOR[1], SLIDER_COLOR[2])),
    FUNC_RES, "CreeateEmptySliderBox()/SDL_FillRect", true);

    SDL_Rect TextDST = {.x = LineDST.x, .y = SliderBoxHeight*0.03 + SliderBoxHeight*slider};
    SDL_Surface* NameSurface  = CreateSurface_TextLine((char*) Slider_nevek[slider]);

    TextDST.h = SliderBoxHeight*0.2;
    TextDST.w = ((double) NameSurface->w / NameSurface->h) * TextDST.h;

    SDL_BlitScaled(NameSurface, NULL, CONTROLL_SURFACE, &TextDST);

    SDL_FreeSurface(NameSurface);
}

void Update_SliderBox(uint16_t slider) {

    CreateEmptySliderBox(slider);

    CharDST.x = CONTROLL.SliderPos[slider].x;
    CharDST.y = CONTROLL.SliderPos[slider].y;

    SDL_BlitSurface(SliderChar_surface, NULL, CONTROLL_SURFACE, &CharDST);

    SDL_Surface* ValueSurface = CreateSurface_TextLine(StringAssembler("%d", CONTROLL.SliderValue[slider]));
    release_remembered_addr();
    Check_SDL((intptr_t) ValueSurface, SDL_PTR, "Update_SliderBox()/ValueSurface", true);

    ValDST.h = SliderBoxHeight*0.3;
    ValDST.w = ((double) ValueSurface->w / ValueSurface->h) * ValDST.h;

    ValDST.y = CONTROLL.SliderPos[slider].y - ValDST.h - SliderBoxHeight* 0.02;
    ValDST.x = ControllSize.w * 0.9 - ValDST.w;

    SDL_BlitScaled(ValueSurface, NULL, CONTROLL_SURFACE, &ValDST);

    SDL_FreeSurface(ValueSurface);
}

void Update_ControllSurface() {
    double slider_val_arany = (double) CONTROLL.SliderLine_width / MAX_SLIDER_VALUE;

    SliderBoxHeight = CONTROLL.SliderBox_height;
    ControllSize.w = CONTROLL.width;
    ControllSize.h = RENDER.WindowSize.h;

    LineDST .w = ControllSize.w * 0.8;
    LineDST.h = SliderBoxHeight * 0.04;
    LineDST.x = (ControllSize.w - LineDST.w) / 2;
    LineDST.y = ((SliderBoxHeight - LineDST.h) - LineDST.h * 10);

    CONTROLL.SliderLine_width = LineDST.w;

    if (SliderChar_surface != NULL) {
        CharDST.w = SliderChar_surface->w;
        CharDST.h = SliderChar_surface->h;

        int Slider_x = LineDST.x;
        int Slider_y = (LineDST.y + LineDST.h / 2) - SliderChar_surface->h / 2;

        CONTROLL.default_slider_xpos = Slider_x;
        CONTROLL.max_slider_xpos = LineDST.w + Slider_x;

        Check_SDL(SDL_FillRect(CONTROLL_SURFACE, NULL,
            SDL_MapRGB(CONTROLL_SURFACE->format,
            CONTROLL_BGND[0], CONTROLL_BGND[1], CONTROLL_BGND[2])), 
            FUNC_RES, "Update_ControllSurface()/SDL_FillRect", true);

        for (int i=0; i<SLIDER_COUNT; i+=1) {
            CONTROLL.SliderPos[i].x = Slider_x + (CONTROLL.SliderValue[i] * slider_val_arany);
            CONTROLL.SliderPos[i].y = Slider_y;

            Update_SliderBox(i);

            Slider_y += SliderBoxHeight;
        }
    }
}

void INIT_Controll() {
    size_16 size = {.w = CONTROLL.width, .h = RENDER.WindowSize.h};
    if (CONTROLL_SURFACE) {
        CONTROLL_SURFACE = ReSize_Surface(CONTROLL_SURFACE, size);
    } else {
        SliderChar_surface = CreateSurface_TextLine(SLIDER_CHAR);
        size_16 CharSize;
        CharSize.h = CONTROLL.SliderBox_height*0.3;
        CharSize.w = CharSize.h * (9.0 / 17);
        SliderChar_surface = ReSize_Surface(SliderChar_surface, CharSize);
        CONTROLL.SliderCharSize = CharSize;

        CONTROLL_SURFACE = SDL_CreateRGBSurface(0, CONTROLL.width, RENDER.WindowSize.h, 24, 0, 0, 0, 0);
        Check_SDL((intptr_t) CONTROLL_SURFACE, SDL_PTR, "CONTROLL_SURFACE", true);

        CONTROLL.SliderPos = dmalloc(sizeof(render_pos) * SLIDER_COUNT);
        Check_DyArraymalloc(CONTROLL.SliderPos, 1);

        CONTROLL.SliderValue = dmalloc(sizeof(uint8_t) * SLIDER_COUNT);
        Check_DyArraymalloc(CONTROLL.SliderValue, 1);
        memset(CONTROLL.SliderValue, 0, sizeof(uint8_t) * SLIDER_COUNT);
    }

    Update_ControllSurface();
}

void Update_ControllTexture() {

    if(TEXTURE) {
        SDL_DestroyTexture(TEXTURE);
    }

    TEXTURE = SDL_CreateTextureFromSurface(RENDER.renderer, CONTROLL_SURFACE);
    Check_SDL((intptr_t) TEXTURE, SDL_PTR, "controll.c/TEXTURE", true);

}

void Draw_Controll() {
    SDL_Rect dest = {.w = CONTROLL.width, .h = RENDER.WindowSize.h, .x = RENDER.WindowSize.w - CONTROLL.width, .y = 0};
    SDL_RenderCopy(RENDER.renderer, TEXTURE, NULL, &dest);
}

void controll_Quit() {
    SDL_DestroyTexture(TEXTURE);
    SDL_FreeSurface(SliderChar_surface);
    SDL_FreeSurface(CONTROLL_SURFACE);
    dfree(CONTROLL.SliderValue);
    dfree(CONTROLL.SliderPos);
}