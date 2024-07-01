#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "Audio.h"
#include "Game.h"
#include "Input.h"

int main() {
    const int screenWidth = 300;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    audio_init();

    Texture2D tileSpriteSheet = LoadTexture("assets/tiles.png");
    SetTextureFilter(tileSpriteSheet, TEXTURE_FILTER_POINT);

    State *state = game_init();

    while (!WindowShouldClose()) {
        // Input handling
        input_handle(&state);
        if (IsKeyPressed(KEY_R)) {
            // game_destroy(state);
            // state = game_init();
        }

        // Updates
        audio_update();
        game_update(state);

        // Rendering
        BeginDrawing();
        ClearBackground((Color){15, 15, 15, 255});

        game_render(state, screenWidth, screenHeight, tileSpriteSheet);

        EndDrawing();
    }

    CloseWindow();

    game_destroy(state);

    return 0;
}