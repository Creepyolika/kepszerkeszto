#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "../inc/main.h"
#include "../inc/generic_fuggvenyek.h"
#include "GUI_handler/inc/input_window.h"
#include "../inc/RAM_handler.h"

#include "../inc/ERROR_handler.h"

bool Check_FILE_HANDLER(enum FILE_HANDLER res) {

    switch (res) {
        case SUCCESS:
            return true;
        case FILE_NOT_FOUND:
            SetInputGUIError("File nem talalhato");
            break;
        case FILE_NOT_PPM:
            SetInputGUIError("A file nem szabvanyos ppm");
            break;
        case FILE_NOT_VALID:
            SetInputGUIError("A file serult");
            break;
        case FILE_TOO_BIG:
            SetInputGUIError("A kep tul nagy");
            break;
        default:
            fprintf(stderr, "Ismeretlen bemenet Check_FILE_HANDLER: %d", res);
            SetInputGUIError("Nem sikerult a filet beolvasni");
            break;
    }

    return false;
}

bool Check_SDL(intptr_t obj, int type, char* label, bool kritikus) {
    bool res = true;
    switch (type) {
        case FUNC_RES:
            if (obj != 0) {
                fprintf(stderr, StringAssembler("%s hibakodot kuldott: %d\n %s", label, obj, SDL_GetError()));
                res = false;
            }
            break;
        case SDL_PTR:
            if (obj == 0) {
                fprintf(stderr, StringAssembler("%s generalasa sikertelen:\n %s", label, SDL_GetError()));
                res = false;
            }
            break;
        default:
            fprintf(stderr, StringAssembler("Ismeretlen Check_SDL type bemenet\n label: %s\n %s", label, SDL_GetError()));
    }

    if (!res && kritikus) {
        Quit(1);
    }

    return res;
}

void Check_DyArraymalloc(void* arr, size_t block_count) {
    if(arr != NULL) {
        if (block_count > 1) {
            for (int i=0; i<block_count; i+=1) {
                if (((void**) arr)[i] == NULL) {
                    goto raise_err;
                }
            }
        }
        return;
    }
raise_err:
    fprintf(stderr, "Memória lefoglalása sikertelen");
    Quit(1);
}