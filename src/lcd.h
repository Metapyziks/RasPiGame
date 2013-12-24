#ifndef _LCD_H_
#define _LCD_H_ 1

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "utils.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define CLR_FROM_RGB(r, g, b) ((color_t) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3)))

#define CLR_WHITE CLR_FROM_RGB(0xff, 0xff, 0xff)
#define CLR_BLACK CLR_FROM_RGB(0x00, 0x00, 0x00)

#define BTN_1 0x01
#define BTN_2 0x02
#define BTN_3 0x03
#define BTN_4 0x04
#define BTN_5 0x05

typedef uint16_t color_t;
typedef uint8_t button_t;

int  lcd_init(void);
void lcd_stop(void);

void lcd_setPixel(int x, int y, color_t clr);

void lcd_clear(color_t clr);

void lcd_blitSprite(color_t* sprite,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH);

void lcd_blitSpriteScaled(color_t* sprite,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY);

void lcd_blitSpritePalette(uint8_t* sprite, color_t* palette,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH);

void lcd_blitSpritePaletteScaled(uint8_t* sprite, color_t* palette,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY);

void lcd_blitTilesPalette(uint8_t* tilemap, color_t* palette,
    int tileW, int tileH, int tilesPerRow, uint16_t* tiles,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH);

void lcd_blitTilesPaletteScaled(uint8_t* tilemap, color_t* palette,
    int tileW, int tileH, int tilesPerRow, uint16_t* tiles,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY);

button_t lcd_getButtons(void);

#endif
