#include <graphx.h>
#include "laser.h"

void TryCreateLaser(uint24_t x, uint8_t y, LaserList *laser_list) {
    for (uint8_t i = 0; i < laser_list->length; i++) {
        if (!(laser_list->list[i].defined)) {
            laser_list->list[i].defined = true;
            laser_list->list[i].x = x;
            laser_list->list[i].y = y;
            break;
        }
    }
}

void UpdateAndRenderLasers(LaserList *laser_list) {

    for (uint8_t i = 0; i < laser_list->length; i++) {
        if (laser_list->list[i].defined) {
            gfx_SetColor(14);
            gfx_FillRectangle_NoClip(laser_list->list[i].x - 7, laser_list->list[i].y, 15, 2);
            gfx_SetColor(8);
            gfx_FillRectangle_NoClip(laser_list->list[i].x, laser_list->list[i].y, 15, 2);
            laser_list->list[i].x += 7;
            if ((laser_list->list[i].x + 12) > 320) {
                laser_list->list[i].defined = false;
            }
        }
    }
}