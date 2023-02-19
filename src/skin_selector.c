#include <cdefs.h>
#include <fileioc.h>
#include <string.h>
#include <graphx.h>
#include <compression.h>
#include <ti/error.h>
#include <keypadc.h>
#include <sys/timers.h>
#include <ti/screen.h>
#include "bg.h"
#include "main.h"
#include "utils.h"
#include "io.h"
#include "gfx/global_palette.h"

#define HEADER_SIZE 10
#define DETECT_STRING "TINYANSKIN"

bool FindSkins(char name[9]) {  // Name can be "" - specified if looking for specific skin
    char *var_name;
    void *vat_ptr = NULL;
    vat_ptr = NULL;
    uint8_t appvar_num= 0;
    uint8_t appvar = 0;
    while ((var_name = ti_Detect(&vat_ptr, DETECT_STRING))) {
        if (var_name != NULL) {
            appvar_num++;
        }
    }
    char **name_list;
    name_list=(char **)malloc(appvar_num*sizeof(char *));
    if (name_list == NULL) {
        return true;
    }
    for (int i=0;i<appvar_num;i++)
    {
        name_list[i]=(char*)malloc(9*sizeof(char));
        if (name_list[i] == NULL) {
            return true;
        }
    }

    vat_ptr = NULL;
    appvar_num = 0;
    while ((var_name = ti_Detect(&vat_ptr, DETECT_STRING))) {
        if (var_name != NULL) {
            strcpy(name_list[appvar_num], var_name);
            appvar_num++;
        }
    }
    unsigned int *table;
    void *base;
    unsigned char ii;
    unsigned char *appvar_list[13];
    for (uint8_t i = 0; i < appvar_num; i++) {
        appvar = ti_Open(name_list[i], "r");
        if (appvar == 0)
        {
            return false;
        }
        table = base = (char*)ti_GetDataPtr(appvar) + HEADER_SIZE;
        if (*table != 13)
        {
            ti_Close(appvar);
            return false;
        }
        for (ii = 0; ii < 13; ii++)
        {
            appvar_list[ii] = (void*)(*++table + (unsigned int)base);
        }

        ti_Close(appvar);
        if (strcmp(name_list[i], name) == 0) {
            for (uint8_t y = 0; y < 12; y++) {
                zx0_Decompress(nyancat_group[y], appvar_list[y + 1]);
            }
            gfx_SetPalette(appvar_list[0], 510, 0);
        }
    }
    for (uint8_t i_=0; i_<appvar_num; i_++) {
        free(name_list[i_]);
    }
    free(name_list);
    return false;
}

bool SkinSelectorMenu(uint8_t count, gfx_sprite_t *current_nyan_resized, char name[9]) {
    bool continue_loop = true;
    bool exit = false;
    bool previous_key = false;  // used to prevent the died screen from being skipped by a key not picked up !
    const int x = ((GFX_LCD_WIDTH - 34 * 2) / 2);
    const int y = ((GFX_LCD_HEIGHT - 21 * 2) / 2);
    unsigned int *table;
    void *base;
    unsigned char ii;
    unsigned char *appvar_list[13];
    uint8_t zero = 0;
    int8_t current_index = 0;
    if (kb_AnyKey()) {
        previous_key = true;
    }
    char *var_name;
    void *vat_ptr = NULL;
    vat_ptr = NULL;
    uint8_t appvar_total= 0;
    uint8_t appvar_num= 0;
    uint8_t appvar = 0;
    while ((var_name = ti_Detect(&vat_ptr, DETECT_STRING))) {
        if (var_name != NULL) {
            appvar_total++;
        }
    }
    char **name_list;
    name_list=(char **)malloc(appvar_total*sizeof(char *));
    for (int i=0;i<appvar_total;i++)
    {
        name_list[i]=(char*)malloc(9*sizeof(char));
    }

    vat_ptr = NULL;
    appvar_num = 0;
    while ((var_name = ti_Detect(&vat_ptr, DETECT_STRING))) {
        if (var_name != NULL) {
            strcpy(name_list[appvar_num], var_name);
            appvar_num++;
        }
    }
    vat_ptr = NULL;
    appvar_num = 0;
    while ((var_name = ti_Detect(&vat_ptr, DETECT_STRING))) {
        if (var_name != NULL) {
            if (strcmp(name, var_name) == 0) {
                current_index = (int8_t)appvar_num;
            }
            appvar_num++;
        }
    }
    bool should_refresh_skin = true;

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
                DrawSprite(x, y, count, &zero, current_nyan_resized);
                DrawBackground(count);
                gfx_SetColor(7);
                for (uint24_t _x = 2; _x < 256; _x += 253) {
                    gfx_FillRectangle_NoClip(_x, 224, 2, 16);
                    gfx_FillRectangle_NoClip(_x + 62, 224, 2, 16);
                    gfx_FillRectangle_NoClip(_x + 2, 222, 60, 2);
                    gfx_FillRectangle_NoClip(_x + 1, 223, 2, 2);
                    gfx_FillRectangle_NoClip(_x + 61, 223, 2, 2);
                }
                gfx_SetColor(1);
                gfx_FillTriangle_NoClip(115, 114, 106, 120, 115, 126);
                gfx_FillTriangle_NoClip(206, 114, 215, 120, 206, 126);
                PrintScaled("MENU", 14, 227, 10, false, 1, 14, false);
                PrintScaled("SELECT NYAN", 73, 20, 16, false, 1, 14, false);
                PrintScaled("DEFAULT", 259, 228, 8, false, 1, 14, false);
                gfx_BlitBuffer();
            }
            if (kb_AnyKey()) {
                kb_Scan();
                if (kb_Data[6] == kb_Clear) {
                    exit = true;
                    continue_loop = false;
                }
                if (!previous_key) {
                    continue_loop = false;
                    previous_key = true;
                    if (kb_Data[7] == kb_Right) {
                        current_index += 1;
                        if (current_index > (appvar_total - 1)) {
                            current_index = 0;
                        }
                        should_refresh_skin = true;
                        continue_loop = true;
                    }
                    if (kb_Data[7] == kb_Left) {
                        current_index -= 1;
                        if (current_index == -1) {
                            current_index = (int8_t)((int8_t)appvar_total - 1);
                        }
                        should_refresh_skin = true;
                        continue_loop = true;
                    }
                    if (kb_Data[1] == kb_Graph) {
                        TryWriteCurrentSkin("\x81\0");
                        gfx_End();
                        os_ClrHome();
                        os_RunPrgm("TINYAN", NULL, 0, NULL);
                    }
                }
            } else {
                previous_key = false;
            }
            if (should_refresh_skin) {
                appvar = ti_Open(name_list[current_index], "r");
                if (appvar == 0) {
                    gfx_End();
                    os_ClrHome();
                    if (appvar_total == 0) {
                        strcpy(os_AppErr1, "NoSkinsFound");  // 12 null terminated string
                        os_ThrowError(OS_E_APPERR1);
                    } else {
                        os_ThrowError(OS_E_INVALID);
                    }
                }
                table = base = (char *) ti_GetDataPtr(appvar) + HEADER_SIZE;
                if (*table != 13) {
                    ti_Close(appvar);
                }
                for (ii = 0; ii < 13; ii++) {
                    appvar_list[ii] = (void *) (*++table + (unsigned int) base);
                }
                ti_Close(appvar);
                for (uint8_t _y = 0; _y < 12; _y++) {
                    zx0_Decompress(nyancat_group[_y], appvar_list[_y + 1]);
                }
                should_refresh_skin = false;
                TryWriteCurrentSkin(name_list[current_index]);
                gfx_SetPalette(appvar_list[0], 510, 0);
            }
        } while (continue_loop);
    }
    for (uint8_t i_=0; i_<appvar_total; i_++) {
        free(name_list[i_]);
    }
    free(name_list);
    return !(exit);
    // TODO: Redo Sprite
    // TODO: Randomize shit
    // TODO: Skin maker
}