#include <stdbool.h>

#include "../../inc/main.h"
#include "../../inc/RAM_handler.h"
#include "../../inc/FILE_handler.h"
#include "../GUI_handler/inc/render.h"
#include "../../inc/ERROR_handler.h"

#include "processing_units.h"

#include "../../inc/img_processing.h"

static edit* edit_list = NULL;

void** PIXEL_DATA = NULL;

// hány byte egy színcsatorna tömbje
size_t ColorChannel_size = 0;

void handle_ValueChange(enum edit_types action, uint8_t value) {

    if(edit_list == NULL) {
        edit_list = (edit*) dmalloc(sizeof(edit));
        Check_DyArraymalloc(edit_list, 1);
        edit_list->action = action;
        edit_list->value = value;
        edit_list->kov = NULL;
        return;
    }

    edit* elem = edit_list;
check_elem:
    if (elem->action == action) {
        elem->value = value;
        return;
    }
    if (elem->kov != NULL) {
        elem = elem->kov;
        goto check_elem;
    }

    elem->kov = dmalloc(sizeof(edit));
    Check_DyArraymalloc(elem->kov, 1);
    elem = elem->kov;
    elem->action = action;
    elem->value = value;
    elem->kov = NULL;
}

void init_processing() {

    ColorChannel_size = IMAGE.size.w * IMAGE.size.h * IMAGE.color_depth;

    PIXEL_DATA = tomb2D_malloc(3, ColorChannel_size);
    Check_DyArraymalloc((void*) PIXEL_DATA, 3);

}

void** process() {
    // reset
    copy_2Dtomb(PIXEL_DATA, IMAGE.color_data, 3, ColorChannel_size);

    edit* edit = edit_list;
    for (; edit != NULL; edit = edit->kov) {
        switch (edit->action) {
        case BLUR:
            blur(edit->value);
            break;
        case SHARPEN:
            sharpen(edit->value);
            break;
        case EDGE_DETECTION:
            edge_detection(edit->value);
            break;
        case DARKEN:
            adjust_brightness(-1*edit->value);
            break;
        case BRIGHTEN:
            adjust_brightness(edit->value);
            break;
        case CONTRAST_INC:
            contrast_inc(edit->value);
            break;
        
        default:
            break;
        }
    }
    return PIXEL_DATA;
}

void Save_image() {
    if(PIXEL_DATA) {
        save_file(PIXEL_DATA);
        return;
    }
    save_file(IMAGE.color_data);
}

void edit_list_free() {
    edit* elem = edit_list;
    while (elem != NULL) {
        edit* kov = elem->kov;
        dfree(elem);
        elem = kov;
    }
    edit_list = NULL;
}

void quit_processing() {
    edit_list_free();
    tomb2D_free(PIXEL_DATA, 3);
    blur_free_kernel();
}