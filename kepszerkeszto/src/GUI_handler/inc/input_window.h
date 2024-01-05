#ifndef INPUT_H
#define INPUT_H

// létrehozza a szükséges textúrákat
void INIT_inputWindow();

// frissíti az input ablakot
void Update_inputWindow();

// Beállítja a megjelenítendő hibaüzenetet
void SetInputGUIError(char* str);

// kilép
void inputWindow_Quit();

#endif