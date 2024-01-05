#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../../../inc/main.h"
#include "../../../inc/ERROR_handler.h"
#include "../../../inc/structs.h"
#include "../../../inc/RAM_handler.h"

#include "../inc/seged_fuggvenyek.h"


int parameter_beolvas(FILE* file) {

    // aktuális paraméter számjegyeit tárolja
    uint8_t* parameter_tomb = NULL;

    // hanyadik paraméternél tart a beolvasás
    uint8_t parameter = 0;

    // hány számjegyből áll a paraméter
    uint8_t parameter_meret = 0;

    bool elozo_parameter = false;
    bool kommentben = false;
    bool parameterek_bekerve = false;

    fseek(file, 2, SEEK_SET);
    uint8_t c = fgetc(file);
    while(!parameterek_bekerve) {
        if (!kommentben) {
            if (c == '#') {
                kommentben = true;
            } else {
                if (!parameterek_bekerve){

                    if (isspace(c) == 0 && elozo_parameter == false) {
                        elozo_parameter = true;
                        parameter_tomb = parameter_bovit(c, parameter_meret, parameter_tomb);
                        parameter += 1;
                        parameter_meret += 1;

                    } else if (isspace(c) == 0 && elozo_parameter == true ) {
                        parameter_tomb = parameter_bovit(c, parameter_meret, parameter_tomb);
                        parameter_meret += 1;

                    } else if (isspace(c) != 0 && elozo_parameter == true) {
                        elozo_parameter = false;
                        uint32_t szam = 0;
                        for (int i = 0; i<parameter_meret; i+=1) {
                            szam += (uint32_t) (parameter_tomb[i]-'0') * tizhatvany(parameter_meret-(i+1));
                        }
                        parameter_meret = 0;

                        if (szam > 0 && szam < 65536) {
                            switch (parameter)
                            {
                            case 1:
                                IMAGE.size.w = (uint16_t) szam;
                                break;
                            case 2:
                                IMAGE.size.h = (uint16_t) szam;
                                break;
                            case 3:
                                IMAGE.maxval = (uint16_t) szam;
                                parameterek_bekerve = true;
                                break;
                            default:
                                return FILE_NOT_VALID;
                            }
                        } else {
                            return FILE_NOT_VALID;
                        }
                    }
                } else {
                    break;
                }
            }
        } else {
            if (c == '\n') {
                kommentben = false;
            }
        }
        if(!parameterek_bekerve) {
            c = fgetc(file);
        }
    }
    if (IMAGE.maxval > 255) {
        IMAGE.color_depth = 2;
    } else {
        IMAGE.color_depth = 1;
    }

    IMAGE.pitch = IMAGE.size.w * IMAGE.color_depth;
    dfree(parameter_tomb);
    if ((uint64_t) IMAGE.size.w*IMAGE.size.h*IMAGE.color_depth > (uint64_t) LONG_MAX*3) {
        return FILE_TOO_BIG;
    }
    return SUCCESS;
}