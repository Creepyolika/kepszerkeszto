#include <string.h>

#include "../inc/main.h"
#include "../inc/RAM_handler.h"
#include "../inc/ERROR_handler.h"

#include "../inc/input_handler.h"

char* input = NULL;

// input tömb elemeinek számát számolja
// '\0' karakter is benne van
static size_t input_meret;

void INIT_input() {
    input = dmalloc(sizeof(char));
    Check_DyArraymalloc(input, 1);
    input[0] = '\0';
    input_meret = 1;
}

void Input_Bovit(char* str) {
    input = tomb_atmeretezo(input, input_meret, input_meret+strlen(str), sizeof(char));
    Check_DyArraymalloc(input, 1);

    strcat(input + input_meret-1, str);
    input_meret += strlen(str);

}

void Input_Torol() {
    if (input_meret > 1)  {
        input_meret -= 1;
        input = tomb_atmeretezo(input, input_meret+1, input_meret, sizeof(char));
        Check_DyArraymalloc(input, 1);
        input[input_meret-1] = '\0';
    }
}

void Input_RESET() {
    dfree(input);
    INIT_input();
}