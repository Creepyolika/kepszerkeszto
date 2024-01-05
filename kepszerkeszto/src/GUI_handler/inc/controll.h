#ifndef CONTROLL_H
#define CONTROLL_H

#include <SDL.h>
#include <stdint.h>

// Frissíti egy adott slider pozícióját a CONTROLL_SURFACE-en
void Update_SliderBox(uint16_t slider);

// Létrehozza a ControllSurface tartalmát a 0-ról
void Update_ControllSurface();

// Létrehozza controllt. Ha már van, csak átméretezi a surface-t.
void INIT_Controll();

// Frissíti a gui controllt
void Update_ControllTexture();

// kirajzolja a controll textúrát az ablakra
void Draw_Controll();

// kilép
void controll_Quit();

#endif