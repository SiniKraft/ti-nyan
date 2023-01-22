#include <graphx.h>
#include <sys/timers.h>
#include <keypadc.h>
#include "main.h"
#include "bg.h"
#include "utils.h"

bool ShowDiedScreen(gfx_sprite_t *background, const int *x, const int *y, uint8_t count, const uint24_t* seconds, const uint24_t* killed) {
    bool should_continue_running = true;
    bool continue_loop = true;
    bool previous_key = false;  // used to prevent the died screen from being skipped by a key not picked up !
    if (kb_AnyKey()) {
        previous_key = true;
    }
    uint8_t zero = 0;  // used to get a pointer pointing to 0. Passing 0 directly as a pointer = NULL dereference !
    DrawSprite(*x, *y, count, background, &zero);
    gfx_BlitBuffer();  // Update screen
    do {
        if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
            /* Increment number of counts */
            count++;
            gfx_FillScreen(14);
            if (count == 13) {
                count = 1;
            }
            timer_AckInterrupt(1, TIMER_RELOADED);
            DrawSprite(*x, *y, count, background, &zero);
            DrawBackground(count);
            PrintScaled("You Died !", 12, 12, 32, false, 8, 0);
            PrintScaled("Press a key", 75, 209, 16, false, 1, 0);
            gfx_BlitBuffer();
        }
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
