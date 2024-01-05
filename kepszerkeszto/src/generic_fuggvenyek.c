#include <stdarg.h>
#include <stdio.h>

#include "../inc/RAM_handler.h"
#include "../inc/ERROR_handler.h"

char* StringAssembler(char* str, ...) {
    va_list args;
    va_start(args, str);

    va_list args_copy;
    va_copy(args_copy, args);
    int length = vsnprintf(NULL, 0, str, args_copy);
    va_end(args_copy);

    char* assembeld_str = remember_dmalloc((length + 1) * sizeof(char));
    Check_DyArraymalloc(assembeld_str, 1);

    vsprintf(assembeld_str, str, args);

    va_end(args);
    return assembeld_str;
}