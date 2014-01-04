#include <stdlib.h>

#include "../map.h"
#include "../utils.h"

#define SPLIT_HORZ 0
#define SPLIT_VERT 1

#define MIN_DIV 8

static int nextVertID = 0;

struct vert
{
    int x, y, mark, id;
    struct vert* t;
    struct vert* l;
    struct vert* b;
    struct vert* r;
};

static struct vert* vert_new(int x, int y)
{
    struct vert* v = NEW(struct vert, 1);

    v->x = x;
    v->y = y;
    v->id = nextVertID++;
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

static void cullPaths(struct map map, struct vert* v)
{

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
    v->mark = -1;

    if (v->t != NULL && v->t->mark != -1) carvePath(map, v, v->t, hollowFunc);
    if (v->l != NULL && v->l->mark != -1) carvePath(map, v, v->l, hollowFunc);
    if (v->b != NULL && v->b->mark != -1) carvePath(map, v, v->b, hollowFunc);
    if (v->r != NULL && v->r->mark != -1) carvePath(map, v, v->r, hollowFunc);

    if (v->t != NULL && v->t->mark != -1) carveNetwork(map, v->t, hollowFunc);
    if (v->l != NULL && v->l->mark != -1) carveNetwork(map, v->l, hollowFunc);
    if (v->b != NULL && v->b->mark != -1) carveNetwork(map, v->b, hollowFunc);
    if (v->r != NULL && v->r->mark != -1) carveNetwork(map, v->r, hollowFunc);
}

static void freeVerts(struct vert* v)
{
    if (v->t != NULL) v->t->b = NULL;
    if (v->l != NULL) v->l->r = NULL;
    if (v->b != NULL) v->b->t = NULL;
    if (v->r != NULL) v->r->l = NULL;

    if (v->t != NULL) freeVerts(v->t);
    if (v->l != NULL) freeVerts(v->l);
    if (v->b != NULL) freeVerts(v->b);
    if (v->r != NULL) freeVerts(v->r);

    free(v);
}

void map_genDungeon(struct map map, int x, int y, int w, int h,
    void (*hollowFunc)(struct map, int, int, int, int),
    void (*solidFunc)(struct map, int, int, int, int))
{
    struct vert* tl = quad(x + 8, y + 8, w - 16, h - 16);

    subdivide(tl);
    carveNetwork(map, tl, hollowFunc);
    solidFunc(map, x + 4, y + 4, w - 8, h - 8);

    // freeVerts(tl);
}
