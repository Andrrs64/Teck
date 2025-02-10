#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "levelDataLoader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PlayerState {
    Model model;
    ModelAnimation* modelAnimations;
    int animsCount;
    int animIndex;
    int animCurrentFrame;

    Vector2 position;
    Vector2 size;
    Vector2 velocity;

    float speed;

    Camera3D camera;
    bool freeCam;
};

void initPlayer(struct PlayerState* state);
void updatePlayer(struct PlayerState* player, struct LvlObjects* objects, float delta);
void drawPlayer(struct PlayerState* player, struct LvlObjects* objects);

void freeCam(struct PlayerState* player, float delta);

#ifdef __cplusplus
}
#endif

#endif // !PLAYER_H
