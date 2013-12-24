#include <unistd.h>

#include "lcd.h"
#include "sprites/tileset.c"

int main(void)
{
    lcd_init();

    unsigned char old = lcd_getButtons();

    static const color_t palette[4] = {
        CLR_FROM_RGB(0x18, 0x18, 0x18),
        CLR_FROM_RGB(0x58, 0xb8, 0xf8),
        CLR_FROM_RGB(0x80, 0xf8, 0x20),
        CLR_FROM_RGB(0xf8, 0xf8, 0xf8),
    };

    lcd_clear(CLR_BLACK);
    lcd_blitTilesPalette(tilemap, palette,
        16, 16, 30, 15, 0, 0, 320, 240);

    while (lcd_getButtons() == old) {
        usleep(10);
    }

    lcd_stop();

    return 0;
}
