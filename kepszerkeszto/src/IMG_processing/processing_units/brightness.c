#include <stdint.h>

#include "../../../inc/main.h"
#include "../../../inc/settings.h"

#include "../../../inc/img_processing.h"

void adjust_brightness(int16_t value) {

    double brightness = 1 + ((double) value / MAX_SLIDER_VALUE);

    for (int c=0; c<3; c+=1) {
        for (int y=0; y<IMAGE.size.h; y+=1) {
            for (int x=0; x<IMAGE.size.w; x+=1) {

                int px = y*IMAGE.pitch + x*IMAGE.color_depth;

                uint32_t num = 0;
                for (int s=0; s<IMAGE.color_depth; s+=1) {
                    num = num << s*8;
                    num += ((uint8_t*)PIXEL_DATA[c])[px+s];
                }
                
                num *= brightness;
                if (num > IMAGE.maxval) {
                    num = IMAGE.maxval;
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