#include "Game.h"

void GameStateInitialize(game_state *GameState, game_offscreen_buffer *Buffer)
{

	GameState->GameMap = CreateBlankMap(50, 50);

	Buffer->MapRegionInUse.x = Buffer->MapRegionTotal.x;
	Buffer->MapRegionInUse.y = Buffer->MapRegionTotal.y;
	Buffer->MapRegionInUse.Height = Buffer->MapRegionTotal.Height;
	Buffer->MapRegionInUse.Width = (float)Buffer->MapRegionTotal.Height / GameState->GameMap->Height * GameState->GameMap->Width;
	
	intvec2 LastSegmentLocation = { 0,0 };
	for (int i = 0; i < 19; i++)
	{
		snake_segment *ss = new snake_segment();
		ss->Location.SetXY(6, 25-i);
		ss->Color = HMRGB(0, 255 - i * 13, 0);
		if (i == 0)
		{
			ss->Direction = intUnitVectorY;
		}
		else
		{
			ss->Direction = LastSegmentLocation - ss->Location;
		}
		LastSegmentLocation = ss->Location;
		GameState->Snake.Segments.push_back(*ss);
	}
	GameState->Snake.Color = HMRGB(0, 255, 0);
	GameState->Snake.Speed = 4;
	GameState->Snake.Timer = 0;

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
	//win32_offscreen_buffer Buffer = GlobalBackBuffer;
	
	//hero Hero = GameState->Hero;
	
	if (KeysDown->Left)
	{
		GameState->Snake.SetDirection(-UnitVectorX);
	}
	if (KeysDown->Right)
	{
		GameState->Snake.SetDirection(UnitVectorX);
	}
	if (KeysDown->Up)
	{
		GameState->Snake.SetDirection(-UnitVectorY);
	}
	if (KeysDown->Down)
	{
		GameState->Snake.SetDirection(UnitVectorY);
	}
	if (KeysDown->Space)
	{
		GameState->Snake.AddSegments(1);
	}

	ProcessTimers(GameState);
	ProcessSnake(&(GameState->Snake));
}

void ProcessTimers(game_state *GameState)
{
	GameState->NewPelletTimer -= 1.0f / 30;
	if (GameState->NewPelletTimer <= 0)
	{
		//AddPellet(GameState);
		GameState->NewPelletTimer = 10;
	}
}

rectangle ConvertMapTileToDisplayRectangle(rectangle r, int MaxX, int MaxY, int x, int y)
{
	return ConvertMapTileToDisplayRectangle(r.x, r.y, r.Width, r.Height, MaxX, MaxY, x, y);
}

rectangle ConvertMapTileToDisplayRectangle(float DisplayRegionLeft, float DisplayRegionTop, float DisplayRegionWidth, float DisplayRegionHeight,
	int MapMaxX, int MapMaxY, int MapX, int MapY)
{
	rectangle Rect;

	Rect.x = (float)MapX / MapMaxX * DisplayRegionWidth + DisplayRegionLeft;
	Rect.y = (float)MapY / MapMaxY * DisplayRegionHeight + DisplayRegionTop;
	Rect.Width = DisplayRegionWidth / MapMaxX;
	Rect.Height = DisplayRegionHeight / MapMaxY;
	return Rect;
}

void DrawMap(game_state *GameState, game_offscreen_buffer *Buffer)
{
	game_map *GameMap = GameState->GameMap;
	for (int y = 0; y < GameMap->Height; y++)
	{
		for (int x = 0; x < GameMap->Width; x++)
		{
			rectangle Tile = ConvertMapTileToDisplayRectangle(GameState->GameboardDisplayRegion, GameMap->Width, GameMap->Height, x, y);
			DrawRectangle(Buffer, Tile.x, Tile.y, 2, 2, HMRGB(127, 127, 127));
		}
	}
}

void DrawBorder(game_state *GameState, game_offscreen_buffer *Buffer, int BorderWidth, int Color)
{
	rectangle Inner = GameState->GameboardDisplayRegion;
	rectangle Outer = {};
	//int BorderWidth = 5;
	Outer.x = Inner.x - BorderWidth;
	Outer.y = Inner.y - BorderWidth;
	Outer.Width = Inner.Width + 2 * BorderWidth;
	Outer.Height = Inner.Height + 2 * BorderWidth;

	DrawRectangle(Buffer, Outer, Color);
	DrawRectangle(Buffer, Inner, HMRGB(0, 0, 0));
}

void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
{
//	//snake *Snake = GameState
	std::list<snake_segment> Segments = GameState->Snake.Segments;

	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); it++)
	{
		intvec2 Location = (*it).Location;
		rectangle Rec = ConvertMapTileToDisplayRectangle(GameState->GameboardDisplayRegion, GameState->GameMap->Width, GameState->GameMap->Height, Location.X, Location.Y);
		DrawRectangle(Buffer, Rec, it->Color);
		
	}
}

void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	ClearBuffer(Buffer);
	DrawBorder(GameState, Buffer, 5, HMRGB(255, 0, 255));
	DrawMap(GameState, Buffer);

	//hero *Hero = &(GameState->Hero);
	//DrawRectangle(Buffer, (int)Hero->Position.X, (int)Hero->Position.Y, Hero->Width, Hero->Height, Hero->Color);

	DrawSnake(GameState, Buffer);

}




