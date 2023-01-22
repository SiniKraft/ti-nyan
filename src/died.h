#ifndef died_include_file
#define died_include_file

#ifdef __cplusplus
extern "C" {
#endif

extern bool ShowDiedScreen(gfx_sprite_t *background, const int *x, const int *y, uint8_t count, const uint24_t* seconds, const uint24_t* killed);

#ifdef __cplusplus
}
#endif

#endif
