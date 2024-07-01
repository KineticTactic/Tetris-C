#include "Game.h"

#include <stdlib.h>

#include "Audio.h"
#include "data.h"

// Create a new game state
State *game_init() {
    State *state = (State *)malloc(sizeof(State));
    state->board = board_create(10, 20);
    state->fallingBlock = block_create_random();
    state->shadowBlock = state->fallingBlock;
    state->score = 0;
    state->isGameOver = false;
    state->isClearingRows = false;
    state->lastTickTime = GetTime();
    state->lastClearTime = GetTime();
    state->gameSpeed = 0.5;
    state->rowClearSpeed = 0.02;
    state->paused = false;
    game_update_shadow_block(state);
    return state;
}

// Destroy the game state
void game_destroy(State *state) {
    board_destroy(&state->board);
    free(state);
}

// ------------ GAME FUNCTIONS ------------

// Game tick moves the falling block downwards
void game_tick(State *state, bool hardDrop) {
    state->fallingBlock.y++;

    // Check if the block is overlapping with the board or the bottom
    if (game_is_block_overlapping_board(&state->board, &state->fallingBlock)) {
        state->fallingBlock.y--;

        game_add_block_to_board(state);
        audio_play(hardDrop ? HARD_DROP : SOFT_DROP);

        int rowsCleared = board_clear_rows(&state->board);
        if (rowsCleared > 0) {
            state->score += rowsCleared * 100;
            state->isClearingRows = true;
            state->lastClearTime = GetTime();
            audio_play(CLEAR);
            // If row was cleared, dont immediately create a new block
            // as clear animation needs time to play out
            return;
        }
        game_create_block(state);
    }
    game_update_shadow_block(state);
}

// Game update occurs every frame
void game_update(State *state) {
    if (state->paused)
        return;

    double currentTime = GetTime();

    // If we are currently clearing rows
    if (state->isClearingRows &&
        currentTime - state->lastClearTime >= state->rowClearSpeed) {

        bool clearingDone = board_continue_clearing_rows(&state->board);
        if (clearingDone) {
            state->isClearingRows = false;
            game_create_block(state);
        }

        state->lastClearTime = currentTime;
        return;
    }

    if (state->isGameOver)
        return;

    // Run game tick every gameSpeed seconds
    if (currentTime - state->lastTickTime >= state->gameSpeed) {
        game_tick(state, false);
        state->lastTickTime = currentTime;
    }
}

// Render the game
void game_render(State *state, int screenWidth, int screenHeight,
                 Texture2D tileSpriteSheet) {
    board_render(&state->board, screenWidth, screenHeight, tileSpriteSheet);
    block_render(&state->fallingBlock, tileSpriteSheet, 1.0);
    block_render(&state->shadowBlock, tileSpriteSheet, 0.2);

    DrawText(TextFormat("Score: %d", state->score), 10, 10, 20, WHITE);

    if (state->isGameOver) {
        DrawRectangle(screenWidth / 2 - 150, screenHeight / 2 - 75, 300, 120,
                      Fade(BLACK, 0.5));
        DrawText("GAME OVER!", screenWidth / 2 - 125, screenHeight / 2 - 50, 40,
                 RED);
        DrawText("Press R to restart", screenWidth / 2 - 105, screenHeight / 2,
                 20, WHITE);
    }
}

// ------------ INTERACTION FUNCTIONS ------------

void game_drop_block(State *state) {
    state->fallingBlock.y = state->shadowBlock.y;
    game_tick(state, true);
}

void game_move_block(State *state, int direction) {
    state->fallingBlock.x += direction;
    if (game_is_block_overlapping_board(&state->board, &state->fallingBlock))
        state->fallingBlock.x -= direction;
    game_update_shadow_block(state);
    // audio_play(MOVE);
}

void game_rotate_block(State *state, int direction) {
    int initialRot = state->fallingBlock.rotation;
    block_rotate(&state->fallingBlock, direction);

    // Check wall kicks
    // I piece has different offset table
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

        if (!game_is_block_overlapping_board(&state->board,
                                             &state->fallingBlock)) {
            game_update_shadow_block(state);
            audio_play(ROTATE);
            return;
        }

        state->fallingBlock.x -= relativeOffset.x;
        state->fallingBlock.y -= relativeOffset.y;
    }

    // If all the rotations failed, then rotate back;
    block_rotate(&state->fallingBlock, -direction);
}

void game_set_speed(State *state, enum GameSpeed speed) {
    if (speed == NORMAL)
        state->gameSpeed = 0.5;
    else if (speed == FAST)
        state->gameSpeed = 0.07;
}

// ------------ INTERNAL FUNCTIONS ------------

void game_create_block(State *state) {
    state->fallingBlock = block_create_random();
    state->shadowBlock = state->fallingBlock;
    if (game_is_block_overlapping_board(&state->board, &state->fallingBlock))
        state->isGameOver = true;
}

void game_add_block_to_board(State *state) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (block_get_cell(&state->fallingBlock, x, y) != 0) {
                int boardX = state->fallingBlock.x + x;
                int boardY = state->fallingBlock.y + y;
                board_set_cell(&state->board, boardX, boardY,
                               block_get_cell(&state->fallingBlock, x, y));
            }
        }
    }
}

void game_update_shadow_block(State *state) {
    state->shadowBlock = state->fallingBlock;
    while (
        !game_is_block_overlapping_board(&state->board, &state->shadowBlock)) {
        state->shadowBlock.y++;
    }
    state->shadowBlock.y--;
}

bool game_is_block_overlapping_board(Board *board, Block *block) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (block_get_cell(block, x, y) == 0)
                continue;

            int boardX = block->x + x;
            int boardY = block->y + y;

            // Check if cell is outside the board
            if (boardX < 0 || boardX >= board->width || boardY < 0 ||
                boardY >= board->height)
                return true;

            if (board_get_cell(board, boardX, boardY) != 0)
                return true;
        }
    }
    return false;
}
