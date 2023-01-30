#include <fileioc.h>
#include <string.h>
#include "io.h"

#define BEST_SCORE_FILE_NAME "NyanBest"
#define SKIN_FILE_NAME "NyanSkin"
#define FILE_READ "r"
#define FILE_WRITE "w"

uint8_t TryWriteBestScore(const uint24_t *seconds, const uint24_t *killed, const char name[13], BestScoreData *bsd) {
    uint8_t var;
    bsd->seconds = *seconds;
    bsd->killed_shitters = *killed;
    bsd->defined = true;
    strcpy(bsd->name, name);
    var = ti_Open(BEST_SCORE_FILE_NAME, FILE_WRITE);
    if (var == 0)
    {
        return 1;
    }
    if (ti_Write(bsd, sizeof(BestScoreData), 1, var) != 1)
    {
        return 1;
    }
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    return 0;
}

uint8_t TryReadBestScore(BestScoreData* bsd) {
    uint8_t var;
    var = ti_Open(BEST_SCORE_FILE_NAME, FILE_READ);
    if (var == 0) {
        return 1;
    }
    if (ti_Read(bsd, sizeof(BestScoreData), 1, var) != 1) {
        return 1;
    }
    return 0;

}


