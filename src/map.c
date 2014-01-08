#include <stdlib.h>

#include "map.h"
#include "utils.h"

struct connector conn_open(int dir, int offset, int size)
{
    struct connector conn = {
        .type = CONN_OPEN,
        .dir = dir,
        .size = size,
        .x = (dir == DIR_T || dir == DIR_B) ? offset : 0,
        .y = (dir == DIR_L || dir == DIR_R) ? offset : 0
    };

    return conn;
}

struct connector conn_door(int x, int y, int dir, int size)
{
    struct connector conn = {
        .type = CONN_DOOR,
        .dir = dir,
        .size = size,
        .x = x,
        .y = y
    };

    return conn;
}

struct connector conn_vert(int x, int y, int up)
{
    struct connector conn = {
        .type = up ? CONN_VERT_UP : CONN_VERT_UP,
        .x = x,
        .y = y
    };

    return conn;
}

void map_clearTile(struct map map, int x, int y)
{
	map.tiles[x + y * map.width].back = DEFAULT_TILE;
	map.tiles[x + y * map.width].fore = DEFAULT_TILE;
    map.tiles[x + y * map.width].flags = TILE_NONE;
}

void map_setTileBackground(struct map map, int x, int y, uint16_t id)
{
	map.tiles[x + y * map.width].back = id;
}

void map_setTileForeground(struct map map, int x, int y, uint16_t id)
{
	map.tiles[x + y * map.width].fore = id;
}

void map_setTileFlags(struct map map, int x, int y, uint8_t flags)
{
	map.tiles[x + y * map.width].flags = flags;
}

uint16_t map_getTileBackground(struct map map, int x, int y)
{
	return map.tiles[x + y * map.width].back;
}

uint16_t map_getTileForeground(struct map map, int x, int y)
{
	return map.tiles[x + y * map.width].fore;
}

uint8_t map_getTileFlags(struct map map, int x, int y)
{
	return map.tiles[x + y * map.width].flags;
}

int map_hasTileBackground(struct map map, int x, int y)
{
	return map.tiles[x + y * map.width].back != DEFAULT_TILE;
}

int map_hasTileForeground(struct map map, int x, int y)
{
	return map.tiles[x + y * map.width].fore != DEFAULT_TILE;
}

struct map map_new(int width, int height)
{
	struct map map = { width, height, NULL };

	map.tiles = NEW(struct tile, width * height);

	for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y) {
		map_clearTile(map, x, y);
	}

	return map;
}

static int bleed(struct map map,
    int x, int y, int w, int h, int mask)
{
    int* valid = NEW(int, w * h);
    int count = 0;

    for (int i = x; i < x + w; ++i)
    for (int j = y; j < y + h; ++j)
    if (!map_hasTileBackground(map, i, j) && (
        map_getTileBackground(map, i - 1, j) == mask ||
        map_getTileBackground(map, i, j - 1) == mask ||
        map_getTileBackground(map, i + 1, j) == mask ||
        map_getTileBackground(map, i, j + 1) == mask)) {
        valid[count++] = i + j * map.width;
    }

    if (count == 0 || count < (w + h) / 2 && (rand() % count) == 0) return FALSE;

    int index = valid[rand() % count];
    map_setTileBackground(map, index % map.width, index / map.width, mask);

    return TRUE;
}

static void purgeMask(struct map map,
    int x, int y, int w, int h, int mask)
{
    for (int i = x; i < x + w; ++i)
    for (int j = y; j < y + h; ++j)
    if (map_getTileBackground(map, i, j) == mask) {
        map_clearTile(map, i, j);
    }
}

void map_generateExterior(struct map map,
    int x, int y, int w, int h,
    int connc, struct connector* connv,
    nonSolidFunc_t nonSolidFunc, solidFunc_t centreSolidFunc,
    solidFunc_t topSolidFunc, solidFunc_t leftSolidFunc,
    solidFunc_t bottomSolidFunc, solidFunc_t rightSolidFunc)
{
    nonSolidFunc(map, x, y, w, h, connc, connv);

    int nextMask = 0xfff0;

    int cMask = nextMask++;
    int tMask = (topSolidFunc == centreSolidFunc) ? cMask
        : nextMask++;
    int lMask = (leftSolidFunc == centreSolidFunc) ? cMask
        : (leftSolidFunc == topSolidFunc) ? tMask
        : nextMask++;
    int bMask = (bottomSolidFunc == centreSolidFunc) ? cMask
        : (bottomSolidFunc == topSolidFunc) ? tMask
        : (bottomSolidFunc == leftSolidFunc) ? lMask
        : nextMask++;
    int rMask = (rightSolidFunc == centreSolidFunc) ? cMask
        : (rightSolidFunc == topSolidFunc) ? tMask
        : (rightSolidFunc == leftSolidFunc) ? lMask
        : (rightSolidFunc == bottomSolidFunc) ? bMask
        : nextMask++;

    for (int i = x; i < x + w - 1; ++i) {
        if (!map_hasTileBackground(map, i, y)) {
            map_setTileBackground(map, i, y, tMask);
        }
        if (!map_hasTileBackground(map, i + 1, y + h - 1)) {
            map_setTileBackground(map, i + 1, y + h - 1, bMask);
        }
    }

    for (int i = y; i < y + h - 1; ++i) {
        if (!map_hasTileBackground(map, x, i + 1)) {
            map_setTileBackground(map, x, i + 1, lMask);
        }
        if (!map_hasTileBackground(map, x + w - 1, i)) {
            map_setTileBackground(map, x + w - 1, i, rMask);
        }
    }

    int tBleed = TRUE;
    int lBleed = TRUE;
    int bBleed = TRUE;
    int rBleed = TRUE;

    while (tBleed || lBleed || bBleed || rBleed) {
        if (tBleed && !bleed(map, x, y, w, 4, tMask)) tBleed = FALSE;
        if (lBleed && !bleed(map, x, y, 4, h, lMask)) lBleed = FALSE;
        if (bBleed && !bleed(map, x, y + h - 4, w, 4, bMask)) bBleed = FALSE;
        if (rBleed && !bleed(map, x + w - 4, y, 4, h, rMask)) rBleed = FALSE;
    }

    purgeMask(map, x, y, w, h, cMask);
    centreSolidFunc(map, x, y, w, h);
    purgeMask(map, x, y, w, h, tMask);
    topSolidFunc(map, x, y, w, h);
    purgeMask(map, x, y, w, h, lMask);
    leftSolidFunc(map, x, y, w, h);
    purgeMask(map, x, y, w, h, bMask);
    bottomSolidFunc(map, x, y, w, h);
    purgeMask(map, x, y, w, h, rMask);
    rightSolidFunc(map, x, y, w, h);
}

void map_genVoidSolid(struct map map,
    int x, int y, int w, int h)
{
    for (int r = y; r < y + h; ++r)
    for (int c = x; c < x + w; ++c) {
        if (!map_hasTileBackground(map, c, r)) {
            map_setTileFlags(map, c, r, TILE_SOLID);
            map_setTileBackground(map, c, r, 0x0007);
            map_setTileForeground(map, c, r, 0x00b3);
        }
    }
}
