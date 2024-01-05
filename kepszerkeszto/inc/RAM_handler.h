#ifndef RAM_HANDLER_H
#define RAM_HANDLER_H

#include <stdint.h>
#include <stddef.h>

// 1D tömb átméretező
// tomb: tömb
// meret: eredeti méret byteban
// ujmeret: új méret byteban
void* tomb_atmeretezo(void* tomb, size_t meret, size_t ujmeret, size_t elem_meret);

// 2D-s tömb lefolgalás
// num_blocks: ennyi egybefüggő memóriablockot rak az első ptr tömbbe
// block_size: Egy block adatszáma
// adat_meret : a leendő adatok mérete byteban
void** tomb2D_malloc(size_t num_blocks, size_t block_size);

// memóriát foglal adott mérettel
void* dmalloc(size_t size);

// memóriát szabadít fel
void dfree(void* addr);

// 2D-s tömböt felszabadít
// block_count: ennyi egybefüggő memóriablockot rak az első ptr tömbbe
void tomb2D_free(void** tomb, size_t block_count);

// Megjegyzi a foglalt címet. release_remembered()-el lehet felszabadítani.
// Ha úgy használjuk, hogy az előző megjegyzett cím nincs felszabadítva, felszabadítja azt
void* remember_dmalloc(size_t size);

// fellszabadítja az utoljára remember_dmalloc()-al foglalt területet
void release_remembered_addr();

// src-ből átmásolja dest-be a dolgokat
void copy_2Dtomb( void** dest, void** src, size_t block_count, size_t block_size);

// beállítja a debugmalloc-nak a max lefoglalható méretet SIZE_MAX-ra
void INIT_RAM_handler();

#endif