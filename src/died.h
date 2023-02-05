#ifndef died_include_file
#define died_include_file

#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool ShowDiedScreen(const int *x, const int *y, uint8_t count, uint24_t *seconds, const uint24_t *killed,
                           gfx_sprite_t *shit_image, BestScoreData *bsd, gfx_sprite_t *current_nyan_resized);

#ifdef __cplusplus
}
#endif

#endif
