#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <raylib.h>

struct LvlObjects {
    int count;
    Vector2* position;
    Vector2* size;
};

int importLevelData(const char* path, struct LvlObjects* buf);
int writeLevelData(struct LvlObjects* objects, const char* path);

#ifdef __cplusplus
}
#endif

#endif // !LEVEL_LOADER_H
