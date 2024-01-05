#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

// textúrákhoz az útvonal
#define ASSET_PATH "assets/"

    /////////////////////////
    //      ViewPort      //
    ////////////////////////

// a kép 1 pixele maximum ennyi pixel szélességű lesz zoomolás közben
#define MAXZOOM 5

// Zoomolási együttható a zoomolás sebességére
// Lásd GUI_ViewPortZoom
#define ZOOM 0.04

    /////////////////////////
    //      CONTROLL      //
    ////////////////////////

// sliderek száma
#define SLIDER_COUNT 6

// Maximális slider érték - max 255
#define MAX_SLIDER_VALUE 100

// 1080 p monitoron ennyi pixel a méret
// monitor felbontásával arányosan változik az érétk
#define CONTROLL_WIDTH 300
#define CONTROLL_HEIGHT 600

#ifdef CONTROLL_C
// controll szineinek RGB kódja (8bit)
const uint8_t CONTROLL_BGND[3] = {100, 100, 100}; // háttér
const uint8_t SLIDER_COLOR[3] = {0, 0, 0}; // slider vonal szín

// Sliderek fölötti feliratok
// 300-as CONTROLL_WIDTH-hez max 20 karakter
const char* Slider_nevek[] = {
    "Elmosas",
    "Elesites",
    "Elkereses",
    "Sotetetes",
    "Vilagositas",
    "Kontraszt"
};

// slider ábrája
char SLIDER_CHAR[2] = {10, '\0'}; // a tömb első eleme a karakter ASCII_extended kódja
#endif

    /////////////////////////
    //       INPUT        //
    ////////////////////////

#ifdef INPUT_C
// Felirat nagyságok
// ez a szám még szorozva lesz MonitorInfo.w / 480.0 -val
// 0: cím, 1: szöveg, 2: input
const double scales[4] = {
    5, 3, 2, 3
};

// Felirat méretek
// 0: cím, 1: szöveg, 2: input
const int ypos[4] = {
    0,
    100,
    200,
    300
};

#endif


    /////////////////////////
    //     KORREKCIÓK     //
    ////////////////////////

// MacOS egyedisége miatt hasznos
#ifdef __APPLE__
#define MONITOR_SCALE_CORRECTION 0.7
#else
#define MONITOR_SCALE_CORRECTION 1
#endif

// Windows-oshoz igazítja a mac-es görgetés irányát
#ifdef __APPLE__
#define SCROLL_CORRECTION -1
#else
#define SCROLL_CORRECTION 1
#endif

#endif