#ifndef RENDER_H
#define RENDER_H

#include "../../../inc/structs.h"

typedef struct GUI_RENDER {
    
    // Ablak méretei
    size_16 WindowSize;

    // Kép w/h
    double kep_arany;

    // SDL Renderer az ablakhoz
    SDL_Renderer* renderer;
    
    // IMAGE MaxVal / 255
    double ColorScale;
} GUI_RENDER;

typedef struct GUI_Controll {
    
    // ViewPort melletti kezelőfelület szélessége
    uint16_t width;

    // Slider körül levő terület mérete
    uint16_t SliderBox_height;

    // slider boxok jobb felső sarkai a ControllSurface-hez relatívan
    // 0-5: föntről le a boxok
    render_pos* SliderPos;

    // slider box méretei
    size_16 SliderCharSize;

    uint16_t SliderLine_width;

    // Slider alap x pozíciója relatívan a SliderBox-hoz
    uint16_t default_slider_xpos;

    // Slider maximum x pozíciója relatívan a SliderBox-hoz
    uint16_t max_slider_xpos;

    // Tömb, ami tárolja a sliderek értékét 0 - MAX_SLIDER_VALUE
    uint8_t* SliderValue;
    
} GUI_Controll;

typedef struct GUI_ViewPort {

    // ViewPort méretei
    size_16 size;

    // ViewPort beli kép elmozgatása relatív a középre igazított pozícióhoz (CenteredVWImagePos)
    render_pos offset;

    // scale 1-es méretek
    size_16 BaseScaleImgSize;

    // x, y: aktuális zoomolás mellett a kép középre igazított x,y pozíciója
    render_pos CenteredImgPos;
    
    // ViewPort w/h
    double ratio;

    // ViewPort nagyítása
    // 1: középre igatítva a ViewPort méreteit kitöltve, képarányt nem torzít
    double ZoomScale;

    // Maximális ZoomScale
    // GUI_UpdateSize-ban frissül  ZoomScale = MAXZOOM x IMAGE.size / BaseScaleImageSize alapján
    double MaxZoomScale;

} GUI_ViewPort;


extern GUI_RENDER RENDER;
extern GUI_Controll CONTROLL;
extern GUI_ViewPort ViewPort;

// átáll editorrra a render
// imageSize: A beolvasott kép méretei
void RENDER_EditorraValt();
// létrehozza a betűk textúráját
void RENDER_INIT();

// Rendereli a ViewPort-ban megjelenítendő SDL_Texture-t
void Update_ImageTexture();

// Frissíti a ViewPort rendert
void Update_ViewPortRENDER();

// kilép
void RENDER_Quit();

#endif
