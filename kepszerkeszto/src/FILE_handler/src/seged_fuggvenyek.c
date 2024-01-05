#include <stdint.h>
#include <stdio.h>

#include "../../../inc/RAM_handler.h"
#include "../../../inc/ERROR_handler.h"

int tizhatvany(int kitevo) {
    int r = 1;
    while (kitevo > 0) {
        r = r*10;
        kitevo -= 1;
    }
    return r;
}

uint8_t* parameter_bovit(uint8_t c, uint8_t parameter_meret, uint8_t* parameter_tomb) {

    uint8_t* ujtomb = tomb_atmeretezo(parameter_tomb, parameter_meret, parameter_meret+1, sizeof(char));
    Check_DyArraymalloc(ujtomb, 1);

    ujtomb[parameter_meret] = c;
    return ujtomb;
}

size_t pixel_byte_count(FILE* file, long pos) {

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, pos, SEEK_SET);

    return fileSize - pos;
}