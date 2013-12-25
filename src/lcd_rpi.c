#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "lcd.h"

#define SET_PIXEL(x, y, clr) (lcd_fbp[(x) + (y) * DISPLAY_WIDTH] = clr)
#define SSD1289_GET_KEYS _IOR('K', 1, uint8_t *)

static struct fb_fix_screeninfo lcd_finfo;
static struct fb_var_screeninfo lcd_vinfo;

static long int lcd_screensize;

static int lcd_fbfd;
static uint16_t* lcd_fbp;

static void (*curIdleFunc)(void) = NULL;
static void (*curDisplayFunc)(void) = NULL;

void lcd_idleFunc(void (*idleFunc)(void))
{
    curIdleFunc = idleFunc;
}

void lcd_displayFunc(void (*displayFunc)(void))
{
    curDisplayFunc = displayFunc;
}

int lcd_init(void)
{
    lcd_fbfd = open("/dev/fb1", O_RDWR);

    if (lcd_fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return FALSE;
    }

    printf("The framebuffer device was opened successfully.\n");

    if (ioctl(lcd_fbfd, FBIOGET_FSCREENINFO, &lcd_finfo) == -1) {
        perror("Error reading fixed information");
        return FALSE;
    }

    if (ioctl(lcd_fbfd, FBIOGET_VSCREENINFO, &lcd_vinfo) == -1) {
        perror("Error reading variable information");
        return FALSE;
    }

    printf("%dx%d, %dbpp\n", lcd_vinfo.xres, lcd_vinfo.yres, lcd_vinfo.bits_per_pixel);

    lcd_screensize = lcd_vinfo.xres * lcd_vinfo.yres * (lcd_vinfo.bits_per_pixel >> 3);

    lcd_fbp = (uint16_t*) mmap(0, lcd_screensize,
        PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fbfd, 0);

    if ((int) lcd_fbp == -1) {
        perror("Error mapping framebuffer device to memory");
        return FALSE;
    }

    printf("The framebuffer device was mapped to memory successfully.\n");

    system("setterm -cursor off");

    return TRUE;
}

void lcd_mainLoop(void)
{
    do {
        curIdleFunc();
        curDisplayFunc();
    } while (!lcd_buttonDown(BTN_0 | BTN_3));
}

void lcd_stop(void)
{
    system("setterm -cursor on");

    munmap(lcd_fbp, lcd_screensize);
    close(lcd_fbfd);
}

#include "lcd.c"

button_t lcd_getButtons(void)
{
    button_t buttons;
 
    if (ioctl(lcd_fbfd, SSD1289_GET_KEYS, &buttons) == -1) {
        perror("_apps ioctl get");
    }

    return ~buttons;
}

int lcd_buttonDown(button_t button)
{
    button_t down = lcd_getButtons();

    return (down & button) == button;
}
