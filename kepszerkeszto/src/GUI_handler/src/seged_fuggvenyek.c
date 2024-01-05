#include <SDL.h>
#include <stdint.h>

#include "../../../inc/structs.h"
#include "../../../inc/ERROR_handler.h"

// ASCII táblázat
static SDL_Surface* ASCII = NULL;

SDL_Surface* ReSize_Surface(SDL_Surface* surface, size_16 NewSize) {
    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, NewSize.w, NewSize.h, 24, 0, 0, 0, 0);
    Check_SDL((intptr_t) new_surface, SDL_PTR, "ReSize_Surface()/new_surface", true);

    SDL_BlitScaled(surface, NULL, new_surface, NULL);
    SDL_FreeSurface(surface);
    return new_surface;
}

SDL_Surface* CreateSurface_TextLine(char* szoveg) {
    
    uint16_t textSurface_len = 1;
    SDL_Rect src = {.w = 9, .h = 17};
    SDL_Rect dest = {.y = 0, .h = 17};
    
    if (szoveg[0] != '\0') {
        textSurface_len = src.w*strlen(szoveg);
    }

    dest.w = textSurface_len;

    SDL_Surface* surface = SDL_CreateRGBSurface(0, textSurface_len, src.h, 24, 0, 0, 0, 0);
    Check_SDL((intptr_t) surface, SDL_PTR, "CreateSurface_TextLine()/surface", true);

    uint16_t i = 0;
    while(szoveg[i] != '\0') {
        src.x = ((uint8_t) szoveg[i] % 16) * src.w;
        src.y = ((uint8_t) szoveg[i] / 16) * src.h;

        dest.x = (dest.w*i);

        SDL_BlitSurface(ASCII, &src, surface, &dest);
        i += 1;
    }

    return surface;
}

SDL_Texture* CreateTexture_TextLine(SDL_Renderer* renderer, char* szoveg) {

    SDL_Surface* surface = CreateSurface_TextLine(szoveg);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    Check_SDL((intptr_t) texture, SDL_PTR, "CreateTexture_TextLine()/texture", true);

    SDL_FreeSurface(surface);
    return texture;

}

void SetAscii(SDL_Surface* ascii) {
    ASCII = ascii;
}

void FreeAscii() {
    SDL_FreeSurface(ASCII);
}