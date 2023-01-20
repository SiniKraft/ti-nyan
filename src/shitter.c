#include <graphx.h>
#include "shitter.h"
#include "gfx/gfx.h"

uint8_t randUInt(uint8_t max) {
    uint8_t a = rand() % (max + 1);
    return a;
}

void ShitterIAStep(ShitterList* sl, uint24_t seconds, __attribute__((unused)) bool update_shitter_coords, const gfx_sprite_t* shit_resized) {
    // Adding 1 to global delay - used to wait before spawning a new shitter
    uint8_t second = (uint8_t)seconds;
    uint8_t to_wait = 40;
    if (seconds > 10) {
        to_wait = 30;
    }
    if (seconds > 30) {
        to_wait = 20;
    }
    if (seconds > 45) {
        to_wait = 10;
    }
    if (seconds > 60) {
        to_wait = 5;
    }
    if (seconds > 255) {
        second = 255;
    }
    sl->delay++;
    if (sl->delay == 0) {
        sl->delay = 255;
    }
    // Add any shitter if necessary
    uint8_t chance = 0;
    int24_t ran = 0;
    if (sl->delay > to_wait) {
        // chance = (255 / to_wait) - (sl->delay / to_wait);  // 255 = DELAY_MAX
        // ran = rand();
        //if (ran < (RAND_MAX / chance)) {
        // Add shitter
        sl->delay = 0;
        bool found = false;
        for (uint8_t i = 0; i < sl->length; i++) {
            if (!(found)) {
                if (!(sl->list[i].defined)) {
                    found = true;
                    int8_t speed = (int8_t)(-6 * ((second/15) + 1));
                    if (speed < -126) {
                        speed = -127;  // security measure to prevent too high speed
                    }
                    sl->list[i].defined = true;
                    sl->list[i].x = 290;
                    sl->list[i].y = randUInt(209) + 1;
                    sl->list[i].angle = randUInt(255);
                    sl->list[i].x_step = speed;
                    sl->list[i].y_step = 0;
                    sl->list[i].angle_step = (int8_t)randUInt(24);
                    sl->list[i].size = 160;
                    sl->list[i].step = 0;
                }
            }
        }
        //}
    }
    // Render Shitter
    for (uint8_t i = 0; i < sl->length; i++) {
        if (sl->list[i].defined) {
            gfx_RotatedTransparentSprite_NoClip(shit_resized, sl->list[i].x, sl->list[i].y, sl->list[i].angle);
            // dbg_printf("Sprite no. %d x:%d y:%d\n", i, sl->list[i].x, sl->list[i].y);
            sl->list[i].x += sl->list[i].x_step;
            sl->list[i].y += sl->list[i].y_step;
            sl->list[i].angle += sl->list[i].angle_step;
            if (sl->list[i].x > 320 || sl->list[i].y > 240) {
                sl->list[i].defined = false;
            }

        }
    }
    //while (true) {
    //    ran = rand();
    //    if (ran < (RAND_MAX / 2)) {
    //        dbg_printf("TRUE %d\n", ran);
    //    } else {
    //        dbg_printf("FALSE %d\n", ran);
    //    }
    //}
}