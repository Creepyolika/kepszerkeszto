#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

enum FILE_HANDLER {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    FILE_NOT_PPM,
    FILE_NOT_VALID,
    FILE_TOO_BIG
};

// FUNC_RES: függvény álltal visszaadott hibajelző értéket néz
// SDL_PTR: függvény álltal visszaadott termékre mutató pointer NULL létét nézi
enum SDL_CHECK {
    FUNC_RES,
    SDL_PTR
};


// ellenőrzi a FILE_HANDLER outputot
// annak megfelelően jelzi a hibát
bool Check_FILE_HANDLER(enum FILE_HANDLER res);

// SDL-t ellenőrzibool
// obj: a tesztelendő érték, type: SDL_CHECK eleme, label: ezzel fogja azonosítani hibaüzenetben az okozó sort kritikus: kilépjen-e a program, ha hibás
bool Check_SDL(intptr_t obj, int type, char* label, bool kritikus);

// RAM lefoglaalt memória terület NULL létét ellenőrzi
// több különálló memóriablockot is kezel, block_count szerint
void Check_DyArraymalloc(void* arr, size_t block_count);

#endif