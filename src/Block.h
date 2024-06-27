#ifndef BLOCK_H
#define BLOCK_H

#include "raylib.h"

// #include "BlockData.h"

typedef struct
{
	int x, y;
	float originX, originY;
	int data[16];
} Block;

Block CreateBlock(int id);
Block CreateRandomBlock();
int GetBlockCell(Block *block, int x, int y);
void RotateBlock(Block *block, int direction);
void RenderBlock(Block *block);

#endif
