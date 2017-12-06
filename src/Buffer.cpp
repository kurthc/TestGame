#include "Buffer.h"


void ClearBuffer(game_offscreen_buffer *Buffer)
{
	int32_t *Pixel = (int32_t*)Buffer->Memory;

	for (int i = 0; i < Buffer->Width * Buffer->Height; i++)
	{
		Pixel[i] = 0;
	}
}

void DrawRectangle(game_offscreen_buffer *Buffer, int Left, int Top, int Width, int Height, int32_t Color)
{
	int32_t* Pixel = (int32_t*)Buffer->Memory;

	for (int y = MAX(Top, 0); y < Top + Height && y < Buffer->Height; y++)
	{
		for (int x = MAX(Left, 0); x < Left + Width && x < Buffer->Width; x++)
		{
			Pixel[y*Buffer->Width + x] = Color;
		}
	}
}

void DrawRectangle(game_offscreen_buffer *Buffer, rectangle Rect, int32_t Color)
{
	DrawRectangle(Buffer, Rect.x, Rect.y, Rect.Width, Rect.Height, Color);
}
