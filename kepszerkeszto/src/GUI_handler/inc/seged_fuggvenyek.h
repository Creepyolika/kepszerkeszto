#ifndef RENDER_SEGED_H
#define RENDER_SEGED_H

#include <SDL.h>
#include "../../../inc/structs.h"

// átméretez egy SDL_Surface-t, tartalmát torzítva átmásolja
SDL_Surface* ReSize_Surface(SDL_Surface* surface, size_16 NewSize);

// Visszaad egy SDL_Surface-t a megadott szöveggel
// "" esetén 1x17-es üres surfacet ad vissza
SDL_Surface* CreateSurface_TextLine(char* szoveg);

// Visszaad egy SDL_Texture-t a megadott szöveggel
SDL_Texture* CreateTexture_TextLine(SDL_Renderer* renderer, char* szoveg);

// beállítja az ASCII globális változót
void SetAscii(SDL_Surface* ascii);

// felszabadítja az ASCII változót
void FreeAscii();

#endif