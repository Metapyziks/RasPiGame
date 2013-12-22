#ifndef _LCD_H_
#define _LCD_H_ 1

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "utils.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define clr_fromRGB(r, g, b) ((color_t) (((r & 0xF8) << 8) | ((g & 0xF8) << 3) | ((b & 0xF8) >> 3)))

#define CLR_WHITE clrFromRGB(0xff, 0xff, 0xff)
#define CLR_BLACK clrFromRGB(0x00, 0x00, 0x00)
#define CLR_NONE 0x0020

typedef unsigned short int color_t;

int  lcd_init(void);
void lcd_stop(void);

void lcd_setPixel(int x, int y, color_t clr);

void lcd_clear(color_t clr);

#endif
