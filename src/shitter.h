#ifndef shitter_include_file
#define shitter_include_file

#include "laser.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool defined;  // Defined is used to tell that the data should not be read and can be safely overwritten
    // This does not tell what the data is. It can be uninitialized, or already written with old values,
    // so remember to overwrite everything when setting defined to true !
    uint24_t x;
    uint8_t y;
    uint8_t angle;
    int8_t x_step;
    int8_t y_step;
    int8_t angle_step;
    uint8_t size;  // unused FOR NOW
    uint8_t step;
} Shitter;

typedef struct {
    uint8_t length;
    Shitter list[5];
    uint8_t delay;
} ShitterList;

extern void ShitterIAStep(ShitterList* sl, uint24_t seconds, LaserList* ll, const gfx_sprite_t* shit_resized, uint8_t* shitted_face, uint8_t* health_count, const int* x, const int* y, bool* died, uint24_t* killed);

#ifdef __cplusplus
}
#endif

#endif
