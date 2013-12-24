#include "sprite.h"

uint8_t* sprite_fromFile(const char* path, uint16_t* width, uint16_t* height)
{
    FILE* fp;
    uint32_t ident;

    printf("Attempting to load %s\n", path);

    fp = fopen(path, "r");

    fread(&ident, 4, 1, fp);

    if (ident != 0x50494354) {
        fprintf(stderr, "Error loading sprite '%s': Bad file format\n", path);
        exit(1);
    }

    fread(width, 2, 1, fp);
    fread(height, 2, 1, fp);

    printf("Loading %s (%ix%i)\n", path, *width, *height);

    int size = *width * *height;

    uint8_t* sprite = (uint8_t*) malloc(size);

    if (sprite == NULL) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }

    int read = fread(sprite, 1, size, fp);

    if (read != size) {
        fprintf(stderr, "Error loading sprite '%s': Unexpected end of file\n", path);
        exit(1);
    }

    fclose(fp);

    return sprite;
}
