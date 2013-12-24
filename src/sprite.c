#include "sprite.h"

uint8_t* sprite_fromFile(const char* path, uint16_t* width, uint16_t* height)
{
    FILE* fp;
    unsigned int ident;

    fp = fopen(path, "r");

    fread(&ident, 4, 1, fp);

    if (ident != 0x50494354) {
        fprintf(stderr, "Error loading sprite '%s': Bad file format", path);
        exit(1);
    }

    fread(width, 2, 1, fp);
    fread(height, 2, 1, fp);

    printf("Loading %s (%ix%i)", path, *width, *height);

    uint8_t* sprite = (uint8_t*) malloc(width * height);

    if (sprite == NULL) {
        fprintf(stderr, "Memory error");
        exit(1);
    }

    int read = fread(sprite, 1, width * height, fp);

    if (read != width * height) {
        fprintf(stderr, "Error loading sprite '%s': Unexpected end of file", path);
        exit(1);
    }

    fclose(file);

    return sprite;
}
