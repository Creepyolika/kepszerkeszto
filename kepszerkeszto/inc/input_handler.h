#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

extern char* input;

// lefoglalja a string memóriaterületét, elmenti a pointert RENDER.input-ba
void INIT_input();

// hozzááadja az új részt a stringhez
void Input_Bovit(char* str);

// törli az utolsó karaktert a stringből
void Input_Torol();

// reseteli az input stringet
void Input_RESET();

#endif