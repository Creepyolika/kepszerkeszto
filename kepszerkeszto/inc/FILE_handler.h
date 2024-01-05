#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// beolvassa a képet
enum FILE_HANDLER kep_beolvas(char* path);

// elmenti a képet
void save_file(void** pixel_data);

// Kiírja a hibát a hibafile-ba
void redirect_stderr();

#endif
