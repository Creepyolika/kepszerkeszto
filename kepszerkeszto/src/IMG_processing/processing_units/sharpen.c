#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/ERROR_handler.h"

#include "../../../inc/img_processing.h"
#include "generic.h"

static const int kernel_size = 3;

void sharpen(uint8_t value) {

    int kernel_fel = kernel_size/2;
    double strength = (double) value / MAX_SLIDER_VALUE;

    double kernel[9] = {
        0, -strength, 0,
        -strength, 0, -strength,
        0, -strength, 0
    };

    double sum_k = 0;
    for (int x=0; x<3; x+=1) {
        for (int y=0; y<3; y+=1) {
            sum_k -= kernel[y*kernel_size + x];
        }
    }
    kernel[kernel_fel*kernel_size + kernel_fel] = sum_k + 1;

    void* temp = dmalloc(ColorChannel_size);
    Check_DyArraymalloc(temp, 1);

    for (int c=0; c<3; c+=1) {
        convolve(PIXEL_DATA[c], kernel, IMAGE.size, kernel_size, IMAGE.color_depth, IMAGE.maxval);
    }
    dfree(temp);
}