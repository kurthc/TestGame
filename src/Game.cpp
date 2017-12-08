#include "Game.h"

void GameStateInitialize(game_state *GameState)
{
	GameState->Hero.Position.SetXY(100, 100);
	GameState->Hero.Width = 20;
	GameState->Hero.Height = 20;
	GameState->Hero.Color = HMRGB(255, 0, 0);
	GameState->Hero.Speed = 2;

	GameState->GameMap = CreateBlankMap(50, 50);
	SetCheckerboardMap(GameState->GameMap);

	//snake_segment *LastSegment;
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

}

void SetCheckerboardMap(game_map *GameMap)
{
	int Width = GameMap->Width;
	int Height = GameMap->Height;
	uint8 *Bytes = (uint8 *)GameMap->Bytes;

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			Bytes[y*Width + x] = (uint8)((x + y) % 2);
		}
	}
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
	
	hero Hero = GameState->Hero;
	
	if (KeysDown->Left)
	{
		GameState->Snake.SetDirection(-UnitVectorX);
		//GameState->Hero.HeroDirection = -UnitVectorX;
		//GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Right)
	{
		GameState->Snake.SetDirection(UnitVectorX);
		//GameState->Hero.HeroDirection = UnitVectorX;
		//GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Up)
	{
		GameState->Snake.SetDirection(-UnitVectorY);
		//GameState->Hero.HeroDirection = -UnitVectorY;
		//GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Down)
	{
		GameState->Snake.SetDirection(UnitVectorY);
		//GameState->Hero.HeroDirection = UnitVectorY;
		//GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Space)
	{
		//GameState->Hero.Color = 10000;
		//GameState->Hero.HeroDirection = DirectionDown;
	}

	//ProcessSnake(GameState);
	ProcessSnake(&(GameState->Snake));
}

//void ProcessSnake(game_state *GameState)


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

	hero *Hero = &(GameState->Hero);
	DrawRectangle(Buffer, (int)Hero->Position.X, (int)Hero->Position.Y, Hero->Width, Hero->Height, Hero->Color);

	DrawSnake(GameState, Buffer);

}




