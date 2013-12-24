#include "sprite.h"

#define SPRITE_ERROR_MSG(msg) "Error loading sprite '%s': "msg"\n", path

uint8_t* sprite_fromFile(const char* path, uint16_t* width, uint16_t* height)
{
    FILE* fp = NULL;
    uint32_t ident = 0;

    printf("Attempting to load %s\n", path);

    fp = fopen(path, "r");

    if (!fp) {
        fprintf(stderr, SPRITE_ERROR_MSG("Couldn't open file"));
        exit(1);
    }

    fread(&ident, 4, 1, fp);

    if (ident != 0x50494354) {
        fprintf(stderr, SPRITE_ERROR_MSG("Bad file format"));
        exit(1);
    }

    fread(width, 2, 1, fp);
    fread(height, 2, 1, fp);

    printf("Loading %s (%ix%i)\n", path, *width, *height);

    int size = *width * *height;

    uint8_t* sprite = (uint8_t*) malloc(size);

    if (sprite == NULL) {
        fprintf(stderr, SPRITE_ERROR_MSG("Memory error"));
        exit(1);
    }

    int read = fread(sprite, 1, size, fp);

    if (read != size) {
        fprintf(stderr, SPRITE_ERROR_MSG("Unexpected end of file"));
        exit(1);
    }

    fclose(fp);

    return sprite;
}
