#include "gold.h"
#include <fileioc.h>

#define NyanGold_HEADER_SIZE 10

unsigned char *NyanGold_appvar[13];

unsigned char NyanGold_init(void)
{
    uint8_t appvar;
    unsigned int *table;
    void *base;
    unsigned char i;

    appvar = ti_Open("NyanGold", "r");
    if (appvar == 0)
    {
        return 0;
    }

    table = base = (char*)ti_GetDataPtr(appvar) + NyanGold_HEADER_SIZE;
    if (*table != 13)
    {
        ti_Close(appvar);
        return 0;
    }

    for (i = 0; i < 13; i++)
    {
        NyanGold_appvar[i] = (void*)(*++table + (unsigned int)base);
    }

    ti_Close(appvar);

    return 1;
}

