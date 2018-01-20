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

void game_offscreen_buffer::RenderBuffer(game_state *GameState)
{
	game_offscreen_buffer* Buffer = GameState->Buffer;
	Buffer->ClearBuffer();
	DrawBorder(GameState, Buffer);
	DrawMap(GameState, Buffer);

	// Draw the pellets.
	for (std::list<pellet>::iterator it = GameState->CurrentRound.Pellets.begin(); it != GameState->CurrentRound.Pellets.end(); it++)
	{
		rectangle Rec = MapToDisplayRectangle(it->Location.X, it->Location.Y, 1.0, 1.0, GameState, Buffer);
		Buffer->DrawRectangle(Rec.x, Rec.y, Rec.Width, Rec.Height, it->Color);
	}

	DrawSnake(GameState, Buffer);
}



//Map Coordinates go from:      0 to GameState->GameMap->Width
//                    and:      0 to GameState->GameMap->Height
//Display Coordinates go from:  Buffer->MapRegionInUse.x to Buffer->MapRegionInUse.x + Buffer->MapRegionInUse.Width
//                        and:  Buffer->MapRegionInUse.y to Buffer->MapRegionInUse.y + Buffer->MapRegionInUse.Height


vec2 MapToDisplayCoordinates(float x, float y, game_state *GameState, game_offscreen_buffer *Buffer)
{
	float NewX = x / GameState->GameMap->Width * Buffer->MapRegionInUse.Width + Buffer->MapRegionInUse.x;
	float NewY = y / GameState->GameMap->Height * Buffer->MapRegionInUse.Height + Buffer->MapRegionInUse.y;
	return vec2(NewX, NewY);
}

//vec2 DisplayToMapCoordinates(float x, float y, game_state *GameState, game_offscreen_buffer *Buffer)
//{
//	//TODO: Write if needed.
//}

//rectangle MapToDisplayRectangle(rectangle r, game_state *GameState, game_offscreen_buffer *Buffer)
//{
//	
//}

rectangle MapToDisplayRectangle(float x, float y, float Width, float Height, game_state *GameState, game_offscreen_buffer *Buffer)
{
	float Right = x + Width;
	float Bottom = y + Height;
	vec2 NewV1 = MapToDisplayCoordinates(x, y, GameState, Buffer);
	vec2 NewV2 = MapToDisplayCoordinates(Right, Bottom, GameState, Buffer);
	rectangle output = {NewV1.X, NewV1.Y, NewV2.X - NewV1.X, NewV2.Y - NewV1.Y};
	return output;
}

void DrawMap(game_state *GameState, game_offscreen_buffer *Buffer)
{
	game_map *GameMap = GameState->GameMap;
	for (int y = 0; y < GameMap->Height; y++)
	{
		for (int x = 0; x < GameMap->Width; x++)
		{
			vec2 UpperLeftCorner = MapToDisplayCoordinates((float)x, (float)y, GameState, Buffer);
			Buffer->DrawRectangle(UpperLeftCorner.X, UpperLeftCorner.Y, 2.0, 2.0, RGB(.5, .5, .5));
		}
	}
}

void DrawBorder(game_state *GameState, game_offscreen_buffer *Buffer)
{
	//rectangle Inner = GameState->GameboardDisplayRegion;
	intrectangle Inner = Buffer->MapRegionInUse;
	intrectangle Outer = {};
	int BorderWidth = Buffer->MapBorderThickness;
	int Color = Buffer->MapBorderColor;
	Outer.x = Inner.x - BorderWidth;
	Outer.y = Inner.y - BorderWidth;
	Outer.Width = Inner.Width + 2 * BorderWidth;
	Outer.Height = Inner.Height + 2 * BorderWidth;

	Buffer->DrawRectangle(Outer, Color);
	Buffer->DrawRectangle(Inner, RGB(0.0, 0.0, 0.0));
}

void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
{
	snake *Snake = GameState->CurrentRound.Snake;
	game_map *GameMap = GameState->GameMap;
	std::list<snake_segment> Segments = Snake->Segments;

	// Iterate through the segmeents in reverse order
	for (std::list<snake_segment>::reverse_iterator it = Segments.rbegin(); it != Segments.rend(); it++)
	{
		vec2 DrawLocation = it->Location + it->Direction * Snake->Timer;

		rectangle Rec = MapToDisplayRectangle(DrawLocation.X, DrawLocation.Y, 1, 1, GameState, Buffer);
		Buffer->DrawRectangle(Rec.x, Rec.y, Rec.Width, Rec.Height, it->Color);
	}

}
