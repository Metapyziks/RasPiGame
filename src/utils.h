#ifndef _UTILS_H_
#define _UTILS_H_ 1

#define TRUE 1
#define FALSE 0

#define WRAP(x, w) ((x) >= 0 ? (x) % (w) : ((w) - 1) - ((-1 - (x)) % (w)))

#endif
