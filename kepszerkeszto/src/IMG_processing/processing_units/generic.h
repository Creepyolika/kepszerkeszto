#ifndef IMPRC_GENERIC_H
#define IMPRC_GENERIC_H

#include <stdint.h>
#include "../../../inc/structs.h"

// elvégzi a konvolúciót, az eredményt src-be másolja
// src, kernel: a két mátrix
// src_size: src magassága, szélessége
// depth: src színmélysége
// maxval: a kép max értéke
void convolve(uint8_t* src, double* kernel, size_16 src_size, size_t kernel_size, uint8_t depth, uint16_t maxval);

// felszabadítja az előző kiszámolt értékeket
void free_prev_indexes();

#endif