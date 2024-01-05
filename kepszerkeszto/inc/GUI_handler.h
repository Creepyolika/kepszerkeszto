#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

// GUI létrehozása
void GUI_INIT();

// kép input bekérése
void GUI_OpenInputWindow();

// Ablakméretek lekérése
// GUI elemek méreteinek beállítása azok alapján
// visszaadja a ViewPort szélességét
int Update_GUISize();

// Editor ablak létrehozása
void GUI_OpenEditorWindow();

// GUI tartalmának frissítése
//csak_ViewPort: csak a ViewPortot frissíti ha SDL_TRUE
// update_processed_image: kelle frissíteni a rendert
void Update_GUI(bool csak_ViewPort, bool update_processed_image);

// ViewPort Zoomolását kezeli
// Itt számolja ki, hogy mennyive kell elmozgatni a képet, hogy az aktuálisan megjelenített rész közeépére zoomoljon
// scroll: a görgetés mértéke
void Update_ViewPortZoom(Sint32 scroll);

// átvált editor ablakra
// visszaadja a ViewPort szélességét
int GUI_EditorraValt(); 

// ViewPort offset-hez adja hozzá dx-et x-hez, dy-t y-hoz
// xrel, yrel: egér relatív x, y menti elmozdulása
void SetViewPortOffset(int xrel, int yrel);

// Bekér egy ablakhoz relatív egérpozíciót
// visszaad egy 0 - SLIDER_COUNT-1 közötti számot, ha az egér rajta van valamelyiken
// -1-et ad vissza, ha nincs eggyiken se
int CheckForSliderContact(render_pos mouse_pos);

// Slider csúsztatás-t kezeli
// mouse_xpos: ablakhoz relatív x egérpozíció
// slider: a kezelni kívánt slider
// sldier_cursor_diff: SliderPos.x, mouse_pos.x különbsége
void Handle_SliderChange(int mouse_xpos, int slider, int slider_cursor_diff);

// visszaadja a cursor x, slider bal oldala közötti pixelek számát
int SliderCursorDiff(int mouse_xpos, int slider);

// 0-ra visszaállítja a slidereket;
void reset_slider_values();

// kilép
void GUI_Quit();

#endif
