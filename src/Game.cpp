#include "Game.h"

void GameStateInitialize(game_state *GameState, game_offscreen_buffer *Buffer)
{

	GameState->GameMap = CreateBlankMap(30, 20);

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
	GameState->Snake.Speed = 2;
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
			DrawRectangle(Buffer, Tile.x, Tile.y, 2, 2, HMRGB(127, 127, 127));
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
	DrawRectangle(Buffer, Inner, HMRGB(0, 0, 0));
}

void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
{
//	//snake *Snake = GameState
	std::list<snake_segment> Segments = GameState->Snake.Segments;

	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); it++)
	{
		intvec2 Location = (*it).Location;
		intrectangle Rec = ConvertMapTileToDisplayRectangle(Buffer->MapRegionInUse, GameState->GameMap->Width, GameState->GameMap->Height, Location.X, Location.Y);
		DrawRectangle(Buffer, Rec, it->Color);
		
	}
}

void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	ClearBuffer(Buffer);
	DrawBorder(GameState, Buffer);
	DrawMap(GameState, Buffer);

	//hero *Hero = &(GameState->Hero);
	//DrawRectangle(Buffer, (int)Hero->Position.X, (int)Hero->Position.Y, Hero->Width, Hero->Height, Hero->Color);

	DrawSnake(GameState, Buffer);

}




