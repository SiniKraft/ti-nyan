#include <keypadc.h>
#include <graphx.h>
#include <string.h>
#include <sys/timers.h>
#include "utils.h"
#include "bg.h"

char* AskName() {   // WARNING :  REQUIRED TO MAKE THIS WORK : gfx_SetMonospaceFont(8); IN INIT OF YOUR PROGRAM !
    static char inpt[10] = ""; // Returned !
    uint8_t cc = 0;
    uint8_t sel = 0;
    uint8_t color = 0;
    char to_add;
    bool pressed = false;
    bool pressed_2 = false;
    gfx_FillScreen(14);
    DrawBackground(0);
    PrintScaled("BEST SCORE !", 25, 15, 24, false, 8, 0, true);
    PrintScaled("Enter your name :", 90, 42, 10, false, 1, 0, false);
    kb_key_t key;
    gfx_SetColor(7);
    gfx_FillRectangle_NoClip(23, 56, 274, 136);
    gfx_SetColor(2);
    gfx_FillRectangle_NoClip(26, 59, 268, 130);
    gfx_SetTextBGColor(2);
    gfx_SetTextFGColor(7);
    // gfx_PrintStringXY("    SUPPR pour reecrire du debut.", 0, 214);
    // gfx_PrintStringXY(" Appuyez sur ALPHA ou (-) pour valider.", 0, 224);
    // TODO: Wait for key to be released when entering string
    // TODO: show controls
    // TODO: improve wait time
    gfx_BlitBuffer();
    boot_WaitShort();
    kb_Scan();
    while (kb_Data[5] != kb_Chs && kb_Data[2] != kb_Alpha && kb_Data[6] != kb_Clear) {
        color++;
        if (color == 6) {
            color = 0;
        }
        gfx_SetTextXY(32, 65);
        kb_Scan();
        key = kb_Data[7];
        pressed = false;
        pressed_2 = true;
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
            pressed_2 = false;
        }
        if (kb_Data[6] == kb_Enter || kb_Data[1] == kb_2nd) {
            if (strlen(inpt) < 9) {
                to_add = (char)sel/*+32*/;
                strcpy(&to_add, &to_add);
                strncat(inpt, &to_add, 1);
                pressed = true;
            }
        }
        if (kb_Data[1] == kb_Del || kb_Data[6] == kb_Clear) {
            char* _tmptt = "\0";
            inpt[0] = *_tmptt;
        }
        while (cc != 128) {
            if (sel == cc) {
                gfx_SetTextBGColor(8 + color);
            }
            gfx_PrintChar((char)cc/* + 32*/);
            gfx_SetTextBGColor(2);
            if (gfx_GetTextX() != 280) {
                gfx_PrintChar(*" ");
            }
            if (gfx_GetTextX() == 280) {
                gfx_SetTextXY(32, gfx_GetTextY() + 16);
            }
            cc++;
        }

        gfx_SetTextBGColor(2);

        gfx_PrintStringXY(inpt, 40, 180);
        gfx_SetTextBGColor(2);
        gfx_BlitBuffer();
        cc = 0;
        if (pressed) {
            boot_WaitShort();
        }
        if (pressed_2) {
            boot_WaitShort();
        }
    }
    return inpt;
}