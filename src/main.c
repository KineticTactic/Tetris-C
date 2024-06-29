#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "Game.h"

int main() {
    const int screenWidth = 300;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    State *state = InitGameState();

    bool alreadyTurned = false;
    bool alreadyMoved = false;
    double lastTick = GetTime();
    double gameSpeed = 0.5;
    double lastMovementInput = GetTime();

    while (!WindowShouldClose()) {
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

        if (IsKeyDown(KEY_DOWN))
            gameSpeed = 0.08;
        else
            gameSpeed = 0.5;

        // Restart game
        if (IsKeyDown(KEY_R)) {
            DestroyGame(state);
            state = InitGameState();
        }

        double currentTime = GetTime();
        if (currentTime - lastTick >= gameSpeed) {
            Tick(state);
            lastTick = currentTime;
        }

        // Rendering
        BeginDrawing();
        ClearBackground((Color){31, 31, 31, 255});
        RenderGame(state, screenWidth, screenHeight);
        EndDrawing();
    }

    CloseWindow();

    DestroyGame(state);

    return 0;
}