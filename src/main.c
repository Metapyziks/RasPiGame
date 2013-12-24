#include <unistd.h>

#include "lcd.h"
#include "sprites/tileset.c"

int main(void)
{
    lcd_init();

    unsigned char old = lcd_getButtons();

    static color_t palette[4] = {
        CLR_FROM_RGB(0x18, 0x18, 0x18),
        CLR_FROM_RGB(0x58, 0xb8, 0xf8),
        CLR_FROM_RGB(0x80, 0xf8, 0x20),
        CLR_FROM_RGB(0xf8, 0xf8, 0xf8),
    };

    static unsigned short int tiles[90] = {
        226, 225, 226, 137,  17,  44,  44,  17, 137, 225, 
        256, 255, 256, 137,  17,  44,  44,  17, 137, 255, 
         17,  17,  17,  17,  17,  44,  44,  17,  17,  17, 
         17,  17,  17,  17,  17, 225, 226,  17,  17,  17, 
         44,  44,  44,  44,  44, 255, 256,  44,  44,  44, 
         44,  44,  44,  44,  44,  44,  44,  44,  44,  44, 
        226, 225, 226, 137,  44,  44,  44,  44, 137, 225, 
        256, 255, 256, 137,  44,  44,  44,  44, 137, 255, 
        226, 225, 226, 137,  44,  44,  44,  44, 137, 225, 
    };

    lcd_clear(CLR_BLACK);
    
    lcd_blitTilesPalette(tileset, palette,
        16, 16, 30, tiles, 0, 0, 160, 144, 0, 0, 160, 144);

    while (lcd_getButtons() == old) {
        usleep(10);
    }

    lcd_clear(CLR_BLACK);

    lcd_stop();

    return 0;
}
