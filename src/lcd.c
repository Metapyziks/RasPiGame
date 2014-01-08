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
    struct map map, int tileW, int tileH,
    int srcX, int srcY, int dstX, int dstY,
    int dstW, int dstH)
{
    lcd_blitTilesPaletteScaled(tilemap, palette,
        map, tileW, tileH, srcX, srcY, 
        dstX, dstY, dstW, dstH, 1, 1);
}

void lcd_blitTilesPaletteScaled(uint8_t* tilemap, color_t* palette,
    struct map map, int tileW, int tileH,
    int srcX, int srcY, int dstX, int dstY,
    int dstW, int dstH, int scaleX, int scaleY)
{
    int minI = srcX;
    int minJ = srcY;
    int maxI = srcX + dstW / scaleX;
    int maxJ = srcY + dstH / scaleY;

    minI = FLOOR(minI, tileW);
    minJ = FLOOR(minJ, tileH);
    maxI = CEIL(maxI, tileW);
    maxJ = CEIL(maxJ, tileH);

    minI = MAX(minI, 0); maxI = MIN(maxI, map.width);
    minJ = MAX(minJ, 0); maxJ = MIN(maxJ, map.height);    

    for (int ty = minJ; ty < maxJ; ++ty) {
        int oy = ((ty * tileH) - srcY) * scaleY;
        int py = dstH - (oy + tileH * scaleY);

        int minY = MAX(0, -oy);
        int maxY = tileH * scaleY + MIN(0, py);

        for (int tx = minI; tx < maxI; ++tx) {
            int ox = ((tx * tileW) - srcX) * scaleX;
            int px = dstW - (ox + tileW * scaleX);

            int minX = MAX(0, -ox);
            int maxX = tileW * scaleX + MIN(0, px);

            struct tile tile = map.tiles[tx + ty * map.width];

            if (tile.back == DEFAULT_TILE) continue;

            int bgOffset = tile.back * tileW * tileH;
            int fgOffset = tile.fore * tileW * tileH;

            for (int dy = minY; dy < maxY; ++dy) {
                int sy = dstY + oy + dy;

                for (int dx = minX; dx < maxX; ++dx) {
                    int sx = dstX + ox + dx;

                    uint8_t bg = tilemap[bgOffset + dx / scaleX + dy / scaleY * tileW];
                    uint8_t fg = tile.fore != DEFAULT_TILE
                        ? tilemap[fgOffset + dx / scaleX + dy / scaleY * tileW]
                        : 0xff;

                    if (fg != 0xff) {
                        SET_PIXEL(sx, sy, palette[fg]);
                    } else if (bg != 0xff) {
                        SET_PIXEL(sx, sy, palette[bg]);
                    }
                }
            }
        }
    }
}
