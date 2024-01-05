#include <stdio.h>
#include <stdint.h>

#include "../../../inc/main.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/structs.h"

#include "../inc/seged_fuggvenyek.h"
#include "../inc/parameter_beolvas.h"

static enum FILE_HANDLER pixel_beolvas(FILE *file) {

    uint8_t ch;
    uint16_t num = 0;

    IMAGE.color_data = tomb2D_malloc(3, IMAGE.size.w*IMAGE.size.h*IMAGE.color_depth);
    Check_DyArraymalloc((void*) IMAGE.color_data, 3);

    ch = fgetc(file);

    for (int y=0; y<IMAGE.size.h; y+=1) {
        for (int x=0; x<IMAGE.size.w; x+=1) {
            for (int c=0; c<3; c+=1) {
                num = 0;
                for (int s=0; s<IMAGE.color_depth; s+=1) {
                    num = num << s*8;
                    num += ch;

                    ((uint8_t*)IMAGE.color_data[c])[y * IMAGE.pitch + x*IMAGE.color_depth + s] = ch;
                    ch = fgetc(file);
                }
                if (num > IMAGE.maxval) {
                    return FILE_NOT_VALID;
                }
            }
        }
    }
    return SUCCESS;
}

enum FILE_HANDLER beolvas(FILE* file) {
    int res;

    res = parameter_beolvas(file);
    if(!res) {
        if (!(pixel_byte_count(file, ftell(file)) < IMAGE.size.w*IMAGE.size.h*IMAGE.color_depth*3)) {
            res = pixel_beolvas(file);
        } else {
            res = FILE_NOT_VALID;
        }
    }

    return res;
}
