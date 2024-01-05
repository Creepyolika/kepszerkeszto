#include <stdlib.h>
#include <string.h>

#include "../../../inc/RAM_handler.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/structs.h"

static size_t prev_size = 0;
static uint32_t* kernel_index = NULL;

void convolve(uint8_t* src, double* kernel, size_16 src_size, size_t kernel_size, uint8_t depth, uint16_t maxval) {
    
    if (prev_size != kernel_size) {
        dfree(kernel_index);
        kernel_index = dmalloc(kernel_size*kernel_size*sizeof(uint32_t));
        Check_DyArraymalloc(kernel_index, 1);
        size_t ki = 0;
        for (uint32_t ky=0; ky<kernel_size; ky+=1) {
            for (uint32_t kx=0; kx<kernel_size; kx+=1) {
                kernel_index[ki] = ky*kernel_size + kx;
                ki +=1;
            }
        }
        prev_size = kernel_size;
    }


    int kernel_fel = kernel_size/2;
    uint32_t pitch = src_size.w*depth;
    size_t src_C_size = src_size.w*src_size.h*depth;

    uint8_t* temp = dmalloc(src_C_size);
    Check_DyArraymalloc(temp, 1);
    memset(temp, 0, src_C_size);

    for (int32_t y=0; y<src_size.h; y+=1) {
        for (int32_t x=0; x<src_size.w; x+=1) {

            double sum = 0;
            size_t ki_max = kernel_size * kernel_size;
            for (int ki=0; ki<ki_max; ki+=1) {
                int64_t xs = x + ki/kernel_size-kernel_fel;
                if (xs < 0) {
                    xs = -xs;
                } else if (xs >= src_size.w) {
                    xs = src_size.w+(src_size.w-xs);
                }

                int64_t ys = y + ki%kernel_size-kernel_fel;
                if (ys < 0) {
                    ys = -ys;
                } else if (ys >= src_size.h) {
                    ys = src_size.h+(src_size.h-ys);
                }

                int32_t num = 0;
                for (int s=0; s<depth; s+=1) {
                    num = num << s*8;
                    num += src[ys*pitch + (xs*depth)+s];
                }
                sum += kernel[kernel_index[ki]] * num;
            }
            sum = (sum > maxval) ? maxval : (sum < 0) ? 0 : sum;

            uint16_t num = sum+0.5;

            int px = y*pitch + x*depth;
            if (depth == 1) {
                temp[px] = num;
            } else {
                temp[px] = num >> 8;
                temp[px+1] = num;
            }
        }
    }
    memcpy(src, temp, src_C_size);
    dfree(temp);

}

void free_prev_indexes() {
    dfree(kernel_index);
}