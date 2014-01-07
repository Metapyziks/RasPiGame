#ifndef _UTILS_H_
#define _UTILS_H_ 1

#define TRUE 1
#define FALSE 0

#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#define ABS(x) ((x) >= 0 ? (x) : -(x))

#define WRAP(x, y) ((x) >= 0 ? (x) % (y) : ((y) - 1) - ((-1 - (x)) % (y)))

#define FLOOR(x, y) (((x) >= 0 ? (x) : ((x) - ((y) - 1))) / (y))
#define CEIL(x, y) (((x) >= 0 ? ((x) + (y) - 1) : (x)) / (y))

#define NEW(t, c) ((t*) malloc(sizeof(t) * (c)))

#endif
