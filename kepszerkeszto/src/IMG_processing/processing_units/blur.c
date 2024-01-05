#include <stdint.h>
#include <math.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/ERROR_handler.h"

#include "../../../inc/img_processing.h"
#include "generic.h"

static const double pi = 3.14159265358979323846264338327950288;

static double* kernel = NULL;
static uint32_t prev_size = 0;
static double kernel_size_scale = 0;
static int kernel_size;

static double gaussian_PDF(int x, int y, double sigma) {
    return exp(-(x*x + y*y) / (2*sigma*sigma)) / (2*pi*sigma*sigma);
}

static double* create_kernel(size_t size) {
    if(size < 2) {
        return NULL;
    }
    double* k = dmalloc(size*size*sizeof(double));
    Check_DyArraymalloc(k, 1);

    double sigma = (double) size / (2 * sqrt( 2 * log(size)));


    int offset = size / 2;
    for (int i=0; i<size*size; i+=1) {
        k[i] = gaussian_PDF((i % size) - offset, (i / size) - offset, sigma);
    }

    double sum = 0.0;
    for (int i=0; i<size*size; i+=1) {
        sum += k[i];
    }

    double kernel_scale = 1.0/sum;

    for (int i=0; i<size*size; i+=1) {
        k[i] *= kernel_scale;
    }
    return k;

}

void blur(uint8_t value) {
    if (value == 0) {
        return;
    }
    if(!kernel_size_scale) {
        kernel_size_scale = fmax(IMAGE.size.w, IMAGE.size.h)/ 5.0 / MAX_SLIDER_VALUE;
    }
    kernel_size = value*kernel_size_scale;
    kernel_size = (kernel_size/2)*2+1;
    if(prev_size != kernel_size) {
        prev_size = kernel_size;
        dfree(kernel);
        kernel = create_kernel(kernel_size);
    }
    if(kernel) {
        for (int c=0; c<3; c+=1) {
            convolve((uint8_t*) PIXEL_DATA[c], kernel, IMAGE.size, kernel_size, IMAGE.color_depth, IMAGE.maxval);
        }
    }
}

void blur_free_kernel() {
    dfree(kernel);
    free_prev_indexes();
}