#pragma once

//#include "Game.h"
#include "Global.h"
#include "Math.h"

class game_offscreen_buffer
{
	// NOTE: Pixels are always 32-bits wide, Memory Order BB GG RR XX
public:
	void *Memory;
	int TotalWidth;
	int TotalHeight;
	intrectangle MapRegionTotal;		// The part of the game screen reserved for a game map.
	intrectangle MapRegionInUse;		// The part of the game screen actually used by the game map.
	int MapBorderThickness = 5;
	int MapBorderColor = HMRGB(255, 0, 255);

	game_offscreen_buffer(int TotalWidth, int TotalHeight);
};


void ClearBuffer(game_offscreen_buffer *Buffer);
void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color);
void DrawRectangle(game_offscreen_buffer *Buffer, intrectangle Rect, int32_t Color);
