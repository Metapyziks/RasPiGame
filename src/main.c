#include <stdlib.h>
#include <time.h>

#include "lcd.h"
#include "sprite.h"
#include "map.h"

void idleFunc(void);
void displayFunc(void);

#define MAP_WIDTH 96
#define MAP_HEIGHT 64

static int cameraX = MAP_WIDTH / 2 * 16;
static int cameraY = MAP_HEIGHT / 2 * 16;

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
        cameraX - (DISPLAY_WIDTH >> 1), cameraY - (DISPLAY_HEIGHT >> 1),
        0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 1, 1);

    lcd_swapBuffers();
}

int main(void)
{
    int tiles = 0;
    tileset = sprite_fromFile("res/tileset.pic", &tilesetW, &tilesetH, &tiles);

    srand(time(NULL));

    curMap = map_new(MAP_WIDTH, MAP_HEIGHT);
    map_genForest(curMap, 0, 0, MAP_WIDTH, MAP_HEIGHT);

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
