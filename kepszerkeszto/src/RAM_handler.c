#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "../inc/debugmalloc.h"

static void* remember = NULL;

void* dmalloc(size_t size) {
    if (size > LONG_MAX) {
        return NULL;
    }

    return malloc(size);
}

void dfree(void* addr) {
    if (addr != NULL) {
        free(addr);
    }
}

void* tomb_atmeretezo(void* tomb, size_t meret, size_t ujmeret, size_t elem_meret) {
    uint8_t* ujtomb = dmalloc(ujmeret * elem_meret);
    if (ujtomb == NULL) {
        return NULL;
    }

    uint8_t* t = tomb;
    meret = (ujmeret < meret) ? ujmeret : meret;

    for (uint16_t i = 0; i < meret; i++) {
        for (uint16_t j = 0; j < elem_meret; j++) {
            ujtomb[i * elem_meret + j] = t[i * elem_meret + j];
        }
    }

    dfree(tomb);
    return (void*) ujtomb;
}


void** tomb2D_malloc(size_t block_count, size_t block_size) {

    void** array = dmalloc(sizeof(void*) * block_count);
    if (array == NULL) {
        return NULL;
    }

    for (int i=0; i<block_count; i+=1) {
        array[i] = dmalloc(block_size);
    }

    return array;
}

void tomb2D_free(void** tomb, size_t block_count) {
    for (int i=0; i<block_count; i+=1) {
        dfree(tomb[i]);
    }
    dfree(tomb);
}

void* remember_dmalloc(size_t size) {
    void* addr = dmalloc(size);
    dfree(remember);
    remember = addr;
    return addr;
}

void release_remembered_addr() {
    dfree(remember);
    remember = NULL;
}

void copy_2Dtomb( void** dest, void** src, size_t block_count, size_t block_size) {
    for (int i=0; i<block_count; i+=1) {
        memcpy(dest[i], src[i], block_size);
    }
}

void INIT_RAM_handler() {
    debugmalloc_max_block_size(LONG_MAX);
    debugmalloc_log_file("debugmalloc_out.txt");
}