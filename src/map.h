#ifndef _MAP_H_
#define _MAP_H_ 1

#include <stdint.h>

#define DEFAULT_TILE 0xffff

#define TILE_NONE 0
#define TILE_SOLID 1

#define DIR_NONE -1
#define DIR_T 0
#define DIR_L 1
#define DIR_B 2
#define DIR_R 3

#define PATH_DEFAULT 0
#define PATH_CONN 1

#define CONN_OPEN 0
#define CONN_DOOR 1
#define CONN_VERT_UP 2
#define CONN_VERT_DN 3

typedef void(*carveFunc_t)(struct map map,
    int x, int y, int w, int h,
    int dir, int flags);

typedef void(*nonSolidFunc_t)(struct map map,
    int x, int y, int w, int h,
    int connc, struct connector* connv);

typedef void(*solidFunc_t)(struct map map,
    int x, int y, int w, int h);

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

struct vert
{
    int x, y, mark, id;
    struct vert* paths[4];
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

struct vert* map_genPaths(struct map map, int x, int y, int width, int height,
    int connc, struct connector* connv);

void map_freeVerts(struct vert* v);

void map_carvePath(struct map map,
    struct vert* a, struct vert* b,
    int size, int flags,
    carveFunc_t carveFunc);

void map_carveNetwork(struct map map,
    struct vert* v,
    carveFunc_t carveFunc);

void map_carveConnectors(struct map map,
    int x, int y, int w, int h,
    int connc, struct connector* connv,
    carveFunc_t carveFunc);

void map_generateExterior(struct map map,
    int x, int y, int w, int h,
    int connc, struct connector* connv,
    nonSolidFunc_t nonSolidFunc, solidFunc_t centreSolidFunc,
    solidFunc_t topSolidFunc, solidFunc_t leftSolidFunc,
    solidFunc_t bottomSolidFunc, solidFunc_t rightSolidFunc);

void map_genVoidSolid(struct map map,
    int x, int y, int w, int h);

void map_genForestCarve(struct map map,
    int x, int y, int w, int h,
    int dir, int flags);

void map_genForestNonSolid(struct map map,
    int x, int y, int w, int h, 
    int connc, struct connector* connv);

void map_genForestSolid(struct map map,
    int x, int y, int w, int h);

#endif
