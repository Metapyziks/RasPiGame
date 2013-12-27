#include <stdlib.h>

#include "map.h"

void map_clearTile(struct map map, int x, int y)
{
	map.tiles[x + y * map.width].back = DEFAULT_TILE;
	map.tiles[x + y * map.width].fore = DEFAULT_TILE;
	map.tiles[x + y * map.width].flags = TFLAG_NONE;
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

	map.tiles = (struct tile*) malloc(sizeof(struct tile) * width * height);

	for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y) {
		map_clearTile(map, x, y);
	}

	return map;
}

#include "mapgen/forest.c"
