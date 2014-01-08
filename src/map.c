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
