#include <unistd.h>

#include "lcd.h"
#include "sprites/charizard.c"

int main(void)
{
    lcd_init();

    unsigned char old = lcd_getButtons();

    lcd_clear(BLACK);
    lcd_blitSpritePalette(charizardSprite, charizardPalette, 0, 0, 56, 56, 8, 8, 56, 56);

    while (lcd_getButtons() == old) {
        usleep(10);
    }

    lcd_stop();

    return 0;
}
