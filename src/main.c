#include "lcd.h"

int main(void)
{
    lcd_init();

    for (int i = 0; i < 10; ++i) {
        lcd_clear(CLR_WHITE);
        sleep(1);
        lcd_clear(CLR_BLACK);
        sleep(1);
    }

    lcd_stop();

    return 0;
}
