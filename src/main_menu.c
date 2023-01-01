#include <graphx.h>
#include <tice.h>
#include <keypadc.h>
#include "main.h"
#include "bg.h"
#include "utils.h"

bool MainMenu(gfx_sprite_t *background, const int *x, const int *y, uint8_t count) {
    bool continue_loop = true;
    bool exit = false;
    gfx_GetSprite(background, *x, *y);
    /* Draw the main sprite */
    DrawSprite(*x, *y, count, background);
    /* Copy the buffer to the screen */
    gfx_BlitBuffer();
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
    do {
        if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
            /* Increment number of counts */
            count++;
            gfx_FillScreen(14);
            if (count == 13) {
                count = 1;
            }
            timer_AckInterrupt(1, TIMER_RELOADED);
            DrawSprite(*x, *y, count, background);
            DrawBackground(count);
            PrintScaled("TI-NYAN", 50, 35, 32, true, 8, 0);
            PrintScaled("Press a key", 75, 170, 16, false, 1, 0);
            gfx_BlitBuffer();
        }
        if (kb_AnyKey()) {
            if (kb_Data[6] == kb_Clear) {
                exit = true;
            }
            continue_loop = false;
        }
    } while (continue_loop);
    return !(exit);

}