#include <stdint.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"

#include "../../../inc/img_processing.h"

void contrast_inc(uint8_t value) {

    double contrast = 1 + ((double) value / MAX_SLIDER_VALUE);
    int maxval_fel = IMAGE.maxval/2;

    for (int c=0; c<3; c+=1) {
        for (int y=0; y<IMAGE.size.h; y+=1) {
            for (int x=0; x<IMAGE.size.w; x+=1) {

                int px = y*IMAGE.pitch + x*IMAGE.color_depth;

                int32_t num = 0;
                for (int s=0; s<IMAGE.color_depth; s+=1) {
                    num = num << s*8;
                    num += ((uint8_t*)PIXEL_DATA[c])[px+s];
                }
                
                num = (num-maxval_fel) * contrast + maxval_fel;
                if (num > IMAGE.maxval) {
                    num = IMAGE.maxval;
                } else if (num < 0) {
                    num = 0;
                }

                if (IMAGE.color_depth < 2) {
                    ((uint8_t*) PIXEL_DATA[c])[px] = num;
                } else {
                    ((uint8_t*) PIXEL_DATA[c])[px] = num >> 8;
                    ((uint8_t*) PIXEL_DATA[c])[px+1] = num;
                }
                if (IMAGE.color_depth < 2) {
                    ((uint8_t*) PIXEL_DATA[c])[px] = num;
                } else {
                    ((uint8_t*) PIXEL_DATA[c])[px] = num >> 8;
                    ((uint8_t*) PIXEL_DATA[c])[px+1] = num;
                }
            }
        }
    }
}