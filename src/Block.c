#include "Block.h"

#include <stdio.h>
#include <string.h>
#include "raymath.h"
#include "Colors.h"

static const Block blockTypes[] = {
	{0, 0, 2.0f, 2.0f, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 1.5f, 1.5f, {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 1.5f, 1.5f, {0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 2.0f, 2.0f, {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0}},
	{0, 0, 1.5f, 1.5f, {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 1.5f, 1.5f, {0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
	{0, 0, 1.5f, 1.5f, {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
};

Block CreateBlock(int id)
{
	if (id < 0 || id >= 7)
		id = 0;

	Block block;
	memcpy(&block, &blockTypes[id], sizeof(Block));
	block.x = 3;
	block.y = 2;
	return block;
}

Block CreateRandomBlock()
{
	return CreateBlock(GetRandomValue(0, 6));
}

int GetBlockCell(Block *block, int x, int y)
{
	if (x >= 0 && x < 4 && y >= 0 && y < 4)
		return block->data[x + y * 4];
	return 0;
}

void RotateBlock(Block *block, int direction)
{
	int rotated[16] = {0};

	double blockWidth = 0.5;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			double xWrtOrigin = x - block->originX + blockWidth;
			double yWrtOrigin = y - block->originY + blockWidth;

			Vector2 vecWrtOrigin = {xWrtOrigin, yWrtOrigin};
			Vector2 rotatedVector = Vector2Rotate(vecWrtOrigin, direction * 90 * DEG2RAD);

			int newX = (int)(rotatedVector.x + block->originX - blockWidth);
			int newY = (int)(rotatedVector.y + block->originY - blockWidth);
			printf("(%f, %f), (%f, %f), %d, %d\n", xWrtOrigin, yWrtOrigin, -yWrtOrigin, xWrtOrigin, newX, newY);
			if (newX * 4 + newY >= 0 && newX * 4 + newY < 16)
				rotated[newX + 4 * newY] = GetBlockCell(block, x, y);
		}
	}
	memcpy(block->data, rotated, sizeof(int) * 16);
}

void RenderBlock(Block *block)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (GetBlockCell(block, i, j) == 0)
				continue;
			DrawRectangle((block->x + i) * 30, (block->y + j) * 30, 30, 30, colors[GetBlockCell(block, i, j) - 1]);
		}
	}
	// Draw border for whole 4x4 grid
	DrawRectangleLines(block->x * 30, block->y * 30, 120, 120, BLACK);
}