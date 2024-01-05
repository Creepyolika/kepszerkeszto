#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include "structs.h"

// képet tároló változó
extern IMAGE_DATA IMAGE;

// a kép beolvasás állapotát tárolja
extern bool kep_beolvasva;

void Quit(int code);

#endif