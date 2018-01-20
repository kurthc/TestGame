#include "Buffer.h"

game_offscreen_buffer::game_offscreen_buffer(int TotalWidth, int TotalHeight)
{
	this->TotalWidth = TotalWidth;
	this->TotalHeight = TotalHeight;
	this->MapBorderThickness = 5;
	this->MapBorderColor = RGB(1.0, 0.0, 1.0);
	this->MapRegionTotal.x = 50;
	this->MapRegionTotal.y = 50;
	this->MapRegionTotal.Width = TotalWidth - 100;
	this->MapRegionTotal.Height = TotalHeight - 100;
}

void game_offscreen_buffer::ClearBuffer()
{
	int32_t *Pixel = (int32_t*)this->Memory;

	for (int i = 0; i < this->TotalWidth * this->TotalHeight; i++)
	{
		Pixel[i] = 0;
	}
}

void game_offscreen_buffer::DrawRectangle(int Left, int Top, int Width, int Height, int32_t Color)
{
	int32_t* Pixel = (int32_t*)this->Memory;

	for (int y = MAX(Top, 0); y < Top + Height && y < this->TotalHeight; y++)
	{
		for (int x = MAX(Left, 0); x < Left + Width && x < this->TotalWidth; x++)
		{
			Pixel[y*this->TotalWidth + x] = Color;
		}
	}
}

void game_offscreen_buffer::DrawRectangle(float Left, float Top, float Width, float Height, int32_t Color)
{
	this->DrawRectangle((int)Left, (int)Top, (int)Width, (int)Height, Color);
}

void game_offscreen_buffer::DrawRectangle(intrectangle Rect, int32_t Color)
{
	this->DrawRectangle(Rect.x, Rect.y, Rect.Width, Rect.Height, Color);
}

//
//void DrawCircle(game_offscreen_buffer *Buffer, int Left, int Top, int Width, int Height, int32_t Color)
//{
//	float CenterX = (float)Left + Width / 2.0;
//	float CenterY = (float)Top + Height / 2.0;
//
//	int32_t* Pixel = (int32_t*)Buffer->Memory;
//
//	for (int y = MAX(Top, 0); y < Top + Height && y < Buffer->TotalHeight; y++)
//	{
//		for (int x = MAX(Left, 0); x < Left + Width && x < Buffer->TotalWidth; x++)
//		{
//			Pixel[y*Buffer->TotalWidth + x] = Color;
//		}
//	}
//}
