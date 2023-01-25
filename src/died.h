#ifndef died_include_file
#define died_include_file

#ifdef __cplusplus
extern "C" {
#endif

extern bool ShowDiedScreen(gfx_sprite_t *background, const int *x, const int *y, uint8_t count, uint24_t *seconds,
                           const uint24_t *killed, gfx_sprite_t* shit_image);

#ifdef __cplusplus
}
#endif

#endif
