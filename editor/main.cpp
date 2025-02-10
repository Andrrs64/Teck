#include <iostream>
#include <stdio.h>
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <levelDataLoader.h>
#include <player.h>
#include "editorState.h"
#include "external/tinyfiledialogs.h"

void loadLevelDataToBuffer(EditorState* state) {
    free(state->dataBuffer.position);
    free(state->dataBuffer.size);

    state->dataBuffer.count = state->objectCount;
    state->dataBuffer.position = (Vector2*)malloc(sizeof(Vector2)*state->dataBuffer.count);
    state->dataBuffer.size = (Vector2*)malloc(sizeof(Vector2)*state->dataBuffer.count);

    Vector2* posPtr = state->dataBuffer.position;
    Vector2* sizePtr = state->dataBuffer.size;

    for (int i = 0; i < state->dataBuffer.count; i++) {
        *posPtr = state->positions[i];
        *sizePtr = state->sizes[i];

        posPtr++;
        sizePtr++;
    }
}

void loadBufferDataToEditor(EditorState* state) {
    std::cout << "Level data count: " << state->dataBuffer.count << "\n";

    Vector2* posPtr = state->dataBuffer.position;
    Vector2* sizePtr = state->dataBuffer.size;
    state->positions.clear();
    state->sizes.clear();
    state->objectCount = state->dataBuffer.count;

    for (int i = 0; i < state->dataBuffer.count; i++) {
        state->positions.push_back(*posPtr);
        state->sizes.push_back(*sizePtr);
        posPtr++;
        sizePtr++;
    }
}

int main() {
    struct EditorState state = {1280, 720};
    state.objectIndex = -1;
    state.runningTest = false;

    initPlayer(&state.playerState);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(state.windowWidth, state.windowHeight, "Level editor");

    SetTargetFPS(60);
    rlImGuiSetup(true);

    float dt;
    char const* lFilterPatterns[2] = {"*.dat"};

    while (!WindowShouldClose()) {
        state.windowWidth = GetScreenWidth();
        state.windowHeight = GetScreenHeight();

        dt = GetFrameTime();

        if (state.runningTest) {
            updatePlayer(&state.playerState, &state.dataBuffer, dt);
        } else {
            freeCam(&state.playerState, dt);
        }

        BeginDrawing();
            ClearBackground(WHITE);

            BeginMode3D(state.playerState.camera);
                for (int oIndex = 0; oIndex < state.objectCount; ++oIndex) {
                    DrawCube(
                        {state.positions[oIndex].x, state.positions[oIndex].y, 0.0f},
                        state.sizes[oIndex].x,
                        state.sizes[oIndex].y,
                        1.0f,
                        GREEN);
                }

                drawPlayer(&state.playerState,NULL);
            EndMode3D();

            rlImGuiBegin();

            ImGui::Begin("File");

            if (ImGui::Button("Open")) {
                state.filePath = tinyfd_openFileDialog(
                    "Open file",
                    state.filePath == NULL ? "./" : state.filePath,
                    1,
                    lFilterPatterns,
                    NULL,
                    0
                );

                if (state.filePath != NULL) {
                    std::cout << state.filePath << "\n";

                    int fileReadError = importLevelData(state.filePath, &state.dataBuffer);
                    if (fileReadError == 0) {
                        loadBufferDataToEditor(&state);
                    }
                }
            }

            if (state.filePath != NULL && ImGui::Button("Save")) {
                loadLevelDataToBuffer(&state);
                writeLevelData(&state.dataBuffer, state.filePath);
            }

            ImGui::End();

            ImGui::Begin("Player");

                if (ImGui::Button("Play")) {
                    loadLevelDataToBuffer(&state);
                    state.runningTest = true;
                }

                if (ImGui::Button("Stop")) {
                    state.runningTest = false;
                    initPlayer(&state.playerState);
                }

            ImGui::End();

            ImGui::Begin("Objects");

                if (ImGui::Button("Add +")) {
                    state.objectCount++;
                    state.positions.push_back({0.0f, 0.0f});
                    state.sizes.push_back({10.0f, 10.0f});
                }

                for (int oIndex = 0; oIndex < state.objectCount; oIndex++) {
                    if (ImGui::Button(("Object##" + std::to_string(oIndex)).c_str())) {
                        state.objectIndex = oIndex;
                        state.objectSettingsOpen = true;
                    }
                }

            ImGui::End();

            if (state.objectIndex >= 0) {
                ImGui::Begin("Object settings", &state.objectSettingsOpen);

                    ImGui::DragFloat2("Position", &state.positions[state.objectIndex].x, 1.0f);
                    ImGui::DragFloat2("Size##", &state.sizes[state.objectIndex].x, 1.0f);

                ImGui::End();
            }
            rlImGuiEnd();

            DrawText(std::to_string(GetFPS()).c_str(), 5, 5, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    rlImGuiShutdown();

    free(state.dataBuffer.position);
    free(state.dataBuffer.size);

    return 0;
}
