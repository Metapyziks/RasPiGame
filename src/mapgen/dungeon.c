#include <stdlib.h>

#include "../map.h"
#include "../utils.h"

#define SPLIT_HORZ 0
#define SPLIT_VERT 1

#define MIN_DIV 8

struct div
{
    int split, offset;
    struct div* a;
    struct div* b;
};

static struct div* subdivide(struct map map, int x, int y, int width, int height)
{
    int offset; struct div* node;

    if (width < 2 * MIN_DIV && height < 2 * MIN_DIV) return NULL;

    if (height < 2 * MIN_DIV || (width >= 2 * MIN_DIV && width >= height)) {
        offset = MIN_DIV + rand() % (width - MIN_DIV * 2 + 1);
        node = NEW(struct div, 1);

        node->split = SPLIT_VERT;
        node->offset = offset;

        node->a = subdivide(map, x, y, offset, height);
        node->b = subdivide(map, x + offset, y, width - offset, height);
    } else {
        offset = MIN_DIV + rand() % (height - MIN_DIV * 2 + 1);
        node = NEW(struct div, 1);

        node->split = SPLIT_HORZ;
        node->offset = offset;

        node->a = subdivide(map, x, y, width, offset);
        node->b = subdivide(map, x, y + offset, width, height - offset);
    }

    return node;
}

static void carveNode(struct map map, int x, int y, int width, int height,
    struct div node, void (*hollowFunc)(struct map, int, int, int, int))
{
    int size = 2 + (rand() & 1) * 2;

    if (node.split == SPLIT_VERT) {
        hollowFunc(map, x + node.offset - size / 2, y, size, height);

        if (node.a != NULL) {
            carveNode(map, x, y, node.offset, height, *(node.a), hollowFunc);
        }
        if (node.b != NULL) {
            carveNode(map, x + node.offset, y, width - node.offset, height, *(node.b), hollowFunc);
        }
    } else {
        hollowFunc(map, x, y + node.offset - size / 2, width, size);

        if (node.a != NULL) {
            carveNode(map, x, y, width, node.offset, *(node.a), hollowFunc);
        }
        if (node.b != NULL) {
            carveNode(map, x, y + node.offset, width, height - node.offset, *(node.b), hollowFunc);
        }
    }
}

static void freeNode(struct div* node)
{
    if (node->a != NULL) freeNode(node->a);
    if (node->b != NULL) freeNode(node->b);

    free(node);
}

void map_genDungeon(struct map map, int x, int y, int width, int height,
    void (*hollowFunc)(struct map, int, int, int, int),
    void (*solidFunc)(struct map, int, int, int, int))
{
    struct div* node;

    node = subdivide(map, x + 8, y + 8, width - 16, height - 16);
    carveNode(map, x + 8, y + 8, width - 16, height - 16, *node, hollowFunc);
    solidFunc(map, x, y, width, height);

    freeNode(node);
}
