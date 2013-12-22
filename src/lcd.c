#include "lcd.h"

#define setPixel(x, y, clr) (lcd_fbp[((x + lcd_vinfo.xoffset) << 1) + (y + lcd_vinfo.yoffset) * lcd_finfo.line_length] = clr)

static struct fb_fix_screeninfo lcd_finfo;
static struct fb_var_screeninfo lcd_vinfo;

static long int lcd_screensize;

static int lcd_fbfd;
static unsigned short int* lcd_fbp;

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

    lcd_fbp = (unsigned short int*) mmap(0, lcd_screensize,
        PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fbfd, 0);

    if ((int) lcd_fbp == -1) {
        perror("Error mapping framebuffer device to memory");
        return FALSE;
    }

    printf("The framebuffer device was mapped to memory successfully.\n");

    system("setterm -cursor off");

    return TRUE;
}

void lcd_stop(void)
{
    system("setterm -cursor on");

    munmap(lcd_fbp, lcd_screensize);
    close(lcd_fbfd);
}

void lcd_setPixel(int x, int y, color_t clr)
{
    if (clr == CLR_NONE) return;

    setPixel(x, y, clr);
}

void lcd_clear(color_t clr)
{
    int x, y;

    if (clr == CLR_NONE) return;

    for (y = 0; y < DISPLAY_HEIGHT; ++y) for (x = 0; x < DISPLAY_WIDTH; ++x) {
        setPixel(x, y, clr);
    }
}
