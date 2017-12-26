#include "Game.h"



void GameStateInitialize(game_state *GameState, game_offscreen_buffer *Buffer)
{
	// Create the game map.
	GameState->GameMap = CreateBlankMap(30, 20);
	
	// Create the snake
	vec2 InitialPosition = { 2,2 };
	vec2 InitialDirection = { 1,0 };
	GameState->Snake = new snake(2, InitialPosition, InitialDirection);

	Buffer->MapRegionInUse.x = Buffer->MapRegionTotal.x;
	Buffer->MapRegionInUse.y = Buffer->MapRegionTotal.y;
	Buffer->MapRegionInUse.Height = Buffer->MapRegionTotal.Height;
	Buffer->MapRegionInUse.Width = (float)Buffer->MapRegionTotal.Height / GameState->GameMap->Height * GameState->GameMap->Width;

	GameState->NewPelletTimer = 3;

}

game_map *CreateBlankMap(int Width, int Height)
{
	game_map *GameMap = new game_map();
	GameMap->Width = Width;
	GameMap->Height = Height;
	GameMap->Bytes = new uint8[Width * Height];
	for (int i = 0; i < Width*Height; i++)
	{
		*((uint8 *)(GameMap->Bytes) + i) = 0;
	}
	return GameMap;
}

void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *GameBuffer)
{
	ProcessInput(GameState, KeysDown);

	ProcessTimers(GameState);
	ProcessSnake(GameState->Snake);

	snake_segment *SnakeHead = &(GameState->Snake->Segments.front());

	std::list<pellet>::iterator it = GameState->Pellets.begin();
	while (it != GameState->Pellets.end())
	{
		if (it->Location.X == SnakeHead->Location.X && it->Location.Y == SnakeHead->Location.Y)
		{
			// Snake head is on a pellet. Clear the pellet. (This works because the parameters are
			// evaluated before the function call.)
			GameState->Pellets.erase(it++);
			GameState->Snake->AddSegments(3);
		}
		else
		{
			it++;
		}
	}

}

void ProcessInput(game_state *GameState, keys_down *KeysDown)
{
	vec2 NewDirection;
	bool DirectionChanged = false;

	if (KeysDown->Left)
	{
		NewDirection = -UnitVectorX;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(-UnitVectorX);
	}
	if (KeysDown->Right)
	{
		NewDirection = UnitVectorX;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(UnitVectorX);
	}
	if (KeysDown->Up)
	{
		NewDirection = -UnitVectorY;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(-UnitVectorY);
	}
	if (KeysDown->Down)
	{
		NewDirection = UnitVectorY;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(UnitVectorY);
	}
	if (KeysDown->Space)
	{
		//GameState->Snake.AddSegments(1);
	}

	if (DirectionChanged)
	{
		GameState->Snake->SetDirection(NewDirection);
	}
}

void ProcessTimers(game_state *GameState)
{
	GameState->NewPelletTimer -= 1.0f / 30;
	if (GameState->NewPelletTimer <= 0)
	{
		AddPellet(GameState);
		GameState->NewPelletTimer = 10;
	}
}

vec2 MapToDisplayCoordinates(float x, float y, game_state *GameState, game_offscreen_buffer *Buffer)
{
	float NewX = x / GameState->GameMap->Width * Buffer->MapRegionInUse.Width + Buffer->MapRegionInUse.x;
	float NewY = y / GameState->GameMap->Height * Buffer->MapRegionInUse.Height + Buffer->MapRegionInUse.y;
	return vec2(NewX, NewY);
}

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
	rectangle output = { NewV1.X, NewV1.Y, NewV2.X-NewV1.X, NewV2.Y - NewV1.Y };
	return output;
}

intrectangle ConvertMapTileToDisplayRectangle(intrectangle r, int MaxX, int MaxY, int x, int y)
{
	return ConvertMapTileToDisplayRectangle(r.x, r.y, r.Width, r.Height, MaxX, MaxY, x, y);
}

intrectangle ConvertMapTileToDisplayRectangle(int DisplayRegionLeft, int DisplayRegionTop, int DisplayRegionWidth, int DisplayRegionHeight,
	int MapMaxX, int MapMaxY, int MapX, int MapY)
{
	intrectangle Rect;

	float Left = (float)MapX / MapMaxX * DisplayRegionWidth + DisplayRegionLeft;
	float Right = (float)(MapX+1) / MapMaxX * DisplayRegionWidth + DisplayRegionLeft;
	float Top = (float)MapY / MapMaxY * DisplayRegionHeight + DisplayRegionTop;
	float Bottom = (float)(MapY+1) / MapMaxY * DisplayRegionHeight + DisplayRegionTop;

	Rect.x = (int)Left;
	Rect.y = (int)Top;
	Rect.Width = (int)Right - (int)Left;
	Rect.Height = (int)Bottom - (int)Top;

	return Rect;
}

void DrawMap(game_state *GameState, game_offscreen_buffer *Buffer)
{
	game_map *GameMap = GameState->GameMap;
	for (int y = 0; y < GameMap->Height; y++)
	{
		for (int x = 0; x < GameMap->Width; x++)
		{
			intrectangle Tile = ConvertMapTileToDisplayRectangle(Buffer->MapRegionInUse, GameMap->Width, GameMap->Height, x, y);
			DrawRectangle(Buffer, Tile.x, Tile.y, 2, 2, RGB(.5, .5, .5));
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

	DrawRectangle(Buffer, Outer, Color);
	DrawRectangle(Buffer, Inner, RGB(0.0, 0.0, 0.0));
}

void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
{
	snake *Snake = GameState->Snake;
	game_map *GameMap = GameState->GameMap;
	std::list<snake_segment> Segments = Snake->Segments;

	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); it++)
	{
		vec2 DrawLocation = it->Location + it->Direction * Snake->Timer;
		//vec2 DrawLocation = it->Location;
		//intrectangle Rec = ConvertMapTileToDisplayRectangle(Buffer->MapRegionInUse, GameMap->Width, GameMap->Height, DrawLocation.X, DrawLocation.Y);
		//DrawRectangle(Buffer, Rec, it->Color);
		rectangle Rec = MapToDisplayRectangle(DrawLocation.X, DrawLocation.Y, 1, 1, GameState, Buffer);
		DrawRectangle(Buffer, Rec.x, Rec.y, Rec.Width, Rec.Height, it->Color);

	}
}

//void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
//{
//	std::list<snake_segment> Segments = GameState->Snake->Segments;
//
//	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); it++)
//	{
//		vec2 Location = it->Location;
//		intrectangle Rec = ConvertMapTileToDisplayRectangle(Buffer->MapRegionInUse, GameState->GameMap->Width, GameState->GameMap->Height, Location.X, Location.Y);
//		DrawRectangle(Buffer, Rec, it->Color);
//		
//	}
//}

void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	ClearBuffer(Buffer);
	DrawBorder(GameState, Buffer);
	DrawMap(GameState, Buffer);

	//hero *Hero = &(GameState->Hero);
	//DrawRectangle(Buffer, (int)Hero->Position.X, (int)Hero->Position.Y, Hero->Width, Hero->Height, Hero->Color);

	for (std::list<pellet>::iterator it = GameState->Pellets.begin(); it != GameState->Pellets.end(); it++)
	{
		intrectangle Rec = ConvertMapTileToDisplayRectangle(Buffer->MapRegionInUse, GameState->GameMap->Width, GameState->GameMap->Height, it->Location.X, it->Location.Y);
		DrawRectangle(Buffer, Rec, it->Color);
	}

	DrawSnake(GameState, Buffer);

}


void AddPellet(game_state *GameState)
{
	int x = (rand() % GameState->GameMap->Width);
	int y = (rand() % GameState->GameMap->Height);
	pellet *Pellet = new pellet();
	Pellet->Location.SetXY(x, y);
	Pellet->Color = RGB(1.0, 1.0, 1.0);
	GameState->Pellets.push_back(*Pellet);
}

