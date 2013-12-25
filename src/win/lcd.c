#include <string.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "../lcd.h"

void lcd_idleFunc(void (*idleFunc)(void))
{
    glutIdleFunc(idleFunc);
}

void lcd_displayFunc(void (*displayFunc)(void))
{
    glutDisplayFunc(displayFunc);
}

int lcd_init(void)
{
    int argc = 0;
    char** argv = NULL;

    glutInit(&argc, argv);
    glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("RasPiGame");

    glEnable(GL_DEPTH_TEST);

    return TRUE;
}

void lcd_mainLoop(void)
{
    glutMainLoop();
}

void lcd_stop(void)
{
    return;
}

void lcd_swapBuffers(void)
{
    GLenum ec = glGetError();
    if (ec != GL_NO_ERROR) {
        printf("Some kind of error or something: %s\n", gluErrorString(ec));
    }

    glutSwapBuffers();
}

void lcd_setPixel(int x, int y, color_t clr)
{
    glColor3f((clr & 0xf800) / (float) 0xf800, (clr & 0x7e0) / (float) 0x7e0, (clr & 0x1f) / (float) 0x1f);
    glRectf(x / (float) DISPLAY_WIDTH, y / (float) DISPLAY_HEIGHT,
        (x + 1) / (float) DISPLAY_WIDTH, (y + 1) / (float) DISPLAY_HEIGHT);
}

void lcd_clear(color_t clr)
{
    glClearColor((clr & 0xf800) / (float) 0xf800, (clr & 0x7e0) / (float) 0x7e0, (clr & 0x1f) / (float) 0x1f, 1.f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        lcd_setPixel(dstX + i, dstY + j, sprite[x + y * srcW]);
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
            lcd_setPixel(dstX + i, dstY + j, palette[index]);
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
                        lcd_setPixel(sx, sy, palette[index]);
                    }
                }
            }
        }
    }
}

button_t lcd_getButtons(void)
{
    return 0;
}

int lcd_buttonDown(button_t button)
{
    return FALSE;
}
