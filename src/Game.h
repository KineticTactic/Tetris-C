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
    double gameSpeed;
    double lastTickTime;
    double rowClearSpeed;
    double lastClearTime;
} State;

State *InitGameState();
void DestroyGame(State *state);

void Tick(State *state, bool hardDrop);
void GameSpeedUp(State *state);
void GameSpeedNormal(State *state);

void CreateFallingBlock(State *state);
void AddBlockToBoard(State *state);
bool IsBlockOverlappingBoard(Board *board, Block *block);
void CalculateFallingBlockShadow(State *state);
void DropFallingBlock(State *state);

void MoveFallingBlock(State *state, int direction);
void RotateFallingBlock(State *state, int direction);

void UpdateGame(State *state);
void RenderGame(State *state, int screenWidth, int screenHeight,
                Texture2D tileSpriteSheet);

#endif