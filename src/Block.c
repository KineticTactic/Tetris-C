#include "Block.h"

#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Block block_create(enum BlockTypes id) {
    if (id < 0 || id >= 7)
        id = 0;

    Block block;
    // Copy over the block layout to this block
    memcpy(block.data, BLOCK_LAYOUTS[id], sizeof(int) * BLOCK_LAYOUT_SIZE);
    block.originX = BLOCK_ORIGINS[id][0];
    block.originY = BLOCK_ORIGINS[id][1];

    int color = GetRandomValue(1, NUM_BLOCK_COLORS);
    for (int i = 0; i < 16; i++) {
        block.data[i] *= color;
    }
    block.x = 3;
    block.y = 1;
    block.type = id;
    block.rotation = 0;
    return block;
}

Block block_create_random() {
    return block_create(GetRandomValue(0, NUM_BLOCK_TYPES - 1));
}

int block_get_cell(Block *block, int x, int y) {
    if (x >= 0 && x < 4 && y >= 0 && y < 4)
        return block->data[x + y * 4];
    return 0;
}

void block_rotate(Block *block, int direction) {
    // Loop over all 16 cells, rotate them and store their values in the rotated
    // positions
    int rotated[16] = {0};

    double halfCellWidth = 0.5;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            // Calculate the position of the center of the cell wrt to the
            // origin of the tetromino
            double xWrtOrigin = x - block->originX + halfCellWidth;
            double yWrtOrigin = y - block->originY + halfCellWidth;

            // Use vector rotation to rotate by 90 degrees
            Vector2 vecWrtOrigin = {xWrtOrigin, yWrtOrigin};
            Vector2 rotatedVector =
                Vector2Rotate(vecWrtOrigin, direction * 90 * DEG2RAD);

            // Convert translated coordinates back to cell coordinates
            int rotatedX =
                (int)(rotatedVector.x + block->originX - halfCellWidth);
            int rotatedY =
                (int)(rotatedVector.y + block->originY - halfCellWidth);

            // Check to see if cell is within bounds
            if (rotatedX * 4 + rotatedY >= 0 && rotatedX * 4 + rotatedY < 16)
                rotated[rotatedX + 4 * rotatedY] = block_get_cell(block, x, y);
        }
    }

    // 0, 1, 2, 3 represents the 4 rotated states
    block->rotation = (block->rotation + direction + 4) % 4;

    // Copy the rotated data back to the block
    memcpy(block->data, rotated, sizeof(int) * 16);
}

void block_render(Block *block, Texture2D tileSpriteSheet, float opacity) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int cell = block_get_cell(block, i, j);
            if (cell == 0)
                continue;
            // DrawRectangle((block->x + i) * 30, (block->y + j) * 30, 30, 30,
            //               colors[block_get_cell(block, i, j) - 1]);
            DrawTexturePro(
                tileSpriteSheet, (Rectangle){(cell - 1) * 16, 0, 16, 16},
                (Rectangle){(block->x + i) * 30, (block->y + j) * 30, 30, 30},
                (Vector2){0, 0}, 0, Fade(WHITE, opacity));
        }
    }
}