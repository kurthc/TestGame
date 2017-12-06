#pragma once

//#include "Game.h"
#include "Global.h"
#include "Math.h"

class game_offscreen_buffer
{
	// NOTE: Pixels are always 32-bits wide, Memory Order BB GG RR XX
public:
	void *Memory;
	int Width;
	int Height;
	int Pitch;
};


void ClearBuffer(game_offscreen_buffer *Buffer);
void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color);
void DrawRectangle(game_offscreen_buffer *Buffer, rectangle Rect, int32_t Color);
