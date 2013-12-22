#include "lcd.h"

int main()
{
    int x, y;

    lcd_init();

    for (y = 0; y < DISPLAY_HEIGHT; ++y) {
        for (x = 0; x < DISPLAY_WIDTH; ++x) {
            lcd_setPixel(x, y, clr_fromRGB(0x7f,
                (x * 0xff) / DISPLAY_WIDTH,
                (y * 0xff) / DISPLAY_HEIGHT));
        }
    }

    lcd_stop();

    return 0;
}
