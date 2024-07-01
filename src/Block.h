#ifndef BLOCK_H
#define BLOCK_H

#include <raylib.h>

#include "data.h"

// All the block type data is stored in data.h
typedef struct {
    int x, y;
    int rotation;
    float originX, originY;
    int type;
    int data[16];
} Block;

Block block_create(enum BlockTypes type);
Block block_create_random();
int block_get_cell(Block *block, int x, int y);
void block_rotate(Block *block, int direction);
void block_render(Block *block, Texture2D tileSpriteSheet, float opacity);

#endif