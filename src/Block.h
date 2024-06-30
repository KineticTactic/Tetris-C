#ifndef BLOCK_H
#define BLOCK_H

#include <raylib.h>

typedef struct {
    int x, y;
    int rotation;
    float originX, originY;
    int type;
    int data[16];
} Block;

Block CreateBlock(int id);
Block CreateRandomBlock();
int GetBlockCell(Block *block, int x, int y);
void RotateBlock(Block *block, int direction);
void RenderBlock(Block *block, Texture2D tileSpriteSheet, float opacity);

#endif