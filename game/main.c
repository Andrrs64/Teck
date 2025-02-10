#include <raylib.h>
#include <stdio.h>
#include <player.h>
#include <levelDataLoader.h>

struct GameState {
    int screenWidth;
    int screenHeight;

    struct PlayerState playerState;
    struct LvlObjects objects;
};

int main() {
    struct GameState state;

    state.screenWidth = 800;
    state.screenHeight = 450;
    state.playerState.freeCam = false;

    InitWindow(state.screenWidth, state.screenHeight, "Teck");

    SetTargetFPS(60);

    initPlayer(&state.playerState);

    /*importLevelData("./test.dat", &state.objects);*/

    float delta = 0.0f;
    bool fcdb = false;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();

        if (IsKeyDown(KEY_G)) {
            if (!fcdb) {
                state.playerState.freeCam = !state.playerState.freeCam;
                fcdb = true;
            }
        } else {
            fcdb = false;
        }

        updatePlayer(&state.playerState, &state.objects, delta);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(state.playerState.camera);
                drawPlayer(&state.playerState, &state.objects);
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
