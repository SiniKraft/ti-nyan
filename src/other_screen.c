#include <graphx.h>
#include <keypadc.h>
#include <ti/real.h>
#include <string.h>
#include "io.h"
#include "utils.h"

bool ShowOtherScreen(gfx_sprite_t *shit_image, BestScoreData *bsd) {
    bool should_continue_running = true;
    bool continue_loop = true;
    bool previous_key = false;  // used to prevent the screen from being skipped by a key not picked up !
    if (kb_AnyKey()) {
        previous_key = true;
    }
    gfx_BlitBuffer();  // Update screen

    gfx_FillScreen(14);
    PrintScaled("HOW TO PLAY ?", 7, 6, 24, false, 8, 0, true);
    PrintScaled("Survive as long as possible.", 22, 37, 10, false, 10, 0, false);
    PrintScaled("Arrow keys: Move around", 22, 49, 10, false, 11, 0, false);
    PrintScaled("2nd: Fire laser", 22, 61, 10, false, 12, 0, false);
    PrintScaled("Clear: quit (without saving)", 22, 74, 10, false, 13, 0, false);
    if (bsd->defined) {
        // BUILD ALL STRING NEEDED, REFER TO 'died.c' FOR MORE INFO
        char seconds_str[6] = "0\0\0\0\0\0";
        real_t seconds_real = os_FloatToReal((float)bsd->seconds);
        os_RealToStr(seconds_str, &seconds_real, 6, 1, 0);
        char killed_str[6] = "0\0\0\0\0\0";
        real_t killed_real = os_FloatToReal((float)bsd->killed_shitters);
        os_RealToStr(killed_str, &killed_real, 6, 1, 0);
        char seconds_built_str[18] = "Survived \0\0\0\0\0\0\0\0\0";
        strcat(seconds_built_str, seconds_str);
        strcat(seconds_built_str, "s !");
        char killed_built_str[18] = "And killed \0\0\0\0\0\0\0";
        strcat(killed_built_str, killed_str);
        uint8_t length_k = 0;
        for (; killed_built_str[length_k] != '\0'; length_k++);
        uint24_t seconds_str_x = ((GFX_LCD_WIDTH - ((strlen(killed_built_str) + 2) * 10)) / 2);  // strlen + 2 = consider shit sprite space (assuming the shit sprite = 10px);
        // To get the x pos, here we calculate how much space the text width will take (because the length of the string is variable)

        PrintScaled("BEST SCORE:", 36, 97, 24, false, 8, 0, true);
        PrintScaled(bsd->name, ((GFX_LCD_WIDTH - (strlen(bsd->name) * 16)) / 2), 128, 16, false, 8, 0, false);
        PrintScaled(seconds_built_str, ((GFX_LCD_WIDTH - (strlen(seconds_built_str) * 16)) / 2), 146, 16, false, 9, 0, false);
        PrintScaled(killed_built_str, seconds_str_x, 165, 10, false, 10, 0, false);
        gfx_TransparentSprite_NoClip(shit_image, seconds_str_x + (length_k * 10) + 10, 163);
    }
    PrintScaled("Code by SiniKraft", 76, 188, 10, false, 11, 0, false);
    gfx_SetMonospaceFont(0);
    gfx_SetTextBGColor(14);
    gfx_SetTextFGColor(12);
    gfx_PrintStringXY("https://github.com/SiniKraft", 60, 201);
    gfx_SetMonospaceFont(8);
    PrintScaled("Press a key", 75, 219, 16, false, 1, 0, false);

    gfx_BlitBuffer();

    do {
        if (kb_AnyKey()) {
            kb_Scan();
            if (kb_Data[6] == kb_Clear) {
                should_continue_running = false;
                continue_loop = false;
            }
            if (!previous_key) {
                continue_loop = false;
                previous_key = true;
            }
        } else {
            previous_key = false;
        }
    } while (continue_loop);
    return should_continue_running;
}
