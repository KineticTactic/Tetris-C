#include "Game.h"

#include <stdlib.h>

#include "Audio.h"
#include "data.h"

State *InitGameState() {
    State *state = (State *)malloc(sizeof(State));
    state->board = CreateBoard(10, 20);
    state->fallingBlock = CreateRandomBlock();
    state->shadowBlock = state->fallingBlock;
    state->score = 0;
    state->isGameOver = false;
    state->isClearingRows = false;
    state->lastTickTime = GetTime();
    state->lastClearTime = GetTime();
    state->gameSpeed = 0.5;
    state->rowClearSpeed = 0.02;
    CalculateFallingBlockShadow(state);
    return state;
}

void DestroyGame(State *state) {
    DestroyBoard(&state->board);
    free(state);
}

void Tick(State *state, bool hardDrop) {
    state->fallingBlock.y++;

    if (IsBlockOverlappingBoard(&state->board, &state->fallingBlock)) {
        state->fallingBlock.y--;
        AddBlockToBoard(state);

        if (hardDrop)
            PlaySoundEffect(HARD_DROP);
        else
            PlaySoundEffect(SOFT_DROP);

        int rowsCleared = ClearRows(&state->board);

        if (rowsCleared > 0) {
            state->score += rowsCleared * 100;
            state->isClearingRows = true;
            state->lastClearTime = GetTime();
            PlaySoundEffect(CLEAR);
            return;
        }

        CreateFallingBlock(state);
    }

    CalculateFallingBlockShadow(state);
}

void GameSpeedUp(State *state) {
    state->gameSpeed = 0.07;
}

void GameSpeedNormal(State *state) {
    state->gameSpeed = 0.5;
}

void CreateFallingBlock(State *state) {
    state->fallingBlock = CreateRandomBlock();
    state->shadowBlock = state->fallingBlock;
    if (IsBlockOverlappingBoard(&state->board, &state->fallingBlock))
        state->isGameOver = true;
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
}

bool IsBlockOverlappingBoard(Board *board, Block *block) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (GetBlockCell(block, x, y) == 0)
                continue;

            int boardX = block->x + x;
            int boardY = block->y + y;

            // Check if cell is outside the board
            if (boardX < 0 || boardX >= board->width || boardY < 0 ||
                boardY >= board->height)
                return true;

            if (GetCell(board, boardX, boardY) != 0)
                return true;
        }
    }
    return false;
}

void CalculateFallingBlockShadow(State *state) {
    state->shadowBlock = state->fallingBlock;
    while (!IsBlockOverlappingBoard(&state->board, &state->shadowBlock)) {
        state->shadowBlock.y++;
    }
    state->shadowBlock.y--;
}

void DropFallingBlock(State *state) {
    state->fallingBlock.y = state->shadowBlock.y;
    Tick(state, true);
}

void MoveFallingBlock(State *state, int direction) {
    state->fallingBlock.x += direction;
    if (IsBlockOverlappingBoard(&state->board, &state->fallingBlock))
        state->fallingBlock.x -= direction;
    CalculateFallingBlockShadow(state);
    // PlaySoundEffect(MOVE);
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

        if (!IsBlockOverlappingBoard(&state->board, &state->fallingBlock)) {
            CalculateFallingBlockShadow(state);
            PlaySoundEffect(ROTATE);
            return;
        }

        state->fallingBlock.x -= relativeOffset.x;
        state->fallingBlock.y -= relativeOffset.y;
    }

    // If all the rotations failed, then rotate back;
    RotateBlock(&state->fallingBlock, -direction);
}

void UpdateGame(State *state) {
    double currentTime = GetTime();

    if (state->isClearingRows &&
        currentTime - state->lastClearTime >= state->rowClearSpeed) {
        bool clearingDone = ContinueClearingRows(&state->board);
        if (clearingDone) {
            state->isClearingRows = false;
            CreateFallingBlock(state);
        }

        state->lastClearTime = currentTime;
        return;
    }

    if (state->isGameOver)
        return;

    if (currentTime - state->lastTickTime >= state->gameSpeed) {
        Tick(state, false);
        state->lastTickTime = currentTime;
    }
}

void RenderGame(State *state, int screenWidth, int screenHeight,
                Texture2D tileSpriteSheet) {
    RenderBoard(&state->board, screenWidth, screenHeight, tileSpriteSheet);
    RenderBlock(&state->fallingBlock, tileSpriteSheet, 1.0);
    RenderBlock(&state->shadowBlock, tileSpriteSheet, 0.2);

    DrawText(TextFormat("Score: %d", state->score), 10, 10, 20, WHITE);

    if (state->isGameOver) {
        DrawRectangle(screenWidth / 2 - 150, screenHeight / 2 - 75, 300, 120,
                      Fade(BLACK, 0.5));
        DrawText("GAME OVER!", screenWidth / 2 - 125, screenHeight / 2 - 50, 40,
                 RED);
        // Write press R to restart
        DrawText("Press R to restart", screenWidth / 2 - 105, screenHeight / 2,
                 20, WHITE);
    }
}