#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <SDL.h>
#include "../../../inc/structs.h"

// Lekéri az aktuális ablakméreteket, eltárolja RENDER->WindowSize-ba
void Window_UpdateSize();

// Ablak létrehozása
// init_w, init_h: Létrehozási méretek
// min_w, min_h: minimum ablakméretek
// flags: SDL flagek az ablakhoz
void Window_INIT(int init_w, int init_h, int min_w, int min_h, Uint32 flags);

//Ablak bezárása
void Window_destroy();

#endif
