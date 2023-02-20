#ifndef main_menu_include_file
#define main_menu_include_file

#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool
MainMenu(const int *x, const int *y, uint8_t count, gfx_sprite_t *current_nyan_resized, BestScoreData *bsd,
         char skin_name[9]);

#ifdef __cplusplus
}
#endif

#endif
