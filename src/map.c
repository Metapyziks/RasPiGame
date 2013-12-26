#include <stdlib.h>

#include "map.h"

void map_setTile(struct map map, int x, int y, uint16_t id, uint8_t flags)
{
	map.tiles[x + y * map.width].id = id;
	map.tiles[x + y * map.height].flags = flags;
}

struct tile map_getTile(struct map map, int x, int y)
{
	return map.tiles[x + y * map.height];
}

struct map map_new(int width, int height)
{
	struct map map = { width, height, NULL };

	map.tiles = (struct tile*) malloc(sizeof(struct tile) * width * height);

	for (int x = 0; x < width; ++x)
	for (int y = 0; y < height; ++y) {
		map_setTile(map, x, y, 0x000a, TFLAG_NONE);
	}

	return map;
}

#include "mapgen/forest.c"
