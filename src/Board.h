#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

typedef struct {
    int width;
    int height;

    // Data is stored as a 1D array of ints, starting from top right to bottom
    // left. 0 represets an empty cell, and any other number (1-7) represents a
    // color of the filled tile.
    int *data;

    // Array of ints, each representing a row.
    // If a row is to be cleared, the value is set to 5 (as there are 10
    // columns) If a row is being cleared, the value is decremented, and two
    // cells are cleared symmetrically from the center
    // 1111111111 -> 1111001111 -> 1110000111 -> ...
    // Default value is -1 (row isnt being cleared)
    int *rowsToBeCleared;
} Board;

Board board_create(int width, int height);
void board_destroy(Board *b);
int board_get_cell(Board *b, int x, int y);
void board_set_cell(Board *b, int x, int y, int value);
int board_clear_rows(Board *b);
bool board_continue_clearing_rows(Board *b); // Returns true if clearing is done
void board_render(Board *b, int screenWidth, int screenHeight,
                  Texture2D tileSpriteSheet);

#endif