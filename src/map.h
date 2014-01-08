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

#define CONN_OPEN 0
#define CONN_DOOR 1
#define CONN_VERT_UP 2
#define CONN_VERT_DN 3

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

struct connector {
    int x, y;
    int type, dir, size;
};

struct connector conn_open(int dir, int offset, int size);
struct connector conn_door(int x, int y, int dir, int size);
struct connector conn_vert(int x, int y, int up);

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

void map_genForest(struct map map, int x, int y, int width, int height,
    int connc, struct connector* connv);

void map_genDungeon(struct map map, int x, int y, int width, int height,
    int connc, struct connector* connv,
    void(*hollowFunc)(struct map, int, int, int, int, int),
    void(*solidFunc)(struct map, int, int, int, int));

#endif
