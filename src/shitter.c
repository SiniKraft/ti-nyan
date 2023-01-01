#include <graphx.h>
#include <sys/rtc.h>
#include "shitter.h"

void ShitterIAStep(ShitterList* sl, uint24_t seconds) {
    // Adding 1 to global delay - used to wait before spawning a new shitter
    sl->delay++;
    if (sl->delay == 0) {
        sl->delay = 255;
    }
    // Add any shitter if necessary
    uint8_t chance = 0;
    srand(rtc_Time());
}