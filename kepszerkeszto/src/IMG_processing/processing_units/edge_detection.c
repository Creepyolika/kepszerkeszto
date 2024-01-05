#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "../../../inc/main.h"
#include "../../../inc/RAM_handler.h"
#include "../../../inc/settings.h"

#include "../../../inc/img_processing.h"

static const double color_scale[3] = {0.2126, 0.7152, 0.0722};
static const int kernel_size = 3;

static const int kernel[3][3] = {
    {-4, 0, 4},
    {-5, 0, 5},
    {-4, 0, 4}
    };

void edge_detection(uint8_t value) {

    int32_t tresshold = value*7;

    int kernel_fel = kernel_size/2;

    memset(PIXEL_DATA[0], 0, ColorChannel_size);

    // grayscale
    for (int c=0; c<3; c+=1) {
        for (int y=0; y<IMAGE.size.h; y+=1) {
            for  (int x=0; x<IMAGE.size.w; x+=1) {

                int px = y*IMAGE.pitch + x*IMAGE.color_depth;

                uint16_t num = 0;
                for (int s=0; s<IMAGE.color_depth; s+=1) {
                    num = num << s*8;
                    num += ((uint8_t*)IMAGE.color_data[c])[px+s];
                }

                num *= color_scale[c];

                if (IMAGE.color_depth < 2) {
                    ((uint8_t*) PIXEL_DATA[0])[px] += num;
                } else {
                    ((uint8_t*) PIXEL_DATA[0])[px] += num >> 8;
                    ((uint8_t*) PIXEL_DATA[0])[px+1] += num;
                }
            }
        }
    }

    for (int64_t y=kernel_fel; y<IMAGE.size.h-kernel_fel; y+=1) {
        for (int64_t x=kernel_fel; x<IMAGE.size.w-kernel_fel; x+=1) {

            int32_t gx = 0;
            int32_t gy = 0;

            for (int ky=0; ky<kernel_size; ky+=1) {
                for (int kx=0; kx<kernel_size; kx+=1) {
                    int64_t xs = x + kx-kernel_fel;
                    int64_t ys = y + ky-kernel_fel;
                    
                    int32_t num = 0;
                    for (int s=0; s<IMAGE.color_depth; s+=1) {
                        num = num << s*8;
                        num += ((uint8_t*) PIXEL_DATA[0])[ys*IMAGE.pitch + (xs*IMAGE.color_depth)+s];
                    }

                    gx += kernel[kx][ky] * num;
                    gy += kernel[ky][kx] * num;

                }
            }

            uint16_t res;
            double magnitude = sqrt((double) gx*gx + (double) gy*gy);
            res = (magnitude > tresshold) ? IMAGE.maxval : 0;

            int px = y*IMAGE.pitch + x*IMAGE.color_depth;
            if (IMAGE.color_depth < 2) {
                ((uint8_t*) PIXEL_DATA[1])[px] = res;
            } else {
                ((uint8_t*) PIXEL_DATA[1])[px] = res >> 8;
                ((uint8_t*) PIXEL_DATA[1])[px+1] = res;
            }

        }
    }    
    memcpy(PIXEL_DATA[0], PIXEL_DATA[1], ColorChannel_size);
    memcpy(PIXEL_DATA[2], PIXEL_DATA[1], ColorChannel_size);
}