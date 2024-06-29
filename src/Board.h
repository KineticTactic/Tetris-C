#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int width;
    int height;
    int *data;
} Board;

Board CreateBoard(int width, int height);
void DestroyBoard(Board *b);
int GetCell(Board *b, int x, int y);
void SetCell(Board *b, int x, int y, int value);
int ClearRows(Board *b);
void RenderBoard(Board *b, int screenWidth, int screenHeight);

#endif