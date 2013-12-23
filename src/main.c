#include "lcd.h"

int main(void)
{
    lcd_init();

    unsigned char old = lcd_getButtons();

    for (;;) {
        unsigned char new;

        while ((new = lcd_getButtons()) == old);

        printf("Buttons : %2x\n", new);

        old = new;
    }

    int delay = 200000;

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(0x00, i, 0x00));
        for (volatile int j = 0; j < delay; ++j);
    }

    for (int k = 0; k < 3; ++k) {
        for (int i = 0; i < 0x100; ++i) {
            lcd_clear(clr_fromRGB(0x00, 0xff - i, i));
            for (volatile int j = 0; j < delay; ++j);
        }

        for (int i = 0; i < 0x100; ++i) {
            lcd_clear(clr_fromRGB(i, 0x00, 0xff - i));
            for (volatile int j = 0; j < delay; ++j);
        }

        for (int i = 0; i < 0x100; ++i) {
            lcd_clear(clr_fromRGB(0xff - i, i, 0x00));
            for (volatile int j = 0; j < delay; ++j);
        }
    }

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(0x00, 0xff - i, 0x00));
        for (volatile int j = 0; j < delay; ++j);
    }

    lcd_stop();

    return 0;
}
