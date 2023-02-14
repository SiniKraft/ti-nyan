#include "pirate.h"
#include <fileioc.h>

#define PirNyate_HEADER_SIZE 10

unsigned char *PirNyate_appvar[13];

unsigned char PirNyate_init(void)
{
    uint8_t appvar;
    unsigned int *table;
    void *base;
    unsigned char i;

    appvar = ti_Open("PirNyate", "r");
    if (appvar == 0)
    {
        return 0;
    }

    table = base = (char*)ti_GetDataPtr(appvar) + PirNyate_HEADER_SIZE;
    if (*table != 13)
    {
        ti_Close(appvar);
        return 0;
    }

    for (i = 0; i < 13; i++)
    {
        PirNyate_appvar[i] = (void*)(*++table + (unsigned int)base);
    }

    ti_Close(appvar);

    return 1;
}

