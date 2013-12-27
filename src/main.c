#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "lcd.h"
#include "sprite.h"
#include "map.h"

void idleFunc(void);
void displayFunc(void);

static int cameraX = 8;
static int cameraY = 8;

static int tilesetW = 0;
static int tilesetH = 0;
static uint8_t* tileset = NULL;

static struct map curMap = { 0, 0, NULL };

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

    lcd_blitTilesPaletteScaled(tileset, palette, curMap, tilesetW, tilesetH,
        cameraX - 80, cameraY - 72, 0, 0, 320, 240, 1, 1);

    lcd_swapBuffers();
}

int main(void)
{
    int tiles = 0;
    tileset = sprite_fromFile("res/tileset.pic", &tilesetW, &tilesetH, &tiles);

    srand(time(NULL));

    curMap = map_new(64, 64);
    map_genForest(curMap, 0, 0, 64, 64);

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
