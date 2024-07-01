#ifndef GAME_H
#define GAME_H

#include "Block.h"
#include "Board.h"

typedef struct {
    Board board;
    Block fallingBlock;
    Block shadowBlock;
    int score;
    bool isGameOver;
    bool isClearingRows;
    bool paused;
    double gameSpeed;
    double lastTickTime;
    double rowClearSpeed;
    double lastClearTime;
} State;

enum GameSpeed { NORMAL, FAST };

// Public functions
State *game_init();
void game_destroy(State *state);

void game_tick(State *state, bool hardDrop);
void game_update(State *state);
void game_render(State *state, int screenWidth, int screenHeight,
                 Texture2D tileSpriteSheet);

// Interaction functions
void game_drop_block(State *state);
void game_move_block(State *state, int direction);
void game_rotate_block(State *state, int direction);
void game_set_speed(State *state, enum GameSpeed speed);

// Internal functions (used in Game.c)
void game_create_block(State *state);
void game_add_block_to_board(State *state);
void game_update_shadow_block(State *state);
bool game_is_block_overlapping_board(Board *board, Block *block);

#endif