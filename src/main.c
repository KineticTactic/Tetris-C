#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "Board.h"
#include "Block.h"

int main()
{
	const int screenWidth = 300;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Tetris");
	SetTargetFPS(60);
	// SetWindowPosition(1400, 200);

	Board board = CreateBoard(10, 20);

	// Randomize board

	Block b = CreateBlock(5);

	double lastTurn = GetTime();

	while (!WindowShouldClose())
	{
		// printf("%f\n", GetTime());
		if (IsKeyDown(KEY_LEFT) && GetTime() - lastTurn >= 0.2)
		{
			RotateBlock(&b, -1);
			lastTurn = GetTime();
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		RenderBoard(&board, screenWidth, screenHeight);
		RenderBlock(&b);
		EndDrawing();

		// printf("FPS: %d\n", GetFPS());
	}

	CloseWindow();

	DestroyBoard(&board);

	return 0;
}