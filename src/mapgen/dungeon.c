#include <stdlib.h>

#include "../map.h"
#include "../utils.h"

#define MIN_DIV 8

#define CARVE_MARK -1

#define DIR_T 0
#define DIR_L 1
#define DIR_B 2
#define DIR_R 3

#define ADJ(v, d) ((v)->paths[(d)])

static int vertCount = 0;

struct vert
{
    int x, y, mark, id;
    struct vert* paths[4];
};

struct path
{
    struct vert* a;
    struct vert* b;
};

static int joinVerts(struct vert* a, struct vert* b)
{
    if ((a->x != b->x) == (a->y != b->y)) return FALSE;

    int dir = a->y > b->y ? DIR_T
            : a->x > b->x ? DIR_L
            : a->y < b->y ? DIR_B
            : DIR_R;

    struct vert* prev = ADJ(b, dir ^ 2);
    if (prev != NULL) {
        if ((dir == DIR_T && prev->y < a->y) ||
            (dir == DIR_L && prev->x < a->x) ||
            (dir == DIR_B && prev->y > a->y) ||
            (dir == DIR_R && prev->x > a->x)) {
            return joinVerts(a, prev);
        } else if ((prev->x != a->x) != (prev->y != a->y)) {
            printf("- (%i, %i) \t(%i, %i)\n", prev->x, prev->y, a->x, a->y);
            ADJ(prev, dir) = NULL;
        } else {
            return FALSE;
        }
    }

    printf("+ (%i, %i) \t(%i, %i)\n", a->x, a->y, b->x, b->y);
    ADJ(a, dir) = b; ADJ(b, dir ^ 2) = a;

    return TRUE;
}

static struct vert* findVert(struct vert* v, int dir, int pos)
{
    if (v == NULL) return NULL;

    if (dir & 1) {
        if (v->x == pos) return v;
        if ( (dir & 2) && v->x > pos) return NULL;
        if (!(dir & 2) && v->x < pos) return NULL;
    } else {
        if (v->y == pos) return v;
        if ( (dir & 2) && v->y > pos) return NULL;
        if (!(dir & 2) && v->y < pos) return NULL;
    }

    return findVert(ADJ(v, dir), dir, pos);
}

static struct vert* vert_new(int x, int y)
{
    struct vert* v = NEW(struct vert, 1);

    v->x = x;
    v->y = y;
    v->id = vertCount++;
    v->mark = 0;

    for (int i = 0; i < 4; ++i) ADJ(v, i) = NULL;

    return v;
}

static struct vert* quad(int x, int y, int w, int h)
{
    struct vert* tl = vert_new(x, y);
    struct vert* tr = vert_new(x + w, y);
    struct vert* bl = vert_new(x, y + h);
    struct vert* br = vert_new(x + w, y + h);

    joinVerts(tl, tr);
    joinVerts(tl, bl);
    joinVerts(tr, br);
    joinVerts(bl, br);

    return tl;
}

static void subdivide(struct vert* tl)
{
    // Find top-right and bottom-left of quad to subdivide.
    struct vert* tr = ADJ(tl, DIR_R);
    struct vert* bl = ADJ(tl, DIR_B);

    if (tr == NULL || bl == NULL) return;

    // The top-right / bottom-left may not be the first adjacent verts.
    while (ADJ(tr, DIR_B) == NULL) tr = ADJ(tr, DIR_R);
    while (ADJ(tl, DIR_R) == NULL) bl = ADJ(bl, DIR_B);

    // Bottom right should always be the vert below the top right.
    struct vert* br = ADJ(tr, DIR_B);

    // Bounding coordinates of the quad.
    int x0 = tl->x;
    int x1 = br->x;
    int y0 = tl->y;
    int y1 = br->y;

    // Size of the quad.
    int w = x1 - x0;
    int h = y1 - y0;
    
    int offset;

    struct vert* a = NULL;
    struct vert* b = NULL;

    // If the quad is too small don't subdivide.
    if (w < 2 * MIN_DIV && h < 2 * MIN_DIV) return;

    // If the quad is wider than it is tall, subdivide vertically.
    if (w >= 2 * MIN_DIV && (w >= h || h < 2 * MIN_DIV)) {

        // Choose where to subdivide.
        offset = MIN_DIV + rand() % (w - MIN_DIV * 2 + 1);

        // Look for existing verts on the top and bottom rows
        // that are along the subdivision line.
        a = findVert(tl, DIR_R, x0 + offset);
        b = findVert(bl, DIR_R, x0 + offset);

        // If no vert on the top row exists along the
        // subdivision line, create one.
        if (a == NULL) {
            a = vert_new(x0 + offset, y0);

            joinVerts(a, tl);
            joinVerts(a, tr);
        }

        // If no vert on the bottom row exists along the
        // subdivision line, create one.
        if (b == NULL) {
            b = vert_new(x0 + offset, y1);

            joinVerts(b, bl);
            joinVerts(b, br);
        }

    // Otherwise, subdivide horizontally.
    } else {

        // Choose where to subdivide.
        offset = MIN_DIV + rand() % (h - MIN_DIV * 2 + 1);

        // Look for existing verts on the left and right
        // columns that are along the subdivision line.
        a = findVert(tl, DIR_B, y0 + offset);
        b = findVert(tr, DIR_B, y0 + offset);

        // If no vert on the left column exists along the
        // subdivision line, create one.
        if (a == NULL) {
            a = vert_new(x0, y0 + offset);

            joinVerts(a, bl);
            joinVerts(a, tl);
        }

        // If no vert on the right column exists along the
        // subdivision line, create one.
        if (b == NULL) {
            b = vert_new(x1, y0 + offset);

            joinVerts(b, br);
            joinVerts(b, tr);
        }
    }

    // Join the two verts that are along the subdivision line.
    joinVerts(a, b);

    // Subdivide the two new quads.
    subdivide(tl); subdivide(a);
}

static void fillVertsArray(struct vert** verts, struct vert* v)
{
    verts[v->id] = v;

    for (int i = 0; i < 4; ++ i) {
        struct vert* next = ADJ(v, i);
        if (next != NULL && verts[next->id] == NULL) fillVertsArray(verts, next);
    }
}

static struct vert** findVerts(struct vert* v)
{
    struct vert** verts = NEW(struct vert*, vertCount);

    for (int i = 0; i < vertCount; ++ i) verts[i] = NULL;

    fillVertsArray(verts, v);

    return verts;
}

static void fillPathsArray(struct path* paths, struct vert* v);

static void addPathToArray(struct path* paths, struct vert* a, struct vert* b)
{
    struct vert* next = b;

    if (b->id < a->id) {
        b = a; a = next;
    }

    int i;
    for (i = 0; i < vertCount * 4 && paths[i].a != NULL; ++i) {
        if (paths[i].a == a && paths[i].b == b) return;
    }

    paths[i].a = a; paths[i].b = b;
    
    fillPathsArray(paths, next);
}

static void fillPathsArray(struct path* paths, struct vert* v)
{
    for (int i = 0; i < 4; ++i) {
        struct vert* next = ADJ(v, i);
        if (next != NULL) addPathToArray(paths, v, next);
    }
}

static struct path* findPaths(struct vert** verts)
{
    struct path* paths = NEW(struct path, vertCount * 4);

    for (int i = 0; i < vertCount * 4; ++ i) {
        paths[i].a = NULL; paths[i].b = NULL;
    }

    fillPathsArray(paths, verts[0]);

    return paths;
}

static int countPaths(struct path* paths) {
    int count;
    for (count = 0; count < vertCount * 4 && paths[count].a != NULL; ++count);
    return count;
}

static void removePath(struct path* paths, struct path path)
{
    for (int i = 0; i < 4; ++i) {
        if (ADJ(path.a, i) == path.b) {
            ADJ(path.a, i) = NULL;
            ADJ(path.b, i ^ 2) = NULL;
            break;
        }
    }

    int found = FALSE;
    for (int i = 0; i < vertCount * 4; ++i) {
        if (!found && paths[i].a == path.a && paths[i].b == path.b) found = TRUE;
        if (found) paths[i] = paths[i + 1];
        if (paths[i].a == NULL) break;
    }
}

static void markNetwork(struct vert** verts, int value)
{
    for (int i = 0; i < vertCount; ++i) verts[i]->mark = value;
}

static void floodNetwork(struct vert* v, int value)
{
    v->mark = value;

    for (int i = 0; i < 4; ++i) {
        struct vert* next = ADJ(v, i);
        if (next != NULL && next->mark != value) floodNetwork(next, value);
    }
}

static void cullPaths(struct vert* v)
{
    struct vert** verts = findVerts(v);
    struct path* paths = findPaths(verts);

    int pathCount = countPaths(paths);
    int currCount = pathCount;
    int destCount = vertCount;

    markNetwork(verts, 0);

    while (pathCount > destCount && currCount > 0) {
        int index = rand() % currCount--;
        struct path path = paths[index];

        removePath(paths, path);
        floodNetwork(path.a, 1 - path.a->mark);

        if (path.b->mark != path.a->mark) {
            floodNetwork(path.b, path.a->mark);
            joinVerts(path.a, path.b);
        } else {
            pathCount--;
        }
    }

    markNetwork(verts, 0);

    free(verts);
    free(paths);
}

static void carvePath(struct map map, struct vert* a, struct vert* b,
    void (*hollowFunc)(struct map, int, int, int, int))
{
    int size = 2 + (rand() & 1) * 2;

    int x0 = MIN(a->x, b->x) - size / 2;
    int y0 = MIN(a->y, b->y) - size / 2;
    int x1 = MAX(a->x, b->x) + size / 2;
    int y1 = MAX(a->y, b->y) + size / 2;

    hollowFunc(map, x0, y0, x1 - x0, y1 - y0);
}

static void carveNetwork(struct map map, struct vert* v,
    void (*hollowFunc)(struct map, int, int, int, int))
{
    v->mark = CARVE_MARK;

    for (int i = 0; i < 4; ++i) {
        struct vert* next = ADJ(v, i);
        if (next != NULL && next->mark != CARVE_MARK) carvePath(map, v, next, hollowFunc);
    }

    for (int i = 0; i < 4; ++i) {
        struct vert* next = ADJ(v, i);
        if (next != NULL && next->mark != CARVE_MARK) carveNetwork(map, next, hollowFunc);
    }
}

static void freeVerts(struct vert* v)
{
    struct vert** verts = findVerts(v);

    for (int i = 0; i < vertCount; ++i) free(verts[i]);

    free(verts);

    vertCount = 0;
}

void map_genDungeon(struct map map, int x, int y, int w, int h,
    void (*hollowFunc)(struct map, int, int, int, int),
    void (*solidFunc)(struct map, int, int, int, int))
{
    struct vert* tl = quad(x + 8, y + 8, w - 16, h - 16);

    subdivide(tl);
    //cullPaths(tl);
    carveNetwork(map, tl, hollowFunc);
    solidFunc(map, x, y, w, h);

    freeVerts(tl);
}
