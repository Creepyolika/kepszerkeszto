#include <stdio.h>

#include "../../inc/main.h"
#include "../../inc/structs.h"
#include "../../inc/ERROR_handler.h"
#include "../../inc/generic_fuggvenyek.h"
#include "../../inc/RAM_handler.h"
#include "inc/beolvas.h"

static enum FILE_HANDLER file_dekodol(FILE* file) {

    int magic_number[2];
    magic_number[0] = fgetc(file);
    magic_number[1] = fgetc(file);
    
    if (!(magic_number[0] == 'P' && (magic_number[1] == '6'))) {
        return FILE_NOT_PPM;
    } else {
        return beolvas(file);
    }
}

enum FILE_HANDLER kep_beolvas(char* path) {

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        return FILE_NOT_FOUND;
    }
    enum FILE_HANDLER res = file_dekodol(file);
    fclose(file);
    return res;
}

void save_file(void** pixel_data) {
    int pathlen = strlen(IMAGE.path);
    IMAGE.path[pathlen-4] = '\0';
    FILE* file = fopen(StringAssembler("%s_edited.ppm", IMAGE.path), "wb");

    char* header = StringAssembler("P6 %d %d %d\n", IMAGE.size.w, IMAGE.size.h, IMAGE.maxval);
    fwrite(header, sizeof(char), strlen(header), file);
    release_remembered_addr();

    for (int px=0; px<IMAGE.size.w*IMAGE.size.h; px+=1) {
        for (int c=0; c<3; c+=1) {
            for (int s=0; s<IMAGE.color_depth; s+=1) {
                fwrite(&((uint8_t*)pixel_data[c])[px+s], sizeof(uint8_t), 1, file);
            }
        }
    }
    fclose(file);
}



void redirect_stderr() {;
    FILE* err_file = freopen("kepfeldolgozo_error.txt", "w", stderr);
    if(err_file == NULL) {
        Quit(2);
    }
}