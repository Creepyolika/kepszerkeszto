#ifndef SEGED_FUGGVENYEK_H
#define SEGED_FUGGVENYEK_H

#include <stdint.h>
#include <stdio.h>

// visszaadja tíz a kitevőiediken-t
int tizhatvany(int kitevo);

// Bővíti a parameter_beolvas függvény paramétertömbjét
// a régi tömböt felszabadítja
uint8_t* parameter_bovit(uint8_t c, uint8_t parameter_meret, uint8_t* parameter_tomb);

// visszaadja hány byte van pos, és a file vége között
size_t pixel_byte_count(FILE* file, long pos);

#endif