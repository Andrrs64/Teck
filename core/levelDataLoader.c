#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "levelDataLoader.h"

#include <raylib.h>

int importLevelData(const char* path, struct LvlObjects* buf) {
    FILE* bin_file = fopen(path, "rb");

    if (bin_file == NULL) {
        printf("Could not open file %s\n", path);
        return 1;
    }

    fseek(bin_file, 0, SEEK_END);
    long size = ftell(bin_file);
    rewind(bin_file);
    if (size < sizeof(int)) {
        printf("Could not read file. File is empty (size: %ld)\n", size);
        fclose(bin_file);
        return 1;
    }

    fread(&buf->count, sizeof(int), 1, bin_file);

    buf->position = (Vector2*)malloc(sizeof(Vector2)*buf->count);
    fread(buf->position, sizeof(Vector2), buf->count, bin_file);

    buf->size = (Vector2*)malloc(sizeof(Vector2)*buf->count);
    fread(buf->size, sizeof(Vector2), buf->count, bin_file);

    fclose(bin_file);

    printf("Read count: %d\n", buf->count);
    for (int i = 0; i < buf->count; i++) {
        printf("Read position %i: x = %f, y = %f\n", i, buf->position[i].x, buf->position[i].y);
        printf("Read size %i: x = %f, y = %f\n", i, buf->size[i].x, buf->size[i].y);
    }

    return 0;
}

int writeLevelData(struct LvlObjects* objects, const char* path) {
    FILE* bin_file = fopen(path, "wb");

    if (bin_file == NULL) {
        printf("Could not open file %s\n", path);
        return 1;
    }

    printf("Writing count: %i\n", objects->count);
    fwrite(&objects->count, sizeof(int), 1, bin_file);

    Vector2* posPtr = objects->position;
    Vector2* sizePtr = objects->size;
    fwrite(posPtr, sizeof(Vector2), objects->count, bin_file);
    fwrite(sizePtr, sizeof(Vector2), objects->count, bin_file);

    fclose(bin_file);

    printf("Size of Vector2: %lu\n", sizeof(Vector2));
    printf("Size of int: %lu\n", sizeof(int));
    printf("Expected file size: %lu\n", sizeof(int) + sizeof(Vector2) * objects->count * 2);

    return 0;
}
