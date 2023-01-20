#include <graphx.h>
#include <keypadc.h>
#include <sys/rtc.h>
#include <sys/timers.h>
#include <sys/lcd.h>
#include <ti/real.h>
#include "gfx/gfx.h"
#include "bg.h"
#include "main_menu.h"
#include "utils.h"
#include "laser.h"
#include "shitter.h"

#define START_X ((LCD_WIDTH - nyancat_1_width * 2) / 2)
#define START_Y ((LCD_HEIGHT - nyancat_1_height * 2) / 2)
#define TIMER_FREQ      32768 /* Frequency of timer in Hz */
// #define ONE_SECOND      (TIMER_FREQ / 1)
#define TIME_2      (TIMER_FREQ / 10)
#define TIME_TO_WAIT    (TIMER_FREQ / 15)
#define MAX_X (321 - nyancat_1_width * 2)
#define MAX_Y (241 - nyancat_1_height * 2)


void DrawSprite(int x, int y, int frame, gfx_sprite_t *background);

void End(void);

void RenderHealth(uint8_t count);

int main(void) {
    uint8_t count = 1;
    srand(rtc_Time());
    bool previous_key = false;
    /* Create a buffer to store the background behind the sprite */
    gfx_UninitedSprite(background, nyancat_1_width * 2, nyancat_1_height * 2);
    /* Disable timer 1, so it doesn't run when setting the configuration */
    timer_Disable(1);
    timer_Disable(2);
    /* Count for 1 second. The counter will be reloaded once it reaches 0 */
    timer_Set(1, TIME_TO_WAIT);
    timer_SetReload(1, TIME_TO_WAIT);
    timer_Set(2, TIME_2);
    timer_SetReload(2, TIME_2);
    gfx_sprite_t* shit_resized = gfx_MallocSprite(26, 26);
    gfx_ScaleSprite(shit, shit_resized);
    background->width = nyancat_1_width * 2;
    background->height = nyancat_1_height * 2;
    /* Coordinates used for the sprite */
    int x = START_X;
    int y = START_Y;
    /* Initialize the graphics */
    gfx_Begin();
    /* Set the palette for the sprites */
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    /* Draw to the offscreen buffer */
    gfx_SetDrawBuffer();
    gfx_FillScreen(14);
    gfx_SetColor(1);

    if (!MainMenu(background, &x, &y, count)) {
        End();
        return 0;
    }

    int8_t to_walk;
    bool hori_move;
    uint24_t seconds = 0;
    uint8_t deciseconds = 0;
    timer_Enable(2, TIMER_32K, TIMER_0INT, TIMER_DOWN);
    uint8_t health_count = 3;
    bool should_update = true;
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
    shitter_list.list[4].defined = false;
    shitter_list.length = 5;
    shitter_list.delay = 0;
    char score_str[] = "00000000";
    char deci_score_str[] = "0";

    do {
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
            ShitterIAStep(&shitter_list, seconds, should_update, shit_resized);

            real_t tmp_real = os_FloatToReal((float)deciseconds);
            os_RealToStr(deci_score_str, &tmp_real, 1, 1, 0);
            real_t tmp_real_2 = os_FloatToReal((float)seconds);
            os_RealToStr(score_str, &tmp_real_2, 9, 1, 0);
            gfx_SetColor(14);
            gfx_FillRectangle_NoClip(304, 0, 16, 16);
            PrintScaled(deci_score_str, 304, 0, 16, false, 1, 0);
            gfx_SetColor(1);
            gfx_FillRectangle_NoClip(298, 10, 4, 4);
            uint8_t length = 0;
            for (length = 0; score_str[length] != '\0'; ++length);
            PrintScaled(score_str, 298 - 16*length, 0, 16, false, 1, 0);
            RenderHealth(health_count);
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

        DrawSprite(x, y, count, background);
        DrawBackground(count);
        UpdateAndRenderLasers(&laser_list);
        /* Copy the buffer to the screen */
        /* Same as gfx_Blit(gfx_buffer) */
        gfx_BlitBuffer();

    } while (kb_Data[6] != kb_Clear);
    End();
    return 0;
}

void RenderHealth(uint8_t count) {
    if (count > 0) {
        for (uint8_t i = 0; i < count; i++) {
            gfx_ScaledTransparentSprite_NoClip(health, 28*i, 0, 3, 3);
        }
    }
}

void End(void) {
    gfx_End();
}

/* Function for drawing the main sprite */
void DrawSprite(int x, int y, int frame, gfx_sprite_t *background) {
    static int oldX = START_X;
    static int oldY = START_Y;
    /* Render the original background */
    gfx_Sprite(background, oldX, oldY);
    gfx_sprite_t* nyancat_group[] = {
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
    gfx_ScaledTransparentSprite_NoClip(nyancat_group[frame - 1], x, y, 2, 2);

    oldX = x;
    oldY = y;
}

