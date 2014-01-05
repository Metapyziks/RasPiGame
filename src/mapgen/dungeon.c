#include <stdlib.h>

#include "../map.h"
#include "../utils.h"

#define MIN_DIV 8

#define SPAN_MARK 1
#define CARVE_MARK -1

static int vertCount = 0;

struct vert
{
    int x, y, mark, id;
    struct vert* t;
    struct vert* l;
    struct vert* b;
    struct vert* r;
};

struct path
{
    struct vert* a;
    struct vert* b;
};

static struct vert* vert_new(int x, int y)
{
    struct vert* v = NEW(struct vert, 1);

    v->x = x;
    v->y = y;
    v->id = vertCount++;
    v->mark = 0;

    v->t = NULL;
    v->l = NULL;
    v->b = NULL;
    v->r = NULL;

    return v;
}

static struct vert* quad(int x, int y, int w, int h)
{
    struct vert* tl = vert_new(x, y);
    struct vert* tr = vert_new(x + w, y);
    struct vert* bl = vert_new(x, y + h);
    struct vert* br = vert_new(x + w, y + h);

    bl->t = tl; br->t = tr;
    tr->l = tl; br->l = bl;
    tl->b = bl; tr->b = br;
    tl->r = tr; bl->r = br;

    return tl;
}

static void subdivide(struct vert* tl)
{
    struct vert* tr = tl->r;
    struct vert* bl = tl->b;

    while (tr->b == NULL) tr = tr->r;
    while (bl->r == NULL) bl = bl->b;

    struct vert* br = tr->b;

    int x0 = tl->x;
    int x1 = br->x;
    int y0 = tl->y;
    int y1 = br->y;

    int w = x1 - x0;
    int h = y1 - y0;
    
    int offset;

    struct vert* a = NULL;
    struct vert* b = NULL;

    if (w < 2 * MIN_DIV && h < 2 * MIN_DIV) return;

    if (w >= 2 * MIN_DIV && (w >= h || h < 2 * MIN_DIV)) {
        offset = MIN_DIV + rand() % (w - MIN_DIV * 2 + 1);

        a = tl;
        b = bl;

        while ((a = a->r) != NULL) {
            if (a->x == x0 + offset) break;
            if (a->x > x0 + offset) {
                a = NULL; break;
            }
        }

        while ((b = b->r) != NULL) {
            if (b->x == x0 + offset) break;
            if (b->x > x0 + offset) {
                b = NULL;
                break;
            }
        }

        if (a == NULL) {
            a = vert_new(x0 + offset, y0);

            a->l = tr;
            a->r = tl;

            while (a->l->x > a->x) a->l = a->l->l; while (a->r->x < a->x) a->r = a->r->r;

            a->l->r = a; a->r->l = a;
        }

        if (b == NULL) {
            b = vert_new(x0 + offset, y1);

            b->l = br;
            b->r = bl;

            while (b->l->x > b->x) b->l = b->l->l; while (b->r->x < b->x) b->r = b->r->r;

            b->l->r = b; b->r->l = b;
        }

        a->b = b;
        b->t = a;

        subdivide(tl); subdivide(a);
    } else {
        offset = MIN_DIV + rand() % (h - MIN_DIV * 2 + 1);

        a = tl;
        b = tr;

        while ((a = a->b) != NULL) {
            if (a->y == y0 + offset) break;
            if (a->y > y0 + offset) {
                a = NULL; break;
            }
        }

        while ((b = b->b) != NULL) {
            if (b->y == y0 + offset) break;
            if (b->y > y0 + offset) {
                b = NULL; break;
            }
        }

        if (a == NULL) {
            a = vert_new(x0, y0 + offset);

            a->t = bl;
            a->b = tl;

            while (a->t->y > a->y) a->t = a->t->t; while (a->b->y < a->y) a->b = a->b->b;

            a->t->b = a; a->b->t = a;
        }

        if (b == NULL) {
            b = vert_new(x1, y0 + offset);

            b->t = br;
            b->b = tr;

            while (b->t->y > b->y) b->t = b->t->t; while (b->b->y < b->y) b->b = b->b->b;

            b->t->b = b; b->b->t = b;
        }

        a->r = b; 
        b->l = a;

        subdivide(tl); subdivide(a);
    }
}

static void fillVertsArray(struct vert** verts, struct vert* v)
{
    verts[v->id] = v;

    if (v->t != NULL && verts[v->t->id] == NULL) fillVertsArray(verts, v->t);
    if (v->l != NULL && verts[v->l->id] == NULL) fillVertsArray(verts, v->l);
    if (v->b != NULL && verts[v->b->id] == NULL) fillVertsArray(verts, v->b);
    if (v->r != NULL && verts[v->r->id] == NULL) fillVertsArray(verts, v->r);
}

static struct vert** findVerts(struct vert* v)
{
    struct vert** verts = NEW(struct vert*, vertCount);

    for (int i = 0; i < vertCount; ++ i) verts[i] = NULL;

    fillVertsArray(verts, v);

    return verts;
}

/*
static int countPaths(struct vert** verts)
{
    int sum = 0;
    for (int i = 0; i < vertCount; ++i) {
        if (verts[i]->t != NULL) ++sum;
        if (verts[i]->l != NULL) ++sum;
        if (verts[i]->b != NULL) ++sum;
        if (verts[i]->r != NULL) ++sum;
    }

    return sum / 2;
}
*/

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
    if (v->t != NULL) addPathToArray(paths, v, v->t);
    if (v->l != NULL) addPathToArray(paths, v, v->l);
    if (v->b != NULL) addPathToArray(paths, v, v->b);
    if (v->r != NULL) addPathToArray(paths, v, v->r);
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
    if (path.a->t == path.b) { path.a->t = NULL; path.b->b = NULL; } else 
    if (path.a->l == path.b) { path.a->l = NULL; path.b->r = NULL; } else 
    if (path.a->b == path.b) { path.a->b = NULL; path.b->t = NULL; } else 
    if (path.a->r == path.b) { path.a->r = NULL; path.b->l = NULL; }

    int found = FALSE;
    for (int i = 0; i < vertCount * 4; ++i) {
        if (!found && paths[i].a == path.a && paths[i].b == path.b) found = TRUE;
        if (found) paths[i] = paths[i + 1];
        if (paths[i].a == NULL) break;
    }
}

static void restorePath(struct path path)
{
    if (path.a->y > path.b->y) { path.a->t = path.b; path.b->b = path.a; } else
    if (path.a->x > path.b->x) { path.a->l = path.b; path.b->r = path.a; } else
    if (path.a->y < path.b->y) { path.a->b = path.b; path.b->t = path.a; } else
    if (path.a->x < path.b->x) { path.a->r = path.b; path.b->l = path.a; }
}

static void markNetwork(struct vert** verts, int value)
{
    for (int i = 0; i < vertCount; ++i) verts[i]->mark = value;
}

static void floodNetwork(struct vert* v, int value)
{
    v->mark = value;

    if (v->t != NULL && v->t->mark != value) floodNetwork(v->t, value);
    if (v->l != NULL && v->l->mark != value) floodNetwork(v->l, value);
    if (v->b != NULL && v->b->mark != value) floodNetwork(v->b, value);
    if (v->r != NULL && v->r->mark != value) floodNetwork(v->r, value);
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
            restorePath(path);
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

    if (v->t != NULL && v->t->mark != CARVE_MARK) carvePath(map, v, v->t, hollowFunc);
    if (v->l != NULL && v->l->mark != CARVE_MARK) carvePath(map, v, v->l, hollowFunc);
    if (v->b != NULL && v->b->mark != CARVE_MARK) carvePath(map, v, v->b, hollowFunc);
    if (v->r != NULL && v->r->mark != CARVE_MARK) carvePath(map, v, v->r, hollowFunc);

    if (v->t != NULL && v->t->mark != CARVE_MARK) carveNetwork(map, v->t, hollowFunc);
    if (v->l != NULL && v->l->mark != CARVE_MARK) carveNetwork(map, v->l, hollowFunc);
    if (v->b != NULL && v->b->mark != CARVE_MARK) carveNetwork(map, v->b, hollowFunc);
    if (v->r != NULL && v->r->mark != CARVE_MARK) carveNetwork(map, v->r, hollowFunc);
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
    cullPaths(tl);
    carveNetwork(map, tl, hollowFunc);
    solidFunc(map, x, y, w, h);

    freeVerts(tl);
}
