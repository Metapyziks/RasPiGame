#include "lcd.h"

#define SET_PIXEL(x, y, clr) (lcd_fbp[(x) + (y) * DISPLAY_WIDTH] = clr)
#define SSD1289_GET_KEYS _IOR('K', 1, uint8_t *)

static struct fb_fix_screeninfo lcd_finfo;
static struct fb_var_screeninfo lcd_vinfo;

static long int lcd_screensize;

static int lcd_fbfd;
static uint16_t* lcd_fbp;

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

void lcd_stop(void)
{
    system("setterm -cursor on");

    munmap(lcd_fbp, lcd_screensize);
    close(lcd_fbfd);
}

void lcd_setPixel(int x, int y, color_t clr)
{
    SET_PIXEL(x, y, clr);
}

void lcd_clear(color_t clr)
{
    for (int y = 0; y < DISPLAY_HEIGHT; ++y) for (int x = 0; x < DISPLAY_WIDTH; ++x) {
        SET_PIXEL(x, y, clr);
    }
}

void lcd_blitSprite(color_t* sprite,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    lcd_blitSpriteScaled(sprite,
        srcX, srcY, srcW, srcH,
        dstX, dstY, dstW, dstH,
        1, 1);
}

void lcd_blitSpriteScaled(color_t* sprite,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY)
{
    int x, y;

    for (int i = 0; i < dstW; ++i) for (int j = 0; j < dstH; ++j) {
        x = WRAP(srcX + i / scaleX, srcW);
        y = WRAP(srcY + j / scaleY, srcH);

        SET_PIXEL(dstX + i, dstY + j, sprite[x + y * srcW]);
    }
}

void lcd_blitSpritePalette(uint8_t* sprite, color_t* palette,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    lcd_blitSpritePaletteScaled(sprite, palette,
        srcX, srcY, srcW, srcH,
        dstX, dstY, dstW, dstH,
        1, 1);
}

void lcd_blitSpritePaletteScaled(uint8_t* sprite, color_t* palette,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY)
{
    int x, y;
    uint8_t index;

    for (int i = 0; i < dstW; ++i) for (int j = 0; j < dstH; ++j) {
        x = WRAP(srcX + i / scaleX, srcW);
        y = WRAP(srcY + j / scaleY, srcH);
        
        index = sprite[x + y * srcW];

        if (index != 0xff) {
            SET_PIXEL(dstX + i, dstY + j, palette[index]);
        }
    }
}

void lcd_blitTilesPalette(uint8_t* tilemap, color_t* palette,
    int tileW, int tileH, int tilesPerRow, uint16_t* tiles,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    lcd_blitTilesPaletteScaled(tilemap, palette,
        tileW, tileH, tilesPerRow, tiles,
        srcX, srcY, srcW, srcH,
        dstX, dstY, dstW, dstH,
        1, 1);
}

void lcd_blitTilesPaletteScaled(uint8_t* tilemap, color_t* palette,
    int tileW, int tileH, int tilesPerRow, uint16_t* tiles,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH,
    int scaleX, int scaleY)
{
    int srcCols = srcW / tileW;
    int srcRows = srcH / tileH;

    int minI = srcX;
    int minJ = srcY;
    int maxI = srcX + dstW / scaleX;
    int maxJ = srcY + dstH / scaleY;

    minI = FLOOR(minI, tileW);
    minJ = FLOOR(minJ, tileH);
    maxI = CEIL(maxI, tileW);
    maxJ = CEIL(maxJ, tileH);

    for (int j = minJ; j < maxJ; ++j) {
        int ty = WRAP(j, srcRows);
        int oy = ((j * tileH) - srcY) * scaleY;
        int py = dstH - (oy + tileH * scaleY);

        int minY = MAX(0, -oy);
        int maxY = tileH * scaleY + MIN(0, py);

        for (int i = minI; i < maxI; ++i) {
            int tx = WRAP(i, srcCols);
            int ox = ((i * tileW) - srcX) * scaleX;
            int px = dstW - (ox + tileW * scaleX);

            int minX = MAX(0, -ox);
            int maxX = tileW * scaleX + MIN(0, px);

            int tileIndex = tiles[tx + ty * srcCols];

            int tileX = (tileIndex % tilesPerRow) * tileW;
            int tileY = (tileIndex / tilesPerRow) * tileH;

            for (int dy = minY; dy < maxY; ++dy) {
                int sy = dstY + oy + dy;
                int tmY = tileY + dy / scaleY;

                for (int dx = minX; dx < maxX; ++dx) {
                    int sx = dstX + ox + dx;
                    int tmX = tileX + dx / scaleX;

                    uint8_t index = tilemap[tmX + tmY * tilesPerRow * tileW];

                    if (index != 0xff) {
                        SET_PIXEL(sx, sy, palette[index]);
                    }
                }
            }
        }
    }
}

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
