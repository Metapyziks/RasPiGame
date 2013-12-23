#include "lcd.h"

int main(void)
{
    lcd_init();

    for (int i = 0; i < 0x100; ++i) {
        lcd_clear(clr_fromRGB(i, i, i));
    }
    
    sleep(5);
    lcd_clear(CLR_BLACK);

    lcd_stop();

    return 0;
}
