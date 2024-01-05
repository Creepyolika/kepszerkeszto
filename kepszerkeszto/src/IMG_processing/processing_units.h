#ifndef PROCESS_UNIT_H
#define PROCESS_UNIT_H

#include <stdint.h>
#include "../../inc/structs.h"

// képfeldolgozó függvények
// definíciók a processing_units mappában
// esetleges hozzájuk tartozó takarító dolgok

void blur(uint8_t value);
void blur_free_kernel();

void sharpen(uint8_t value);
void edge_detection(uint8_t value);
void adjust_brightness(int16_t value);
void contrast_inc(uint8_t value);

void convolve(uint8_t* src, double* kernel, size_16 src_size, size_t kernel_size, uint8_t depth);

#endif