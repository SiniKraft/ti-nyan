#ifndef main_include_file
#define main_include_file

#ifdef __cplusplus
extern "C" {
#endif

extern void DrawSprite(int x, int y, int frame, gfx_sprite_t *background, uint8_t *shitted_face, gfx_sprite_t* current_nyan_resized);
extern gfx_sprite_t* nyancat_group [];


#ifdef __cplusplus
}
#endif

#endif
