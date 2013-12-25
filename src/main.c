#include <unistd.h>

#include "lcd.h"
#include "sprite.h"

int main(void)
{
    lcd_init();

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

    uint16_t tilesetW = 0;
    uint16_t tilesetH = 0;
    uint8_t* tileset = NULL;

    tileset = sprite_fromFile("res/tileset.pic", &tilesetW, &tilesetH);

    lcd_clear(CLR_BLACK);

    int x = 0;
    int y = 0;

    do {
        if (lcd_buttonDown(BTN_1)) ++y;
        if (lcd_buttonDown(BTN_2)) ++x;
        if (lcd_buttonDown(BTN_3)) --x;
        if (lcd_buttonDown(BTN_4)) --y;

        lcd_blitTilesPalette(tileset, palette, 16, 16, tilesetW / 16,
            tiles, x, y, 160, 160, 0, 0, 320, 240, 2, 2);

        usleep(8000);
    } while (!lcd_buttonDown(BTN_1 | BTN_4));

    lcd_clear(CLR_BLACK);

    lcd_stop();

    free(tileset);

    return 0;
}
