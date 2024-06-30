#include "Board.h"

#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#include "Colors.h"

Board CreateBoard(int width, int height) {
    Board board;
    board.width = width;
    board.height = height;
    board.data = (int *)malloc(width * height * sizeof(int));
    memset(board.data, 0, width * height * sizeof(int));

    board.rowsToBeCleared = (int *)malloc(height * sizeof(int));
    memset(board.rowsToBeCleared, -1, height * sizeof(int));

    return board;
}

void DestroyBoard(Board *board) {
    free(board->data);
    board->data = NULL;
    board->width = 0;
    board->height = 0;
}

int GetCell(Board *board, int x, int y) {
    if (x >= 0 && x < board->width && y >= 0 && y < board->height)
        return board->data[x + y * board->width];
    return 0;
}

void SetCell(Board *board, int x, int y, int value) {
    if (x >= 0 && x < board->width && y >= 0 && y < board->height) {
        board->data[x + y * board->width] = value;
    }
}

int ClearRows(Board *board) {
    // int rowsCleared = 0;
    // for (int y = board->height - 1; y >= 0; y--) {
    //     bool isRowFull = true;
    //     for (int x = 0; x < board->width; x++) {
    //         if (GetCell(board, x, y) == 0) {
    //             isRowFull = false;
    //             break;
    //         }
    //     }

    //     if (isRowFull) {
    //         for (int y2 = y; y2 > 0; y2--) {
    //             for (int x = 0; x < board->width; x++) {
    //                 SetCell(board, x, y2, GetCell(board, x, y2 - 1));
    //             }
    //         }
    //         y++;
    //         rowsCleared++;
    //         board->rowsToBeCleared[y] = board->width / 2;
    //         board->framesTillClearStart = 0;
    //     }
    // }
    // return rowsCleared;

    // Dont clear rows, just mark them as to be cleared
    int rowsCleared = 0;
    for (int y = board->height - 1; y >= 0; y--) {
        bool isRowFull = true;
        for (int x = 0; x < board->width; x++) {
            if (GetCell(board, x, y) == 0) {
                isRowFull = false;
                break;
            }
        }

        if (isRowFull) {
            board->rowsToBeCleared[y] = board->width / 2;
            rowsCleared++;
        }
    }
    return rowsCleared;
}

bool ContinueClearingRows(Board *board) {
    bool allCleared = true;
    for (int i = 0; i < board->height; i++) {
        if (board->rowsToBeCleared[i] == -1)
            continue;
        SetCell(board, board->rowsToBeCleared[i], i, 0);
        SetCell(board, board->width - board->rowsToBeCleared[i] - 1, i, 0);

        board->rowsToBeCleared[i]--;

        if (board->rowsToBeCleared[i] == -1) {
            for (int y2 = i; y2 > 0; y2--) {
                for (int x = 0; x < board->width; x++) {
                    SetCell(board, x, y2, GetCell(board, x, y2 - 1));
                }
            }
        } else
            allCleared = false;
    }
    return allCleared;
}

void RenderBoard(Board *board, int screenWidth, int screenHeight,
                 Texture2D tileSpriteSheet) {
    int cellWidth = screenWidth / board->width;
    int cellHeight = screenHeight / board->height;

    for (int i = 0; i < board->width; i++) {
        for (int j = 0; j < board->height; j++) {
            const int cell = GetCell(board, i, j);
            if (cell == 0)
                continue;

            // DrawRectangle(i * cellWidth, j * cellHeight, cellWidth,
            // cellHeight,
            //               colors[GetCell(board, i, j) - 1]);
            DrawTexturePro(tileSpriteSheet,
                           (Rectangle){(cell - 1) * 16, 0, 16, 16},
                           (Rectangle){i * cellWidth, j * cellHeight, cellWidth,
                                       cellHeight},
                           (Vector2){0, 0}, 0, WHITE);
        }
    }
}