#ifndef _LCD_H_
#define _LCD_H_ 1

#include <stdint.h>

#include "utils.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define CLR_FROM_RGB(r, g, b) ((color_t) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3)))

#define CLR_WHITE CLR_FROM_RGB(0xff, 0xff, 0xff)
#define CLR_BLACK CLR_FROM_RGB(0x00, 0x00, 0x00)

#define BTN_0 (1<<0)
#define BTN_1 (1<<1)
#define BTN_2 (1<<2)
#define BTN_3 (1<<3)
#define BTN_4 (1<<4)

typedef uint16_t color_t;
typedef uint8_t button_t;

void lcd_idleFunc(void (*idleFunc)(void));
void lcd_displayFunc(void (*displayFunc)(void));

int lcd_init(void);
void lcd_mainLoop(void);
void lcd_stop(void);

void lcd_swapBuffers(void);

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
    uint16_t* tiles, int tileW, int tileH,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH);

void lcd_blitTilesPaletteScaled(uint8_t* tilemap, color_t* palette,
    uint16_t* tiles, int tileW, int tileH,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY);

button_t lcd_getButtons(void);

int lcd_buttonDown(button_t button);

#endif
