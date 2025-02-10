#include "player.h"
#include "levelDataLoader.h"
#include <_stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#define GRAVITY 0.05f
#define FRICTION 3.0f
#define VELOCITY_DEADZONE 0.05f
#define JUMP_POWER 0.3f
#define MAX_JUMP 15
#define IN_AIR_CONTROL 0.1f
#define PLAYER_ACCELERATION 5

int jumpCount = 0;
bool jumpHeld = false;
bool grounded = false;
float moveForce = 0.0f;

void initPlayer(struct PlayerState* state) {
    state->position.x = 0.0f;
    state->position.y = 0.0f;
    state->size.x = 3.0f;
    state->size.y = 6.0f;
    state->speed = 0.4f;

    state->camera = (Camera){
        (Vector3){ 0.0f, 0.0f, 10.0f },
        (Vector3){ 0.0f, 0.0f, 0.0f },
        (Vector3){ 0.0f, 1.0f, 0.0f },
        20.0f,
        CAMERA_ORTHOGRAPHIC
    };
    state->position.y = -4;
}

void _movePlayer(struct PlayerState* player, struct LvlObjects* objects, float delta);
void _moveCamera(struct PlayerState* player, float delta);
void freeCamera(struct PlayerState* player, float delta);

void updatePlayer(struct PlayerState* player, struct LvlObjects* objects, float delta) {
    if (player->freeCam) {
        freeCam(player, delta);
    } else {
        _movePlayer(player, objects, delta);
        _moveCamera(player, delta);
    }
}

void drawPlayer(struct PlayerState* player, struct LvlObjects* objects) {
    if (objects != NULL) {
        Vector2* size = objects->size;
        Vector2* position = objects->position;

        for (int i = 0; i < objects->count; ++i) {
            float halfWidth = size->x * 0.5f;
            float halfHeight = size->y * 0.5f;

            DrawTriangle3D(
                    (Vector3){position->x - halfWidth, position->y + halfHeight, 0},
                    (Vector3){position->x - halfWidth, position->y - halfHeight, 0},
                    (Vector3){position->x + halfWidth, position->y + halfHeight, 0},
                    GREEN
                    );
            DrawTriangle3D(
                    (Vector3){position->x - halfWidth, position->y - halfHeight, 0},
                    (Vector3){position->x + halfWidth, position->y - halfHeight, 0},
                    (Vector3){position->x + halfWidth, position->y + halfHeight, 0},
                    GREEN
                    );

            size++;
            position++;
        }
    }

    DrawCube(
        (Vector3){
            player->position.x,
            player->position.y + (player->size.y * 0.5f),
            -5.0f
        },
        player->size.x,
        player->size.y,
        1.0f,
        RED
    );
}

void _movePlayer(struct PlayerState* player, struct LvlObjects* objects, float delta) {
    grounded = false;

    if (objects == NULL) {
        return;
    }

    for (int i = 0; i < objects->count; ++i) {
        Vector2 size = objects->size[i];
        Vector2 position = objects->position[i];

        float pHalfWidth = player->size.x * 0.5f;
        float oHalfWidth = size.x * 0.5f;
        float oHalfHeight = size.y * 0.5f;

        if (player->position.x + pHalfWidth >= position.x - oHalfWidth &&
                player->position.x - pHalfWidth <= position.x + oHalfWidth) {

            if (player->position.x + pHalfWidth >= position.x - oHalfWidth &&
                    player->position.x < position.x - oHalfWidth &&
                    player->position.y > position.y - oHalfHeight &&
                    player->position.y + player->size.y < position.y + oHalfHeight){

                player->position.x = position.x - oHalfWidth - pHalfWidth;
                player->velocity.x = 0.0f;
            }

            else if (player->position.x - pHalfWidth <= position.x + oHalfWidth &&
                    player->position.x > position.x + oHalfWidth &&
                    player->position.y > position.y + oHalfHeight &&
                    player->position.y + player->size.y < position.y - oHalfHeight) {

                player->position.x = position.x + oHalfWidth + pHalfWidth;
                player->velocity.x = 0.0f;
            }

            else if (player->position.y <= position.y + oHalfHeight &&
                    player->position.y >= position.y - oHalfHeight) {

                player->position.y = position.y + oHalfHeight;
                player->velocity.y = 0.0f;
                grounded = true;
            }

            else if (player->position.y + player->size.y >= position.y - oHalfHeight &&
                    player->position.y + player->size.y <= position.y + oHalfHeight) {

                player->position.y = position.y  - oHalfHeight - player->size.y;
                player->velocity.y = 0.0f;
                jumpCount = MAX_JUMP;
            }
        }
    }

    if (grounded) {
        if (!jumpHeld) {
            jumpCount = 0;
        }

        if (player->velocity.x < 0) {
            if (player->velocity.x > -VELOCITY_DEADZONE) {
                player->velocity.x = 0.0f;
            } else {
                player->velocity.x += FRICTION * delta;
            }
        }
        if (player->velocity.x > 0) {
            if (player->velocity.x < VELOCITY_DEADZONE) {
                player->velocity.x = 0.0f;
            } else {
                player->velocity.x -= FRICTION * delta;
            }
        }
    } else {
        player->velocity.y -= GRAVITY;
    }

    moveForce = PLAYER_ACCELERATION * delta;

    if (!grounded) {
        moveForce *= IN_AIR_CONTROL;
    }
    if (IsKeyDown(KEY_D)) {
        player->velocity.x += moveForce;
        if (player->velocity.x > player->speed) {
            player->velocity.x = player->speed;
        }
    }
    if (IsKeyDown(KEY_A)) {
        player->velocity.x -= moveForce;
        if (player->velocity.x < -player->speed) {
            player->velocity.x = -player->speed;
        }
    }

    if (IsKeyDown(KEY_SPACE)) {
        if (jumpCount < MAX_JUMP) {
            player->velocity.y = JUMP_POWER;
            jumpCount++;
            jumpHeld = true;
        }
    } else if (grounded) {
        jumpHeld = false;
    } else if (jumpHeld == true) {
        jumpCount = MAX_JUMP;
    }

    player->position = Vector2Add(player->position, player->velocity);
}

#define CAMERA_FOLLOW_WIGGLE 4.0f
#define CAMERA_Y_OFFSET 4.0f

void _moveCamera(struct PlayerState* player, float delta) {
    if (player->camera.position.x > player->position.x + CAMERA_FOLLOW_WIGGLE) {
        player->camera.position.x = player->position.x + CAMERA_FOLLOW_WIGGLE;
    }
    if (player->camera.position.x < player->position.x - CAMERA_FOLLOW_WIGGLE) {
        player->camera.position.x = player->position.x - CAMERA_FOLLOW_WIGGLE;
    }
    player->camera.target.x = player->camera.position.x;

    player->camera.position.y = player->position.y + CAMERA_Y_OFFSET;
    player->camera.target.y = player->camera.position.y;
}

#define FREECAM_SPEED 50.0f

bool zidb = false;
bool zodb = false;

void freeCam(struct PlayerState* player, float delta) {
    if (IsKeyDown(KEY_D)) {
        player->camera.position.x += FREECAM_SPEED * delta;
    }
    if (IsKeyDown(KEY_A)) {
        player->camera.position.x -= FREECAM_SPEED * delta;
    }

    if (IsKeyDown(KEY_W)) {
        player->camera.position.y += FREECAM_SPEED * delta;
    }
    if (IsKeyDown(KEY_S)) {
        player->camera.position.y -= FREECAM_SPEED * delta;
    }

    player->camera.target.x = player->camera.position.x;
    player->camera.target.y = player->camera.position.y;

    if (IsKeyDown(KEY_E)) {
        if (!zidb) {
            player->camera.fovy -= 5.0f;
            zidb = true;
        }
    } else {
        zidb = false;
    }
    if (IsKeyDown(KEY_Q)) {
        if (!zodb) {
            player->camera.fovy += 5.0f;
            zodb = true;
        }
    } else {
        zodb = false;
    }
}
