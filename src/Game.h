#ifndef GAME_H
#define GAME_H

#include "Block.h"
#include "Board.h"

typedef struct {
    Board board;
    Block fallingBlock;
    int score;
    bool isGameOver;
} State;

State *InitGameState();
void DestroyGame(State *state);

void Tick(State *state);

void AddBlockToBoard(State *state);
bool IsFallingBlockOverlapping(State *state);

void MoveFallingBlock(State *state, int direction);
void RotateFallingBlock(State *state, int direction);

void RenderGame(State *state, int screenWidth, int screenHeight);

#endif