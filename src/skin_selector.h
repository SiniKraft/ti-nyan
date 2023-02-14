#ifndef skin_selector_include_file
#define skin_selector_include_file

#ifdef __cplusplus
extern "C" {
#endif

extern bool FindSkins(char name[9]);
extern bool SkinSelectorMenu(uint8_t count, gfx_sprite_t *current_nyan_resized, char name[9]);

#ifdef __cplusplus
}
#endif

#endif