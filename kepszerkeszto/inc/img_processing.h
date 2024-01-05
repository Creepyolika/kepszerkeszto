#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

#include <stdint.h>

enum edit_types {
    BLUR,
    SHARPEN,
    EDGE_DETECTION,
    DARKEN,
    BRIGHTEN,
    CONTRAST_INC
};

typedef struct edit {
    enum edit_types action;
    uint8_t value;

    struct edit* kov;
} edit;

extern void** PIXEL_DATA;

extern size_t ColorChannel_size;

// hezeli a láncolt-lista elemeit
// action: milyen változtatás történik, value: slider value
void handle_ValueChange(enum edit_types action, uint8_t value);

// feldolgozza a képet
// visszaadja arr[3][w][h] alakban
void** process();

// lefoglalja a memóriát, átmásolja a dolgokat;
void init_processing();

// elmenti a képet fájlba
void Save_image();

// felszabadítja a láncolt listát
void edit_list_free();

// felszabadítja a memóriát
void quit_processing();

#endif