#include "Board.h"

#include <raylib.h>
#include <stdlib.h>
#include <string.h>

Board board_create(int width, int height) {
    Board board;
    board.width = width;
    board.height = height;

    board.data = (int *)malloc(width * height * sizeof(int));
    memset(board.data, 0, width * height * sizeof(int));

    board.rowsToBeCleared = (int *)malloc(height * sizeof(int));
    memset(board.rowsToBeCleared, -1, height * sizeof(int));

    return board;
}

void board_destroy(Board *board) {
    free(board->data);
    board->data = NULL;
    free(board->rowsToBeCleared);
    board->rowsToBeCleared = NULL;
}

// Get a cell from the board
int board_get_cell(Board *board, int x, int y) {
    if (x >= 0 && x < board->width && y >= 0 && y < board->height)
        return board->data[x + y * board->width];
    return 0;
}

// Set a cell on the board
void board_set_cell(Board *board, int x, int y, int value) {
    if (x >= 0 && x < board->width && y >= 0 && y < board->height) {
        board->data[x + y * board->width] = value;
    }
}

// Marks the full rows which needs to be cleared
int board_clear_rows(Board *board) {
    int rowsCleared = 0;

    // Loop over all rows
    for (int y = 0; y < board->height; y++) {
        bool isRowFull = true;

        for (int x = 0; x < board->width; x++) {
            if (board_get_cell(board, x, y) == 0) {
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

// Clears the rows which are marked to be cleared
bool board_continue_clearing_rows(Board *board) {
    bool allCleared = true;

    for (int i = 0; i < board->height; i++) {
        if (board->rowsToBeCleared[i] == -1)
            continue;
        // Set the central cells to 0 (clearing animation starts from center and
        // goes to the edges)
        board_set_cell(board, board->rowsToBeCleared[i], i, 0);
        board_set_cell(board, board->width - board->rowsToBeCleared[i] - 1, i,
                       0);

        board->rowsToBeCleared[i]--;

        // If the row has been completely cleared, move the rows above downwards
        if (board->rowsToBeCleared[i] == -1) {
            for (int y = i; y > 0; y--) {
                for (int x = 0; x < board->width; x++) {
                    board_set_cell(board, x, y,
                                   board_get_cell(board, x, y - 1));
                }
            }
        } else
            allCleared = false;
    }
    return allCleared;
}

void board_render(Board *board, int screenWidth, int screenHeight,
                  Texture2D tileSpriteSheet) {
    int cellWidth = screenWidth / board->width;
    int cellHeight = screenHeight / board->height;

    for (int i = 0; i < board->width; i++) {
        for (int j = 0; j < board->height; j++) {
            const int cell = board_get_cell(board, i, j);
            if (cell == 0)
                continue;

            // DrawRectangle(i * cellWidth, j * cellHeight, cellWidth,
            // cellHeight,
            //               colors[board_get_cell(board, i, j) - 1]);
            DrawTexturePro(tileSpriteSheet,
                           (Rectangle){(cell - 1) * 16, 0, 16, 16},
                           (Rectangle){i * cellWidth, j * cellHeight, cellWidth,
                                       cellHeight},
                           (Vector2){0, 0}, 0, WHITE);
        }
    }
}