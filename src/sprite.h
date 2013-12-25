#ifndef _SPRITE_H_
#define _SPRITE_H_ 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t* sprite_fromFile(const char* path, int* width, int* height, int* tiles);

#endif
