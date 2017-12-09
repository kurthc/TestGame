#include "Buffer.h"

game_offscreen_buffer::game_offscreen_buffer(int TotalWidth, int TotalHeight)
{
	this->TotalWidth = TotalWidth;
	this->TotalHeight = TotalHeight;
	this->MapBorderThickness = 5;
	this->MapBorderColor = HMRGB(255, 0, 255);
	this->MapRegionTotal.x = 50;
	this->MapRegionTotal.y = 50;
	this->MapRegionTotal.Width = TotalWidth - 100;
	this->MapRegionTotal.Height = TotalHeight - 100;
}

void ClearBuffer(game_offscreen_buffer *Buffer)
{
	int32_t *Pixel = (int32_t*)Buffer->Memory;

	for (int i = 0; i < Buffer->TotalWidth * Buffer->TotalHeight; i++)
	{
		Pixel[i] = 0;
	}
}

void DrawRectangle(game_offscreen_buffer *Buffer, int Left, int Top, int Width, int Height, int32_t Color)
{
	int32_t* Pixel = (int32_t*)Buffer->Memory;

	for (int y = MAX(Top, 0); y < Top + Height && y < Buffer->TotalHeight; y++)
	{
		for (int x = MAX(Left, 0); x < Left + Width && x < Buffer->TotalWidth; x++)
		{
			Pixel[y*Buffer->TotalWidth + x] = Color;
		}
	}
}

void DrawRectangle(game_offscreen_buffer *Buffer, intrectangle Rect, int32_t Color)
{
	DrawRectangle(Buffer, Rect.x, Rect.y, Rect.Width, Rect.Height, Color);
}

