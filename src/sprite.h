#ifndef _SPRITE_H_
#define _SPRITE_H_ 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t* sprite_fromFile(const char* path, uint16_t* width, uint16_t* height);

#endif
