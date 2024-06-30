#include "Block.h"

#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Colors.h"
#include "data.h"

Block CreateBlock(int id) {
    if (id < 0 || id >= 7)
        id = 0;

    Block block;
    memcpy(block.data, BLOCK_LAYOUTS[id], sizeof(int) * BLOCK_LAYOUT_SIZE);

    int color = GetRandomValue(1, 7);
    for (int i = 0; i < 16; i++) {
        block.data[i] *= color;
    }
    block.x = 3;
    block.y = 1;
    block.type = id;
    block.rotation = 0;
    block.originX = BLOCK_ORIGINS[id][0];
    block.originY = BLOCK_ORIGINS[id][1];
    return block;
}

Block CreateRandomBlock() {
    return CreateBlock(GetRandomValue(0, NUM_COLORS - 1));
}

int GetBlockCell(Block *block, int x, int y) {
    if (x >= 0 && x < 4 && y >= 0 && y < 4)
        return block->data[x + y * 4];
    return 0;
}

void RotateBlock(Block *block, int direction) {
    int rotated[16] = {0};

    double blockWidth = 0.5;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            double xWrtOrigin = x - block->originX + blockWidth;
            double yWrtOrigin = y - block->originY + blockWidth;

            Vector2 vecWrtOrigin = {xWrtOrigin, yWrtOrigin};
            Vector2 rotatedVector =
                Vector2Rotate(vecWrtOrigin, direction * 90 * DEG2RAD);

            int newX = (int)(rotatedVector.x + block->originX - blockWidth);
            int newY = (int)(rotatedVector.y + block->originY - blockWidth);
            // printf("(%f, %f), (%f, %f), %d, %d\n", xWrtOrigin, yWrtOrigin,
            // -yWrtOrigin, xWrtOrigin, newX, newY);
            if (newX * 4 + newY >= 0 && newX * 4 + newY < 16)
                rotated[newX + 4 * newY] = GetBlockCell(block, x, y);
        }
    }

    block->rotation = (block->rotation + direction + 4) % 4;

    memcpy(block->data, rotated, sizeof(int) * 16);
}

void RenderBlock(Block *block, Texture2D tileSpriteSheet, float opacity) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int cell = GetBlockCell(block, i, j);
            if (cell == 0)
                continue;
            // DrawRectangle((block->x + i) * 30, (block->y + j) * 30, 30, 30,
            //               colors[GetBlockCell(block, i, j) - 1]);
            DrawTexturePro(
                tileSpriteSheet, (Rectangle){(cell - 1) * 16, 0, 16, 16},
                (Rectangle){(block->x + i) * 30, (block->y + j) * 30, 30, 30},
                (Vector2){0, 0}, 0, Fade(WHITE, opacity));
        }
    }
    // Draw border for whole 4x4 grid
    // DrawRectangleLines(block->x * 30, block->y * 30, 120, 120, BLACK);
}