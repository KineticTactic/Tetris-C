#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

typedef struct {
    int width;
    int height;
    int *data;

    int *rowsToBeCleared;
} Board;

Board CreateBoard(int width, int height);
void DestroyBoard(Board *b);
int GetCell(Board *b, int x, int y);
void SetCell(Board *b, int x, int y, int value);
int ClearRows(Board *b);
bool ContinueClearingRows(Board *b);
void RenderBoard(Board *b, int screenWidth, int screenHeight,
                 Texture2D tileSpriteSheet);

#endif