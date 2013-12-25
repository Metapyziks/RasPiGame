#include <unistd.h>

#include "lcd.h"
#include "sprite.h"

void idleFunc(void);
void displayFunc(void);

static int cameraX = 0;
static int cameraY = 0;

static uint16_t tilesetW = 0;
static uint16_t tilesetH = 0;
static uint8_t* tileset = NULL;

void idleFunc(void)
{
    if (lcd_buttonDown(BTN_1)) ++cameraY;
    if (lcd_buttonDown(BTN_2)) ++cameraX;
    if (lcd_buttonDown(BTN_3)) --cameraX;
    if (lcd_buttonDown(BTN_4)) --cameraY;
}

void displayFunc(void)
{
    static color_t palette[4] = {
        CLR_FROM_RGB(0x18, 0x18, 0x18),
        CLR_FROM_RGB(0x58, 0xb8, 0xf8),
        CLR_FROM_RGB(0x80, 0xf8, 0x20),
        CLR_FROM_RGB(0xf8, 0xf8, 0xf8),
    };

    static uint16_t tiles[100] = {
        226, 225, 226, 137,  17,  44,  44,  17, 137, 225,
        256, 255, 256, 137,  17,  44,  44,  17, 137, 255,
         17,  17,  17,  17,  17,  44,  44,  17,  17,  17,
         17,  17,  17,  17,  17, 225, 226,  17,  17,  17,
         44,  44,  44,  44,  44, 255, 256,  44,  44,  44,
         44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
        226, 225, 226, 137,  44,  44,  44,  44, 137, 225,
        256, 255, 256, 137,  44,  44,  44, 133, 137, 255,
        226, 225, 226, 137,  44,  44,  44,  44, 137, 225,
        256, 255, 256, 137,  17,  44,  44,  17, 137, 255,
    };

    lcd_blitTilesPaletteScaled(tileset, palette, 16, 16, tilesetW / 16,
        tiles, cameraX, cameraY, 160, 160, 80, 48, 160, 144, 1, 1);

    lcd_swapBuffers();
}

int main(void)
{
    tileset = sprite_fromFile("res/tileset.pic", &tilesetW, &tilesetH);

    lcd_init();
    lcd_clear(CLR_BLACK);
    lcd_idleFunc(idleFunc);
    lcd_displayFunc(displayFunc);
    lcd_mainLoop();
    lcd_clear(CLR_BLACK);
    lcd_stop();

    free(tileset);

    return 0;
}
