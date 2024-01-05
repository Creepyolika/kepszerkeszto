#define SDL_MAIN_HANDLED // Hibát dob, elkezdi keresni a WinMain-t, ha nincs itt

#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/input_handler.h"
#include "inc/FILE_handler.h"
#include "inc/structs.h"
#include "inc/GUI_handler.h"
#include "inc/RAM_handler.h"
#include "inc/settings.h"
#include "inc/ERROR_handler.h"
#include "inc/img_processing.h"

#include "inc/main.h"

IMAGE_DATA IMAGE;

bool kep_beolvasva = false;

void Quit(int code) {
    if (kep_beolvasva) {
        tomb2D_free(IMAGE.color_data, 3);
        quit_processing();
    }
    dfree(input);
    GUI_Quit();
    release_remembered_addr();
    exit(code);
}


int main(int argc, char* argv[]) {

    redirect_stderr();
    INIT_RAM_handler();
    INIT_input();
    GUI_INIT();
    GUI_OpenInputWindow();

    SDL_Event event;

    // egér aktuális pozíciója
    // MOUSEMOTION alkalmával frissül
    render_pos mouse_pos;
    
    uint16_t VPwidth = 0;

    bool bal_gomb = false;
    // default true
    bool csak_VPUpdate = false;
    // default false
    bool process_img = false;
    bool first_update = true;

    int Slider_megfogva = -1;
    int slider_cursor_diff = 0;
    
    SDL_StartTextInput();
    Update_GUI(false, false);
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
        if (!kep_beolvasva) {
            switch (event.type) {
                case SDL_TEXTINPUT:
                    Input_Bovit(event.text.text);
                    goto update;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_BACKSPACE:
                            Input_Torol();
                            goto update;
                        case SDLK_RETURN:
                            if (!Check_FILE_HANDLER(kep_beolvas(input))) {
                                Input_RESET();
                                goto update;
                            }
                            init_processing();
                            SDL_StopTextInput();
                            VPwidth = GUI_EditorraValt();
                            IMAGE.path = input;
                            goto update;
                        default:
                            continue;
                    }
                    break;
                default:
                    if (first_update) {
                        first_update = false;
                        goto update;
                    }
                    continue;
            }

        } else {
            switch (event.type) {
                
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        VPwidth = Update_GUISize();
                        csak_VPUpdate = false;
                        goto update;
                    }
                    if (first_update) {
                        first_update = false;
                        goto update;
                    }
                    continue;

                case SDL_MOUSEWHEEL:
                    if (mouse_pos.x < VPwidth) {
                        Update_ViewPortZoom(event.wheel.y*SCROLL_CORRECTION);
                        goto update;
                    }
                    continue;

                case SDL_MOUSEMOTION:
                    SDL_GetMouseState((int*) &mouse_pos.x, (int*) &mouse_pos.y);

                    if (bal_gomb) {
                        // ViewPortban
                        if ( mouse_pos.x-event.motion.xrel < VPwidth && !(Slider_megfogva+1)) {
                            SetViewPortOffset(event.motion.xrel, event.motion.yrel);
                            goto update;
                        // controllban
                        } else {
                            if (Slider_megfogva > -1) {
                                Handle_SliderChange(mouse_pos.x, Slider_megfogva, slider_cursor_diff);
                                csak_VPUpdate = false;
                                goto update;
                            }
                        }
                    }
                    continue;

                // Egérgomb kezelés
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        bal_gomb = true;

                        Slider_megfogva = CheckForSliderContact(mouse_pos);
                        if (Slider_megfogva > -1) {
                            slider_cursor_diff = SliderCursorDiff(mouse_pos.x, Slider_megfogva);
                        }
                    }
                    continue;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        bal_gomb = false;
                        if(Slider_megfogva > -1) {
                            Slider_megfogva = -1;
                            process_img = true;
                            goto update;
                        }
                    }
                    continue;

                // billentyű lenyomás
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_r:
                        edit_list_free();
                        reset_slider_values();
                        csak_VPUpdate = false;
                        process_img = true;
                        goto update;
                    case SDLK_s:
                        Save_image();
                        break;
                    default:
                        continue;
                    }
                default:
                    if (first_update) {
                        first_update = false;
                        goto update;
                    }
                    continue;
            }
        }

update:
    Update_GUI(csak_VPUpdate, process_img);
    process_img = false;
    csak_VPUpdate = true;
    
    }

    Quit(0);
}



