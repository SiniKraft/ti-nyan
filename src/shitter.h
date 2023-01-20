#ifndef shitter_include_file
#define shitter_include_file

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool defined;
    uint24_t x;
    uint8_t y;
    uint8_t angle;
    int8_t x_step;
    int8_t y_step;
    int8_t angle_step;
    uint8_t size;
    uint8_t step;
} Shitter;

typedef struct {
    uint8_t length;
    Shitter list[5];
    uint8_t delay;
} ShitterList;

extern void ShitterIAStep(ShitterList* sl, uint24_t seconds, __attribute__((unused)) bool update_shitter_coords, const gfx_sprite_t* shit_resized);

#ifdef __cplusplus
}
#endif

#endif
