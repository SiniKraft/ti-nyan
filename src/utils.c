#include <graphx.h>
#include <ti/screen.h>
#include <ti/error.h>

void PrintScaled(const char *text, const uint24_t x, const uint8_t y, const uint8_t size, bool is_title, uint8_t color,
                 uint8_t bg_color, bool is_best_score) {
    gfx_SetTextBGColor(bg_color);
    gfx_SetTextFGColor(color);
    uint8_t length = 0;
    for (length = 0; text[length] != '\0'; ++length);
    gfx_sprite_t *char_img;
    char_img = gfx_MallocSprite(size, size);
    gfx_TempSprite(ori_char, 8, 8);
    if (char_img == NULL || ori_char == NULL) {
        gfx_End();
        os_ClrHome();
        os_ThrowError(OS_E_MEMORY);
    } else {
        for (uint8_t index = 0; index < length; index++) {
            ori_char = gfx_GetSpriteChar(text[index]);
            gfx_ScaleSprite(ori_char, char_img);
            gfx_TransparentSprite_NoClip(char_img, x + (index * size), y);
            if (is_title) {
                color++;
                if (index == 1) {
                    color = 7;
                } else if (index == 2) {
                    color = 10;
                }
                gfx_SetTextFGColor(color);
            }
            if (is_best_score) {
                // 8 - 13
                color++;
                if (color == 14) {
                    color = 8;
                }
                gfx_SetTextFGColor(color);
            }
        }
    }
    // free(ori_char); CAUSE MEMORY ERROR
    free(char_img);
}