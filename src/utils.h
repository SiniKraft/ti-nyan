#ifndef utils_include_file
#define utils_include_file

#ifdef __cplusplus
extern "C" {
#endif

extern void
PrintScaled(const char *text, const uint24_t x, const uint8_t y, const uint8_t size, bool is_title, uint8_t fg_color,
            uint8_t bg_color, bool is_best_score);

#ifdef __cplusplus
}
#endif

#endif
