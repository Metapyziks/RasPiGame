#include <stdlib.h>

#include "../map.h"
#include "../utils.h"

static void placeTree(struct map map, int x, int y)
{
    map_setTileBackground(map, x, y, 0x0088);
    map_setTileFlags(map, x, y, TILE_SOLID);
}

static void placeStump(struct map map, int x, int y)
{
    map_setTileBackground(map, x, y, 0x0089);
    map_setTileFlags(map, x, y, TILE_SOLID);
}

static void placeBigTree(struct map map, int x, int y)
{
    map_setTileBackground(map, x + 0, y + 0, 0x00e1);
    map_setTileBackground(map, x + 1, y + 0, 0x00e2);
    map_setTileBackground(map, x + 0, y + 1, 0x00ff);
    map_setTileBackground(map, x + 1, y + 1, 0x0100);

    map_setTileFlags(map, x + 0, y + 0, TILE_SOLID);
    map_setTileFlags(map, x + 1, y + 0, TILE_SOLID);
    map_setTileFlags(map, x + 0, y + 1, TILE_SOLID);
    map_setTileFlags(map, x + 1, y + 1, TILE_SOLID);
}

static void placeGrass(struct map map, int x, int y, int width, int height, int dir, int flags)
{
    int grass = !(flags & PATH_CONN) && (rand() & 3);

    for (int r = MAX(0, y - 1); r < MIN(map.height, y + height + 1); ++r)
    for (int c = MAX(0, x - 1); c < MIN(map.width, x + width + 1); ++c) {
        if (map_getTileFlags(map, c, r) != TILE_NONE) continue;

        int isHedge = ((dir == DIR_L || dir == DIR_R) && c == x + width / 2)
            || (dir == DIR_B && r == y + height / 2);

        if (r == y - 1 || c == x - 1 || r == y + height || c == x + width) {
            if ((r == y - 1 || r == y + height) == (c == x - 1 || c == x + width)) continue;
            if (isHedge) { placeTree(map, c, r); continue; }
            if ((rand() & 0x3) < 0x3) continue;
        }

        map_setTileFlags(map, c, r, TILE_NONE);

        if ((rand() & 0xff) < 0x7) {
            map_setTileForeground(map, c, r, 0x00ec);
        }

        if (!isHedge && (rand() & 3) < grass) {
            map_setTileBackground(map, c, r, 0x0010);
        } else {
            switch (rand() & 0x3) {
            case 0x0:
            case 0x1:
                map_setTileBackground(map, c, r, 0x000f); break;
            case 0x2:
                map_setTileBackground(map, c, r, 0x0069); break;
            case 0x3:
                map_setTileBackground(map, c, r, 0x006a); break;
            }

            if (isHedge) {
                switch (dir) {
                case DIR_L:
                    map_setTileForeground(map, c, r, 0x17a); break;
                case DIR_B:
                    map_setTileForeground(map, c, r, 0x199); break;
                case DIR_R:
                    map_setTileForeground(map, c, r, 0x17c); break;
                }
            }
        }
    }
}

static void placeTrees(struct map map, int x, int y, int width, int height)
{
    for (int r = y; r < y + height - 1; ++r)
    for (int c = x; c < x + width - 1; ++c) {
        if (map_hasTileBackground(map, c + 0, r + 0) ||
            map_hasTileBackground(map, c + 1, r + 0) ||
            map_hasTileBackground(map, c + 1, r + 1) ||
            map_hasTileBackground(map, c + 0, r + 1)) {
            continue;
        }

        int i = 1;

        int down = r < y + height - 2 &&
            !map_hasTileBackground(map, c + 0, r + 2) &&
            !map_hasTileBackground(map, c + 1, r + 2) ? i++ : 0;

        int right = c < x + width - 2 &&
            !map_hasTileBackground(map, c + 2, r + 0) &&
            !map_hasTileBackground(map, c + 2, r + 1) ? i++ : 0;

        int downRight = r < y + height - 2 && c < x + width - 2 &&
            !map_hasTileBackground(map, c + 1, r + 1) &&
            !map_hasTileBackground(map, c + 2, r + 1) &&
            !map_hasTileBackground(map, c + 2, r + 2) &&
            !map_hasTileBackground(map, c + 1, r + 2) ? i++ : 0;

        int select = rand() % i;

        if (select == 0) {
            placeBigTree(map, c + 0, r + 0);
        } else if (select == down) {
            placeBigTree(map, c + 0, r + 1);
        } else if (select == right) {
            placeBigTree(map, c + 1, r + 0);
        } else if (select == downRight) {
            placeBigTree(map, c + 1, r + 1);
        }
    }

    for (int r = y; r < y + height; ++r)
    for (int c = x; c < x + width; ++c) {
        if (!map_hasTileBackground(map, c, r)) {
            if ((rand() & 0xff) < 0xe7) {
                placeTree(map, c, r);
            } else {
                placeStump(map, c, r);
            }
        }
    }
}

void map_genForest(struct map map, int x, int y, int w, int h,
    int connc, struct connector* connv)
{
    struct vert* v = map_genPaths(map, x, y, w, h, connc, connv);

    map_carveNetwork(map, v, placeGrass);
    map_carveConnectors(map, x, y, w, h, connc, connv, placeGrass);
    map_freeVerts(v);
    
    placeTrees(map, x, y, w, h);
}
