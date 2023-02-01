#include <graphx.h>
#include <tice.h>
#include <keypadc.h>
#include "main.h"
#include "bg.h"
#include "utils.h"
#include "gfx/gfx.h"

bool MainMenu(gfx_sprite_t *background, const int *x, const int *y, uint8_t count, bool renew, gfx_sprite_t* current_nyan_resized) {
    bool continue_loop = true;
    bool exit = false;
    bool previous_key = false;  // used to prevent the died screen from being skipped by a key not picked up !
    if (kb_AnyKey()) {
        previous_key = true;
        if (kb_Data[7] > 0) {  // If any arrow key pressed, skip menu !
            continue_loop = false;
        }
    }
    uint8_t zero = 0;  // used to get a pointer pointing to 0. Passing 0 directly as a pointer = NULL dereference !
    if (!renew) {
        gfx_GetSprite(background, (const int) *x, *y);
    }
    /* Draw the main sprite */
    DrawSprite(*x, *y, count, background, &zero, current_nyan_resized);
    /* Copy the buffer to the screen */
    gfx_BlitBuffer();
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);  // do not delete, timer is firstly activated here
    if (continue_loop) {
        do {
            if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
                /* Increment number of counts */
                count++;
                gfx_FillScreen(14);
                if (count == 13) {
                    count = 1;
                }
                timer_AckInterrupt(1, TIMER_RELOADED);
                gfx_ScaleSprite(nyancat_group[count - 1], current_nyan_resized);
                DrawSprite(*x, *y, count, background, &zero, current_nyan_resized);
                DrawBackground(count);
                PrintScaled("TI-NYAN", 50, 35, 32, true, 8, 0, false);
                PrintScaled("Press a key", 75, 170, 16, false, 1, 0, false);
                gfx_SetColor(7);
                for (uint24_t _x = 2; _x < 256; _x += 253) {
                    gfx_FillRectangle_NoClip(_x, 224, 2, 16);
                    gfx_FillRectangle_NoClip(_x + 62, 224, 2, 16);
                    gfx_FillRectangle_NoClip(_x + 2, 222, 60, 2);
                    gfx_FillRectangle_NoClip(_x + 1, 223, 2, 2);
                    gfx_FillRectangle_NoClip(_x + 61, 223, 2, 2);
                }
                gfx_TransparentSprite_NoClip(head, 26, 225);
                gfx_SetColor(1);
                gfx_FillTriangle_NoClip(22, 226, 17, 231, 22, 236);
                gfx_FillTriangle_NoClip(45, 226, 50, 231, 45, 236);
                gfx_FillRectangle_NoClip(276, 229, 6, 6);
                gfx_FillRectangle_NoClip(284, 229, 6, 6);
                gfx_FillRectangle_NoClip(292, 229, 6, 6);
                gfx_BlitBuffer();
            }
            if (kb_AnyKey()) {
                kb_Scan();
                if (kb_Data[6] == kb_Clear) {
                    exit = true;
                    continue_loop = false;
                }
                if (kb_Data[1] == kb_Yequ) {
                    // CHANGE SKIN
                    exit = true;
                    continue_loop = false;
                }
                if (kb_Data[1] == kb_Graph) {
                    // OTHER SCREEN
                    exit = true;
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
    }
    return !(exit);

}