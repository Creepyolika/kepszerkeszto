#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <SDL.h>

// unsigned 16 bites méretek tárolására
typedef struct size_16 {
    uint16_t w, h;
} size_16;

// rener pozíciók tárolására signed 32 bit
typedef struct render_pos {
    int32_t x, y;
} render_pos;


typedef struct IMAGE {

    char* path;

    // kép méretei
    size_16 size;

    // színmélység
    uint16_t maxval;

    // color_data[3][w*h*color_depth] formátumú
    // 0-2 : R-B
    // c, x,y : [c][y*pitch + x*color_depth]
    void** color_data;

    // egy szín hány byte-ot tartalmaz
    // 8 bites kép 1, 16 bites 2
    uint8_t color_depth;

    // egy sorhoz hány byte tartozik
    // size.w * color_depth
    uint32_t pitch;
    
} IMAGE_DATA;

#endif