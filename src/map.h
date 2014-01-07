#ifndef _MAP_H_
#define _MAP_H_ 1

#include <stdint.h>

#define DEFAULT_TILE 0xffff

#define TFLAG_NONE 0x00
#define TFLAG_SOLID 0x01

#define DIR_NONE -1
#define DIR_T 0
#define DIR_L 1
#define DIR_B 2
#define DIR_R 3

struct tile {
	uint16_t back;
	uint16_t fore;
	uint8_t flags;
};

struct map {
	int width;
	int height;
	struct tile* tiles;
};

struct map map_new(int width, int height);

void map_clearTile(struct map map, int x, int y);

void map_setTileBackground(struct map map, int x, int y, uint16_t id);
void map_setTileForeground(struct map map, int x, int y, uint16_t id);
void map_setTileFlags(struct map map, int x, int y, uint8_t flags);
uint16_t map_getTileBackground(struct map map, int x, int y);
uint16_t map_getTileForeground(struct map map, int x, int y);
uint8_t map_getTileFlags(struct map map, int x, int y);
int map_hasTileBackground(struct map map, int x, int y);
int map_hasTileForeground(struct map map, int x, int y);

void map_genForest(struct map map, int x, int y, int width, int height);
void map_genDungeon(struct map map, int x, int y, int width, int height,
    void (*hollowFunc)(struct map, int, int, int, int, int),
    void (*solidFunc)(struct map, int, int, int, int));

#endif
