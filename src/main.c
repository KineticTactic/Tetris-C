#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "Audio.h"
#include "Game.h"

int main() {
    const int screenWidth = 300;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    InitAudio();

    Texture2D tileSpriteSheet = LoadTexture("assets/tiles.png");
    SetTextureFilter(tileSpriteSheet, TEXTURE_FILTER_POINT);

    State *state = InitGameState();

    bool alreadyTurned = false;
    bool alreadyMoved = false;
    bool alreadyDropped = false;
    double lastMovementInput = GetTime();
    bool paused = false;

    while (!WindowShouldClose()) {
        UpdateMusic();

        if (IsKeyDown(KEY_UP) && !alreadyTurned) {
            RotateFallingBlock(state, 1);
            alreadyTurned = true;
        } else if (!IsKeyDown(KEY_UP) && alreadyTurned)
            alreadyTurned = false;

        /// TODO: Fix this mess of an input handler
        if (IsKeyDown(KEY_LEFT) && !alreadyMoved) {
            MoveFallingBlock(state, -1);
            alreadyMoved = true;
            lastMovementInput = GetTime();
        } else if (IsKeyDown(KEY_RIGHT) && !alreadyMoved) {
            MoveFallingBlock(state, 1);
            alreadyMoved = true;
            lastMovementInput = GetTime();
        } else if (((!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) &&
                    alreadyMoved) ||
                   GetTime() - lastMovementInput > 0.1)
            alreadyMoved = false;

        if (IsKeyDown(KEY_SPACE) && !alreadyDropped) {
            DropFallingBlock(state);
            alreadyDropped = true;
        } else if (!IsKeyDown(KEY_SPACE) && alreadyDropped)
            alreadyDropped = false;

        if (IsKeyPressed(KEY_P))
            paused = !paused;

        if (IsKeyDown(KEY_DOWN))
            GameSpeedUp(state);
        else
            GameSpeedNormal(state);

        // Restart game
        if (IsKeyDown(KEY_R)) {
            DestroyGame(state);
            state = InitGameState();
        }

        UpdateGame(state);

        // Rendering
        BeginDrawing();
        ClearBackground((Color){31, 31, 31, 255});
        RenderGame(state, screenWidth, screenHeight, tileSpriteSheet);
        EndDrawing();
    }

    CloseWindow();

    DestroyGame(state);

    return 0;
}