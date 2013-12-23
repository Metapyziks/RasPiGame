#include "lcd.h"

int main(void)
{
    lcd_init();

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(0x00, 0xff - i, i));
        for (volatile int j = 0; j < 100000; ++j);
    }

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(i, 0x00, 0xff - i));
        for (volatile int j = 0; j < 100000; ++j);
    }

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(0xff - i, i, 0x00));
        for (volatile int j = 0; j < 100000; ++j);
    }

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(0x00, 0xff - i, 0x00));
        for (volatile int j = 0; j < 100000; ++j);
    }

    lcd_stop();

    return 0;
}
