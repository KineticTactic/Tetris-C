#include "Game.h"

#include <stdlib.h>

#include "data.h"

State *InitGameState() {
    State *state = (State *)malloc(sizeof(State));
    state->board = CreateBoard(10, 20);
    state->fallingBlock = CreateRandomBlock();
    state->score = 0;
    state->isGameOver = false;
    return state;
}

void DestroyGame(State *state) {
    DestroyBoard(&state->board);
    free(state);
}

void Tick(State *state) {
    state->fallingBlock.y++;

    if (IsFallingBlockOverlapping(state)) {
        state->fallingBlock.y--;
        AddBlockToBoard(state);
        state->fallingBlock = CreateRandomBlock();
        if (IsFallingBlockOverlapping(state))
            state->isGameOver = true;
    }
}

void AddBlockToBoard(State *state) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (GetBlockCell(&state->fallingBlock, x, y) != 0) {
                int boardX = state->fallingBlock.x + x;
                int boardY = state->fallingBlock.y + y;
                SetCell(&state->board, boardX, boardY,
                        GetBlockCell(&state->fallingBlock, x, y));
            }
        }
    }
    state->score += ClearRows(&state->board);
}

bool IsFallingBlockOverlapping(State *state) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (GetBlockCell(&state->fallingBlock, x, y) == 0)
                continue;

            int boardX = state->fallingBlock.x + x;
            int boardY = state->fallingBlock.y + y;

            // Check if cell is outside the board
            if (boardX < 0 || boardX >= state->board.width || boardY < 0 ||
                boardY >= state->board.height)
                return true;

            if (GetCell(&state->board, boardX, boardY) != 0)
                return true;
        }
    }
    return false;
}

void MoveFallingBlock(State *state, int direction) {
    state->fallingBlock.x += direction;
    if (IsFallingBlockOverlapping(state))
        state->fallingBlock.x -= direction;
}

void RotateFallingBlock(State *state, int direction) {
    int initialRot = state->fallingBlock.rotation;
    RotateBlock(&state->fallingBlock, direction);

    const Offset(*offsets)[5] =
        state->fallingBlock.type == I_PIECE ? I_PIECE_OFFSETS : OFFSETS;

    for (int i = 0; i < 5; i++) {
        Offset initialRotationOffset = offsets[initialRot][i];
        Offset finalRotationOffset = offsets[state->fallingBlock.rotation][i];
        Offset relativeOffset = {
            initialRotationOffset.x - finalRotationOffset.x,
            initialRotationOffset.y - finalRotationOffset.y};

        state->fallingBlock.x += relativeOffset.x;
        state->fallingBlock.y += relativeOffset.y;

        if (!IsFallingBlockOverlapping(state))
            return;

        state->fallingBlock.x -= relativeOffset.x;
        state->fallingBlock.y -= relativeOffset.y;
    }

    // If all the rotations failed, then rotate back;
    RotateBlock(&state->fallingBlock, -direction);
}

void RenderGame(State *state, int screenWidth, int screenHeight) {
    RenderBoard(&state->board, screenWidth, screenHeight);
    RenderBlock(&state->fallingBlock);

    DrawText(TextFormat("Score: %d", state->score), 10, 10, 20, WHITE);

    if (state->isGameOver) {
        DrawRectangle(screenWidth / 2 - 150, screenHeight / 2 - 75, 300, 120,
                      Fade(BLACK, 0.3));
        DrawText("GAME OVER!", screenWidth / 2 - 125, screenHeight / 2 - 50, 40,
                 RED);
        // Write press R to restart
        DrawText("Press R to restart", screenWidth / 2 - 105, screenHeight / 2,
                 20, WHITE);
    }
}