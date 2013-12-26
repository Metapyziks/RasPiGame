#ifndef _MAP_H_
#define _MAP_H_ 1

#include <stdint.h>

#define TFLAG_NONE 0x00
#define TFLAG_SOLID 0x01

struct tile {
	uint16_t id;
	uint8_t flags;
};

struct map {
	int width;
	int height;
	struct tile* tiles;
};

struct map map_new(int width, int height);

void map_setTile(struct map map, int x, int y, uint16_t id, uint8_t flags);
struct tile map_getTile(struct map map, int x, int y);

void map_genForest(struct map map, int x, int y, int width, int height);

#endif
