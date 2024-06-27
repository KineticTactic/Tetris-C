#include "Board.h"

#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "Colors.h"

Board CreateBoard(int width, int height)
{
	Board board;
	board.width = width;
	board.height = height;
	board.data = (int *)malloc(width * height * sizeof(int));
	memset(board.data, 0, width * height * sizeof(int));
	return board;
}

void DestroyBoard(Board *board)
{
	free(board->data);
	board->data = NULL;
	board->width = 0;
	board->height = 0;
}

int GetCell(Board *board, int x, int y)
{
	if (x >= 0 && x < board->width && y >= 0 && y < board->height)
		return board->data[x + y * board->width];
	return 0;
}

void SetCell(Board *board, int x, int y, int value)
{
	if (x >= 0 && x < board->width && y >= 0 && y < board->height)
	{
		board->data[x + y * board->width] = value;
	}
}

void RenderBoard(Board *board, int screenWidth, int screenHeight)
{
	int cellWidth = screenWidth / board->width;
	int cellHeight = screenHeight / board->height;

	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			if (GetCell(board, i, j) == 0)
				continue;

			DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight, colors[GetCell(board, i, j) - 1]);
		}
	}
}