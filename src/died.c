#include <graphx.h>
#include <sys/timers.h>
#include <keypadc.h>
#include <ti/real.h>
#include <string.h>
#include "main.h"
#include "bg.h"
#include "utils.h"
#include "best_score.h"
#include "io.h"

#define EXIT_CHAR  ((char)129)

bool ShowDiedScreen(gfx_sprite_t *background, const int *x, const int *y, uint8_t count, uint24_t *seconds,
                    const uint24_t *killed, gfx_sprite_t *shit_image, BestScoreData *bsd) {
    bool should_continue_running = true;
    bool continue_loop = true;
    bool previous_key = false;  // used to prevent the died screen from being skipped by a key not picked up !
    if (kb_AnyKey()) {
        previous_key = true;
    }
    uint8_t zero = 0;  // used to get a pointer pointing to 0. Passing 0 directly as a pointer = NULL dereference !
    DrawSprite(*x, *y, count, background, &zero);
    gfx_BlitBuffer();  // Update screen
    if (*seconds > 9999) {
        *seconds = 9999;  // Assuming the player can't survive more than 2,7 hours
    }

    bool best_score;
    if (bsd->defined) {
        if (bsd->seconds < *seconds) {
            best_score = true;
        } else {
            best_score = false;
        }
    } else {
        best_score = true;
    }

    // Convert the seconds number into a string :
    char seconds_str[6] = "0\0\0\0\0\0";  // Allocate memory
    real_t seconds_real = os_FloatToReal((float)*seconds);
    os_RealToStr(seconds_str, &seconds_real, 6, 1, 0);  // The minimum maxLength is 6
    // Processing the second string
    char killed_str[6] = "0\0\0\0\0\0";  // Alloc enough to hold 6 chars.
    real_t killed_real = os_FloatToReal((float)*killed);
    os_RealToStr(killed_str, &killed_real, 6, 1, 0);  // uint24_t can have up to 8 digits,
    // But this func ensure that the max output is 6 length. Assuming you can't shoot more than 99999(9) shitters.


    // Now building the string we need to show !
    char seconds_built_str[22] = "You survived \0\0\0\0\0\0\0\0\0";  // Ensuring enough memory is allocated.
    // Result returned by os_RealToStr can be 6 length !
    strcat(seconds_built_str, seconds_str);
    strcat(seconds_built_str, "s !");
    // seconds_built_str can now be 'You survived 9999s !'
    // Processing killed str :
    char killed_built_str[22] = "And you killed \0\0\0\0\0\0\0";
    strcat(killed_built_str, killed_str);

    // checking length of killed_built_str to determine where to place shitter.
    uint8_t length_k = 0;
    for (; killed_built_str[length_k] != '\0'; length_k++);
    // Each character is 10 pixels width.
    uint24_t shitter_x_pos = 50 + (length_k * 10) + 10;  // + 10 add empty white space

    uint8_t color_index = 0;


    do {
        if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
            /* Increment number of counts */
            count++;
            gfx_FillScreen(14);
            if (count == 13) {
                count = 1;
            }
            timer_AckInterrupt(1, TIMER_RELOADED);
            DrawSprite(*x, *y, count, background, &zero);
            DrawBackground(count);
            PrintScaled("You Died !", 12, 12, 32, false, 8, 0, false);
            PrintScaled(seconds_built_str, 12, 54, 16, false, 1, 0, false);
            PrintScaled(killed_built_str, 50, 75, 10, false, 1, 0, false);
            gfx_TransparentSprite_NoClip(shit_image, shitter_x_pos, 72);
            PrintScaled("Press a key", 75, 209, 16, false, 1, 0, false);
            if (best_score) {
                color_index++;
                if (color_index == 6) {
                    color_index = 0;
                }
                PrintScaled("BEST SCORE !", 15, 170, 24, false, 8 + color_index, 0, true);
            }
            gfx_BlitBuffer();
        }
        if (kb_AnyKey()) {
            kb_Scan();
            if (kb_Data[6] == kb_Clear) {
                should_continue_running = false;
                continue_loop = false;
            }
            if (!previous_key) {
                continue_loop = false;
                previous_key = true;
            }
        } else {
            previous_key = false;
        }
    } while (continue_loop);
    if (should_continue_running) {
        if (best_score) {
            // Implement Enter name section. Add Undefined byte to allow player to define name if skipped
            char name[13] = "";
            strcpy(name, AskName());  // Avoid random memory errors
            if (name[0] == EXIT_CHAR) {  // this char cannot be typed in by user, it's associated with exit.
                // Note that the default font. Only go through 0-128 characters customs can have up to 256
                should_continue_running = false;
            }
            TryWriteBestScore(seconds, killed, name, bsd);
        }
    }
    return should_continue_running;
}
