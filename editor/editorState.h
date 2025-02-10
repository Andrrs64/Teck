#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <player.h>
#include <levelDataLoader.h>
#include <vector>

struct EditorState {
    int windowWidth;
    int windowHeight;

    bool runningTest;
    bool objectListOpen;
    bool objectSettingsOpen;

    char* filePath;

    int objectIndex;

    // in editor level data
    int objectCount;
    std::vector<Vector2> positions;
    std::vector<Vector2> sizes;

    PlayerState playerState;
    LvlObjects dataBuffer;
};

#endif // EDITOR_STATE_H
