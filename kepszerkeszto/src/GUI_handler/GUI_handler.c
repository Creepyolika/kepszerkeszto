#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../../inc/main.h"
#include "../../inc/ERROR_handler.h"
#include "../../inc/RAM_handler.h"
#include "../../inc/settings.h"
#include "../../inc/structs.h"
#include "../../inc/img_processing.h"

#include "inc/window_handler.h"
#include "inc/render.h"
#include "inc/input_window.h"
#include "inc/controll.h"
#include "inc/viewport.h"

// minimum editor ablakméret
static size_16 min_AblakMeret_editor = {.w = 400, .h = 400}; // default minimum ablakmélret
// monitor méretek
static size_16 MONITOR_SIZE = {.w = 400, .h = 400}; // default monitor méret

void GUI_INIT() {

    //  SDL indítása
    Check_SDL(SDL_Init(SDL_INIT_EVERYTHING), FUNC_RES, "SDL_Init", true);

    //  Monitor méretek lekérése
    int kijelzo_szam = SDL_GetNumVideoDisplays();
    if (kijelzo_szam > 0) {
        SDL_Window* temp_window = SDL_CreateWindow("IdeiglenesAblak\n", 0, 0, 1, 1, SDL_WINDOW_HIDDEN);
        if (Check_SDL((intptr_t) temp_window, SDL_PTR, "TempWindow", false)) {
            SDL_DisplayMode MonitorInfo;
            int Mindex = SDL_GetWindowDisplayIndex(temp_window);
            SDL_DestroyWindow(temp_window);
            
            //  Ablak méreteinek beállíása
            if (Check_SDL(SDL_GetDisplayMode(Mindex, 0, &MonitorInfo), FUNC_RES, "SDL_GetDisplayMode", false)) {
                MONITOR_SIZE.w = MonitorInfo.w*MONITOR_SCALE_CORRECTION;
                MONITOR_SIZE.h = MonitorInfo.h*MONITOR_SCALE_CORRECTION;
            }
        } 
    }
    RENDER_INIT();
}

void GUI_OpenInputWindow() {

    size_16 AblakMeret = {.w = MONITOR_SIZE.w * 0.5,
                            .h = (MONITOR_SIZE.w * 0.5) * 0.5};

    Window_INIT(AblakMeret.w, AblakMeret.h, 100, 50, 0);
    SDL_SetRenderDrawColor(RENDER.renderer, 0, 0, 0, 255);
    INIT_inputWindow();
}

int Update_GUISize() {
    Window_UpdateSize();
    ViewPort.size.w = RENDER.WindowSize.w-CONTROLL.width;
    ViewPort.size.h = RENDER.WindowSize.h;

    INIT_Controll();
    
    ViewPort.ratio = (double) ViewPort.size.w/ViewPort.size.h;

    if (RENDER.kep_arany > ViewPort.ratio) {
        ViewPort.BaseScaleImgSize.w = ViewPort.size.w;
        ViewPort.BaseScaleImgSize.h = round(ViewPort.BaseScaleImgSize.w / RENDER.kep_arany);

        ViewPort.CenteredImgPos.x = 0;
        ViewPort.CenteredImgPos.y = (ViewPort.size.h - ViewPort.BaseScaleImgSize.h)/2;

    } else {
        ViewPort.BaseScaleImgSize.h = ViewPort.size.h;
        ViewPort.BaseScaleImgSize.w = round(ViewPort.BaseScaleImgSize.h * RENDER.kep_arany);

        ViewPort.CenteredImgPos.x = (ViewPort.size.w-ViewPort.BaseScaleImgSize.w)/2;
        ViewPort.CenteredImgPos.y = 0;

    }
    
    ViewPort.MaxZoomScale = MAXZOOM * IMAGE.size.w / (double) ViewPort.BaseScaleImgSize.w;
    return ViewPort.size.w;
}

void GUI_OpenEditorWindow() {

    size_16 AblakMeret;

    size_16 KezeloFelulet = {.w = round(MONITOR_SIZE.w/1920.0) * CONTROLL_WIDTH,
                                .h = (round((MONITOR_SIZE.w/16.0)*9.0)/1080.0) * CONTROLL_HEIGHT};

    CONTROLL.width = (uint16_t) KezeloFelulet.w;

    double kep_arany = (double) IMAGE.size.w / IMAGE.size.h;
    RENDER.kep_arany = kep_arany;

    if (kep_arany < 1.0) {
        AblakMeret.h = round(MONITOR_SIZE.h*0.5);
        AblakMeret.w = round(AblakMeret.h*kep_arany) + KezeloFelulet.w;
    } else {
        AblakMeret.w = round(MONITOR_SIZE.w*0.5) + KezeloFelulet.w;
        AblakMeret.h = round(MONITOR_SIZE.w*0.5 / kep_arany);
    }

    min_AblakMeret_editor.w = KezeloFelulet.w + round(ViewPort.size.w*0.3);
    min_AblakMeret_editor.h = round(KezeloFelulet.w*((double) CONTROLL_HEIGHT / CONTROLL_WIDTH));

    CONTROLL.SliderBox_height = min_AblakMeret_editor.h / SLIDER_COUNT;


    //  ablak lérehozása
    Window_INIT(AblakMeret.w, AblakMeret.h, min_AblakMeret_editor.w, min_AblakMeret_editor.h, SDL_WINDOW_RESIZABLE);
    Check_SDL(SDL_SetRenderDrawColor(RENDER.renderer, 0, 0, 0, 255), FUNC_RES, "SDL_SetRenderDrawColor", false);

    ViewPort.ZoomScale = 1;
    ViewPort.offset.x = 0;
    ViewPort.offset.y = 0;

    Update_GUISize();
}

void Update_GUI(bool csak_ViewPort, bool update_processed_image) {
    SDL_RenderClear(RENDER.renderer);
    if(update_processed_image) {
        Update_ImageTexture();
    }
    if (kep_beolvasva) {
        Update_ViewPortRENDER();

        if (!csak_ViewPort) {
            Update_ControllTexture();
        }

        Draw_Controll();
    } else {
        Update_inputWindow();
    }
    SDL_RenderPresent(RENDER.renderer);
}

void Update_ViewPortZoom(Sint32 scroll) {

    double scale_inc = scroll * ZOOM;
    size_16 kepmeret = {
        .w = ViewPort.BaseScaleImgSize.w * ViewPort.ZoomScale,
        .h = ViewPort.BaseScaleImgSize.h * ViewPort.ZoomScale
    };
    int lemarado[2] = {0, 0};
    if (kepmeret.w > ViewPort.size.w+2) {
        lemarado[0] = kepmeret.w - ViewPort.size.w;
    }
    if (kepmeret.h > ViewPort.size.h+2) {
        lemarado[1] = kepmeret.h - ViewPort.size.h;
    }
    int VWfel[2] = {
        round(ViewPort.size.w / 2),
        round(ViewPort.size.h /2)
    };
    int b_oldal[2] = {
        (-1*(ViewPort.offset.x+ViewPort.CenteredImgPos.x)) + VWfel[0],
        (-1*(ViewPort.offset.y+ViewPort.CenteredImgPos.y)) + VWfel[1]
    };
    int j_oldal[2] = {
        abs((int32_t) kepmeret.w - b_oldal[0]),
        abs((int32_t) kepmeret.h - b_oldal[1])

    };
    // b_oldal / j_oldal
    double arany[2] = {1.0, 1.0};
    for (int i=0; i<2; i+=1) {
        if (lemarado[i] > 2) {
            arany[i] = (double) b_oldal[i] / j_oldal[i];
        }
    }

    ViewPort.ZoomScale += ViewPort.ZoomScale * scale_inc;
    if (ViewPort.ZoomScale > ViewPort.MaxZoomScale) {
        ViewPort.ZoomScale = ViewPort.MaxZoomScale;
    }
    if(ViewPort.ZoomScale < 1) {
        ViewPort.ZoomScale = 1;
        ViewPort.offset.x = 0;
        ViewPort.offset.y = 0;
    }

    ViewPort.CenteredImgPos.x = round((ViewPort.size.w - ViewPort.BaseScaleImgSize.w * ViewPort.ZoomScale) / 2.0);
    ViewPort.CenteredImgPos.y = round((ViewPort.size.h - ViewPort.BaseScaleImgSize.h * ViewPort.ZoomScale) / 2.0);

    kepmeret.w = ViewPort.BaseScaleImgSize.w * ViewPort.ZoomScale;
    kepmeret.h = ViewPort.BaseScaleImgSize.h * ViewPort.ZoomScale;

    j_oldal[0] = round((double) kepmeret.w / (1 + arany[0]));
    j_oldal[1] = round((double) kepmeret.h / (1 + arany[1]));

    lemarado[0] = 0; lemarado[1] = 0;
    if (kepmeret.w > ViewPort.size.w+2) {
        lemarado[0] = kepmeret.w - ViewPort.size.w;
    }
    if (kepmeret.h > ViewPort.size.h+2) {
        lemarado[1] = kepmeret.h - ViewPort.size.h;
    }

    b_oldal[0] = kepmeret.w - j_oldal[0];
    b_oldal[1] = kepmeret.h - j_oldal[1];

    ViewPort.offset.x = -1*(b_oldal[0] - VWfel[0]) - ViewPort.CenteredImgPos.x;
    ViewPort.offset.y = -1*(b_oldal[1] - VWfel[1]) - ViewPort.CenteredImgPos.y;

}

int GUI_EditorraValt() {
    kep_beolvasva = true;
    Window_destroy();
    SDL_DestroyRenderer(RENDER.renderer);
    RENDER.renderer = NULL;

    GUI_OpenEditorWindow();
    RENDER_EditorraValt();
    Update_GUI(false, true);
    return ViewPort.size.w;
}

void SetViewPortOffset(int xrel, int yrel) {
    ViewPort.offset.x += xrel;
    ViewPort.offset.y += yrel;
}

int CheckForSliderContact(render_pos mouse_pos) {
    mouse_pos.x -= ViewPort.size.w;

    int slider = mouse_pos.y / CONTROLL.SliderBox_height;

    if (slider < SLIDER_COUNT) {
        if ((uint16_t) (mouse_pos.x - CONTROLL.SliderPos[slider].x) < CONTROLL.SliderCharSize.w && (uint16_t) (mouse_pos.y - CONTROLL.SliderPos[slider].y) < CONTROLL.SliderCharSize.h) {
            return slider;
        }
    }

    return -1;
}

void Handle_SliderChange(int mouse_xpos, int slider, int slider_cursor_diff) {

    mouse_xpos -= ViewPort.size.w;
    CONTROLL.SliderPos[slider].x = mouse_xpos + slider_cursor_diff;

    if (CONTROLL.SliderPos[slider].x >= CONTROLL.default_slider_xpos) {
        if (CONTROLL.SliderPos[slider].x > CONTROLL.max_slider_xpos) {
            CONTROLL.SliderPos[slider].x = CONTROLL.max_slider_xpos;
        }
    } else {
        CONTROLL.SliderPos[slider].x = CONTROLL.default_slider_xpos;
    }

    CONTROLL.SliderValue[slider] = (CONTROLL.SliderPos[slider].x - CONTROLL.default_slider_xpos) / ((double) CONTROLL.SliderLine_width / MAX_SLIDER_VALUE);
    Update_SliderBox(slider);
    handle_ValueChange(slider, CONTROLL.SliderValue[slider]);
}

int SliderCursorDiff(int mouse_xpos, int slider) {
    mouse_xpos -= ViewPort.size.w;
    return CONTROLL.SliderPos[slider].x - mouse_xpos;
}

void reset_slider_values() {
    for (int s=0; s<SLIDER_COUNT; s+=1) {
        CONTROLL.SliderValue[s] = 0;
    }
    Update_ControllSurface();
}

void GUI_Quit() {
    RENDER_Quit();
    SDL_Quit();
}
