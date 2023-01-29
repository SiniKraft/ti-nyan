#include <keypadc.h>
#include <graphx.h>
#include <string.h>
#include <sys/timers.h>
#include "utils.h"
#include "bg.h"

#define X_POS 25
#define Y_POS 56
#define X_TEXT_POS (X_POS + 11)
#define X_TEXT_MAX (X_TEXT_POS + 248)
#define TEXT_FG_COLOR 7
#define TEXT_BG_COLOR 2
#define TEXT_HIGHLIGHT_COLOR 8  // Below the color is changed
#define INPUT_MAX_LENGTH 12
#define TIMER_ID 3  // Which timer to use ?
#define TIMER_TIME (32768 / 50)  // 20ms
#define TIMER_LONG_TIME (32768 / 3)  // 300ms

#define EXIT_CHAR  ((char)129)

char* AskName() {   // WARNING :  REQUIRED TO MAKE THIS WORK : gfx_SetMonospaceFont(8); IN INIT OF YOUR PROGRAM !
    static char input[INPUT_MAX_LENGTH + 1] = ""; // Returned ! Ensure to hold this memory !  (+1 is the final \0)
    uint8_t cc = 0;
    uint8_t sel = 0;
    uint8_t color = 0;
    int8_t index = 0;
    bool arrow_previous_key = false;
    char to_add;
    // bool add_char_pressed = false;   Enable if using it
    bool previous_del_key = false;
    bool arrow_pressed = false;

    // Optional dealing with timers for better accuracy and ease of use
    timer_Disable(TIMER_ID);
    timer_Set(TIMER_ID, TIMER_TIME);
    timer_SetReload(TIMER_ID, TIMER_TIME);

    // Added own routines to match with game design
    gfx_FillScreen(14);
    DrawBackground(0);
    PrintScaled("BEST SCORE !", 25, 15, 24, false, 8, 0, true);
    PrintScaled("Enter your name :", 78, 42, 10, false, 1, 0, false);
    gfx_SetColor(12);
    gfx_FillRectangle_NoClip(18, 209, 32, 16);
    gfx_SetColor(11);
    gfx_FillRectangle_NoClip(263, 209, 45, 16);
    gfx_SetTextBGColor(14);
    gfx_SetTextFGColor(7);
    gfx_PrintStringXY("Add char", 6, 230);
    gfx_PrintStringXY("del: remove", 116, 230);
    gfx_PrintStringXY("Confirm", 256, 230);
    gfx_SetTextBGColor(12);
    gfx_PrintStringXY("2nd", 22, 213);
    gfx_SetTextBGColor(11);
    gfx_PrintStringXY("alpha", 266, 213);

    kb_key_t key;
    gfx_SetColor(TEXT_FG_COLOR);
    gfx_FillRectangle_NoClip(X_POS, Y_POS, 269, 135);
    gfx_SetColor(TEXT_BG_COLOR);
    gfx_FillRectangle_NoClip(X_POS + 3, Y_POS + 3, 263, 129);
    gfx_SetTextBGColor(TEXT_BG_COLOR);
    gfx_SetTextFGColor(TEXT_FG_COLOR);
    bool previous_key = false;
    // TODO: show controls
    gfx_BlitBuffer();
    boot_WaitShort();
    kb_Scan();
    while (kb_Data[2] != kb_Alpha && kb_Data[6] != kb_Clear) {
        color++;
        if (color == 6) {
            color = 0;
        }  // makes color rainbow

        gfx_SetTextXY(X_TEXT_POS, Y_POS + 8);
        kb_Scan();
        key = kb_Data[7];
        // add_char_pressed = false;
        arrow_pressed = true;
        if (key == kb_Up) {
            sel -= 16;
            if (sel > 127) {
                sel += 16;
            }
        }
        else if (key == kb_Down) {
            sel += 16;
            if (sel > 127) {
                sel -= 16;
            }
        }
        else if (key == kb_Right && (sel + 1) % 16 != 0) {
            if (sel != 127) {
                sel += 1;
            }
        }
        else if (key == kb_Left && sel % 16 != 0) {
            if (sel != 0) {
                sel -= 1;
            }
        } else {
            arrow_pressed = false;
            arrow_previous_key = false;
        }

        if (kb_Data[6] == kb_Enter || kb_Data[1] == kb_2nd) {
            if (!previous_key) {
                if (strlen(input) < INPUT_MAX_LENGTH) {
                    to_add = (char) sel;
                    strncat(input, &to_add, 1);
                    // add_char_pressed = true;
                    previous_key = true;
                    index++;
                }
            }
        } else {
            previous_key = false;
        }
        if (kb_Data[1] == kb_Del) {
            if (!previous_del_key) {
                if (index != 0) {
                    input[index - 1] = '\0';
                    index -= 1;
                    previous_del_key = true;
                }
            }
        } else {
            previous_del_key = false;
        }

        if (kb_Data[6] == kb_Clear) {
            // Will exit
            input[0] = EXIT_CHAR;  // Can't be typed in by user
        }
        while (cc != 128) {
            if (sel == cc) {
                gfx_SetTextBGColor(TEXT_HIGHLIGHT_COLOR + color);
            }
            gfx_PrintChar((char)cc);
            gfx_SetTextBGColor(TEXT_BG_COLOR);
            if (gfx_GetTextX() != X_TEXT_MAX) {
                gfx_PrintChar(' ');
            }
            if (gfx_GetTextX() == X_TEXT_MAX) {
                gfx_SetTextXY(X_TEXT_POS, gfx_GetTextY() + 16);
            }
            cc++;
        }


        // Other stuff needed like rendering additional things
        // Render current input string
        gfx_SetColor(14);
        gfx_FillRectangle_NoClip(64, 202, 192, 16); // Override Previous text if any
        PrintScaled(input, 64, 202, 16, false, 1, 0, false);


        gfx_SetTextBGColor(TEXT_BG_COLOR);  // The routines above may have changed those values
        gfx_SetTextFGColor(TEXT_FG_COLOR);
        gfx_BlitBuffer();
        cc = 0;
        // if (add_char_pressed) {
        // }
        if (arrow_pressed) {
            if (!arrow_previous_key) {
                arrow_previous_key = true;
                timer_Set(TIMER_ID, TIMER_LONG_TIME);
                timer_SetReload(TIMER_ID, TIMER_LONG_TIME);
            } else {
                timer_Set(TIMER_ID, TIMER_TIME);
                timer_SetReload(TIMER_ID, TIMER_TIME);
            }
            timer_Enable(TIMER_ID, TIMER_32K, TIMER_0INT, TIMER_DOWN);
            while (!timer_ChkInterrupt(TIMER_ID, TIMER_RELOADED)) {
                kb_Scan();
                if (!kb_Data[7]) {  // If no arrows pressed, stop waiting
                    break;
                }
            }
            timer_SetReload(TIMER_ID, TIMER_TIME);
            timer_Set(TIMER_ID, TIMER_TIME);
            timer_AckInterrupt(TIMER_ID, TIMER_RELOADED);
            timer_Disable(TIMER_ID);
        } else {
            timer_Disable(TIMER_ID);
        }
    }
    return input;
}