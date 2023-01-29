#include <fileioc.h>
#include <string.h>
#include "io.h"

uint8_t TryWriteBestScore(const uint24_t* seconds, const uint24_t *killed, const char name[13]) {
    uint8_t var;
    BestScoreData bsd;
    bsd.seconds = *seconds;
    bsd.killed_shitters = *killed;
    strcpy(bsd.name, name);
    var = ti_Open("NyanBest", "w");
    if (var == 0)
    {
        return 1;
    }
    if (ti_Write(&bsd, sizeof(bsd), 1, var) != 1)
    {
        return 1;
    }
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    return 0;
}

uint8_t TryReadBestScore(BestScoreData* bsd) {
    uint8_t var;
    var = ti_Open("NyanBest", "r");
    if (var == 0) {
        return 0;
    }
    if (ti_Read(bsd, sizeof(BestScoreData), 1, var) != 1) {
        return 0;
    }
    return 1;

}


