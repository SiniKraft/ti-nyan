#include <graphx.h>
#include <keypadc.h>
#include <sys/rtc.h>
#include <sys/timers.h>
#include <sys/lcd.h>
#include <ti/real.h>
#include <ti/error.h>
#include <ti/screen.h>
#include <string.h>
#include <debug.h>
#include "gfx/gfx.h"
#include "bg.h"
#include "main_menu.h"
#include "utils.h"
#include "laser.h"
#include "shitter.h"
#include "died.h"
#include "io.h"
#include "best_score.h"
#include "skin_selector.h"

#define START_X ((LCD_WIDTH - nyancat_1_width * 2) / 2)
#define START_Y ((LCD_HEIGHT - nyancat_1_height * 2) / 2)
#define TIMER_FREQ      32768 /* Frequency of timer in Hz */
// #define ONE_SECOND      (TIMER_FREQ / 1)
#define TIME_2      (TIMER_FREQ / 10)
#define TIME_TO_WAIT    (TIMER_FREQ / 15)
#define MAX_X (321 - nyancat_1_width * 2)
#define MAX_Y (241 - nyancat_1_height * 2)

#define EXIT_CHAR  ((char)129)

gfx_sprite_t* nyancat_group[] = {  // global var which can only be edited by FindSkins
        nyancat_1,
        nyancat_2,
        nyancat_3,
        nyancat_4,
        nyancat_5,
        nyancat_6,
        nyancat_7,
        nyancat_8,
        nyancat_9,
        nyancat_10,
        nyancat_11,
        nyancat_12
};


void DrawSprite(int x, int y, int frame, uint8_t *shitted_face, gfx_sprite_t *current_nyan_resized);

void End(void);

void RenderHealth(uint8_t count, gfx_sprite_t *health_resized);

int main(void) {
    uint8_t count = 1;
    srand(rtc_Time());
    bool previous_key = false;
    uint8_t shitted_face = 0;
    /* Disable timer 1, so it doesn't run when setting the configuration */
    timer_Disable(1);
    timer_Disable(2);
    /* Count for 1 second. The counter will be reloaded once it reaches 0 */
    timer_Set(1, TIME_TO_WAIT);
    timer_SetReload(1, TIME_TO_WAIT);
    timer_Set(2, TIME_2);
    timer_SetReload(2, TIME_2);
    char file_name[9] = "\x81\0";
    TryReadCurrentSkin(file_name);

    gfx_sprite_t* shit_resized = gfx_MallocSprite(26, 26);
    gfx_sprite_t* health_resized = gfx_MallocSprite(27, 24);
    gfx_sprite_t* current_nyan_resized = gfx_MallocSprite(68, 42);
    if (health_resized == NULL || shit_resized == NULL || current_nyan_resized == NULL) {
        End();
        os_ClrHome();
        os_ThrowError(OS_E_MEMORY);
    }
    gfx_ScaleSprite(shit, shit_resized);
    gfx_ScaleSprite(health, health_resized);

    /* Coordinates used for the sprite */
    int x = START_X;
    int y = START_Y;
    bool died = false;
    /* Initialize the graphics */
    gfx_Begin();
    /* Set the palette for the sprites */
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    if (file_name[0] != EXIT_CHAR) {
        FindSkins(file_name);
    }
    /* Draw to the offscreen buffer */
    gfx_SetDrawBuffer();
    gfx_FillScreen(14);
    gfx_SetColor(1);
    gfx_SetMonospaceFont(8);

    // Read best score data

    static BestScoreData bsd;
    bsd.defined = false;
    TryReadBestScore(&bsd);
    if (bsd.defined) {
        if (bsd.name[0] == EXIT_CHAR) {
            char name[13] = "";
            strcpy(name, AskName());  // Avoid random memory errors
            uint24_t seconds = bsd.seconds;  // Avoid pointer change mess
            uint24_t killed_shitters = bsd.killed_shitters;
            TryWriteBestScore(&seconds, &killed_shitters, name, &bsd);
            if (name[0] == EXIT_CHAR) {
                End();
                return 0;
            }
        }
    }

    if (!MainMenu(&x, &y, count, current_nyan_resized, &bsd)) {
        End();
        return 0;
    }

    int8_t to_walk;
    bool hori_move;
    uint24_t seconds = 0;
    uint8_t deciseconds = 0;
    timer_Enable(2, TIMER_32K, TIMER_0INT, TIMER_DOWN);
    uint8_t health_count = 3;
    LaserList laser_list;
    laser_list.length = 2;
    laser_list.list[0].defined = false;
    laser_list.list[1].defined = false;
    laser_list.list[2].defined = false;
    ShitterList shitter_list;
    shitter_list.list[0].defined = false;
    shitter_list.list[1].defined = false;
    shitter_list.list[2].defined = false;
    shitter_list.list[3].defined = false;
    shitter_list.list[4].defined = false;  // Ensuring memory is set
    shitter_list.length = 5;
    shitter_list.delay = 0;
    uint24_t destroyed_shitters = 0;
    char score_str[] = "00000000";
    char deci_score_str[] = "0";

    do {
        if (died) {
            previous_key = false;
            shitted_face = 0;
            x = START_X;
            y = START_Y;
            died = false;
            if (!ShowDiedScreen(&x, &y, count, &seconds, &destroyed_shitters, shit, &bsd,
                                current_nyan_resized)) {
                End();
                return 0;
            } else {
                seconds = 0;
                deciseconds = 0;
                health_count = 3;
                laser_list.list[0].defined = false;
                laser_list.list[1].defined = false;
                laser_list.list[2].defined = false;
                shitter_list.list[0].defined = false;
                shitter_list.list[1].defined = false;
                shitter_list.list[2].defined = false;
                shitter_list.list[3].defined = false;
                shitter_list.list[4].defined = false;
                shitter_list.delay = 0;
                destroyed_shitters = 0;
                if (!MainMenu(&x, &y, count, current_nyan_resized, &bsd)) {
                    End();
                    return 0;
                }
            }
        }
        kb_key_t arrows;
        hori_move = false;
        to_walk = 4;
        /* Scan the keypad to update kb_Data */
        kb_Scan();
        /* Get the arrow key statuses */
        arrows = kb_Data[7];
        /* Check if any arrows are pressed */
        if (arrows) {
            /* Do different directions depending on the keypress */
            if (arrows & kb_Right || arrows & kb_Left) {
                hori_move = true;
            }
            if (arrows & kb_Up || arrows & kb_Down) {
                if (hori_move == true) {
                    to_walk = 3;
                }
            }
            if (arrows & kb_Right) {
                x += to_walk;
            }
            if (arrows & kb_Left) {
                x -= to_walk;
            }
            if (arrows & kb_Down) {
                y += to_walk;
            }
            if (arrows & kb_Up) {
                y -= to_walk;
            }
            if (x >= MAX_X) {
                x = MAX_X - to_walk;
            }
            if (y >= MAX_Y) {
                y = MAX_Y - to_walk;
            }
            if (x <= -1) {
                x = 0;
            }
            if (y <= -1) {
                y = 0;
            }


        }
        if (kb_Data[1] == kb_2nd) {
            if (!previous_key) {
                TryCreateLaser(x + 50, y + 30, &laser_list);
                previous_key = true;
            }
        } else {
            previous_key = false;
        }
        /* If the timer is reloaded, we reached 0 */
        if (timer_ChkInterrupt(1, TIMER_RELOADED)) {
            /* Increment number of counts */
            count++;
            gfx_FillScreen(14);
            if (count == 13) {
                count = 1;
            }
            /* Acknowledge the reload */
            timer_AckInterrupt(1, TIMER_RELOADED);
            ShitterIAStep(&shitter_list, seconds, &laser_list, shit_resized, &shitted_face, &health_count, &x, &y, &died, &destroyed_shitters);
            gfx_ScaleSprite(nyancat_group[count - 1], current_nyan_resized);
            real_t tmp_real = os_FloatToReal((float)deciseconds);
            os_RealToStr(deci_score_str, &tmp_real, 1, 1, 0);
            real_t tmp_real_2 = os_FloatToReal((float)seconds);
            os_RealToStr(score_str, &tmp_real_2, 9, 1, 0);
            gfx_SetColor(14);
            gfx_FillRectangle_NoClip(304, 0, 16, 16);
            PrintScaled(deci_score_str, 304, 0, 16, false, 1, 0, false);
            gfx_SetColor(1);
            gfx_FillRectangle_NoClip(298, 10, 4, 4);
            uint8_t length = 0;
            for (length = 0; score_str[length] != '\0'; ++length);
            PrintScaled(score_str, 298 - 16 * length, 0, 16, false, 1, 0, false);
            RenderHealth(health_count, health_resized);
        }

        if (timer_ChkInterrupt(2, TIMER_RELOADED)) {

            deciseconds++;
            if (deciseconds == 10) {
                deciseconds = 0;
                seconds++;
                if (seconds == 0) {  // security measure to prevent score being too high
                    seconds = 16777215;
                }
            }
            timer_AckInterrupt(2, TIMER_RELOADED);
        }
        /* Render the sprite */

        DrawSprite(x, y, count, &shitted_face, current_nyan_resized);
        DrawBackground(count);
        UpdateAndRenderLasers(&laser_list);
        /* Copy the buffer to the screen */
        /* Same as gfx_Blit(gfx_buffer) */
        gfx_BlitBuffer();

    } while (kb_Data[6] != kb_Clear);
    End();
    free(shit_resized);
    free(health_resized);
    free(current_nyan_resized);
    return 0;
}

void RenderHealth(uint8_t count, gfx_sprite_t *health_resized) {
    if (count > 0) {
        for (uint8_t i = 0; i < count; i++) {
            gfx_TransparentSprite_NoClip(health_resized, 28*i, 0);
        }
    }
}

void End(void) {
    gfx_End();
}

/* Function for drawing the main sprite */
void DrawSprite(int x, int y, int frame, uint8_t *shitted_face, gfx_sprite_t *current_nyan_resized) {
    static int oldX = START_X;
    static int oldY = START_Y;

    gfx_SetColor(14);
    gfx_FillRectangle_NoClip(oldX, oldY, nyancat_1_width * 2, nyancat_1_height *2);

    // gfx_ScaledTransparentSprite_NoClip(nyancat_group[frame - 1], x, y, 2, 2);
    gfx_TransparentSprite_NoClip(current_nyan_resized, x, y);
    if (*shitted_face > 0) {
        uint8_t factor_x = 36;
        uint8_t factor_y = 12;
        if (frame > 1) {
            factor_x = 38;
        }
        if (frame > 2) {
            factor_y = 14;
        }
        if (frame > 4) {
            factor_x = 36;
        }
        if (frame > 5) {
            factor_y = 12;
        }
        if (frame > 7) {
            factor_x = 38;
        }
        if (frame > 8) {
            factor_y = 14;
        }
        if (frame > 10) {
            factor_x = 36;
        }
        if (frame > 11) {
            factor_y = 12;
        }
        gfx_ScaledTransparentSprite_NoClip(shithead, x + factor_x, y + factor_y, 2, 2);
        gfx_SetColor(21);
        if (*shitted_face > 94) {
            uint8_t width = 6;
            if (*shitted_face > 98) {
                width = 6;
                gfx_FillRectangle_NoClip(x + factor_x - 5, y + factor_y + 6, 2, 2);
                gfx_FillRectangle_NoClip(x + factor_x - 5, y + factor_y + 20, 2, 2);
                gfx_FillRectangle_NoClip(x + factor_x + 31, y + factor_y + 6, 2, 2);
                gfx_FillRectangle_NoClip(x + factor_x + 31, y + factor_y + 20, 2, 2);
            } else {
                if (*shitted_face > 96) {
                    width = 4;
                    gfx_FillRectangle_NoClip(x + factor_x - 7, y + factor_y + 5, 2, 2);
                    gfx_FillRectangle_NoClip(x + factor_x - 7, y + factor_y + 21, 2, 2);
                    gfx_FillRectangle_NoClip(x + factor_x + 33, y + factor_y + 5, 2, 2);
                    gfx_FillRectangle_NoClip(x + factor_x + 33, y + factor_y + 21, 2, 2);
                } else {
                    if (*shitted_face > 94) {
                        width = 2;
                        gfx_FillRectangle_NoClip(x + factor_x - 9, y + factor_y + 4, 2, 2);
                        gfx_FillRectangle_NoClip(x + factor_x - 9, y + factor_y + 22, 2, 2);
                        gfx_FillRectangle_NoClip(x + factor_x + 35, y + factor_y + 4, 2, 2);
                        gfx_FillRectangle_NoClip(x + factor_x + 35, y + factor_y + 22, 2, 2);
                    }
                }
            }
            gfx_FillRectangle_NoClip(x + factor_x - 4 - width, y + factor_y + 11, width, 2);
            gfx_FillRectangle_NoClip(x + factor_x - 4 - width, y + factor_y + 15, width, 2);
            gfx_FillRectangle_NoClip(x + factor_x + 32, y + factor_y + 11, width, 2);
            gfx_FillRectangle_NoClip(x + factor_x + 32, y + factor_y + 15, width, 2);
        }

        *shitted_face -= 1;
    }

    oldX = x;
    oldY = y;
}

