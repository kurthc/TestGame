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
	vec2 LastSegmentLocation = { 0,0 };
	for (int i = 0; i < 9; i++)
	{
		snake_segment *ss = new snake_segment();
		ss->Location.SetXY(6, 15-i);
		ss->Color = HMRGB(0, 255 - i * 30, 0);
		if (i == 0)
		{
			ss->Direction = UnitVectorY;
		}
		else
		{
			ss->Direction = LastSegmentLocation - ss->Location;
		}
		LastSegmentLocation = ss->Location;
		GameState->Snake.Segments.push_back(*ss);
	}
	GameState->Snake.Color = HMRGB(0, 255, 0);
	GameState->Snake.Speed = 5;
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

	ProcessSnake(GameState);
}

void ProcessSnake(game_state *GameState)
{
	snake *Snake = &(GameState->Snake);

	std::list<snake_segment> *Segments = &((*Snake).Segments);
	(*Snake).Timer += (*Snake).Speed / 30;   // FPS?
	if ((*Snake).Timer >= 1)
	{
		vec2 LastLocation = { 0,0 };
		for (std::list<snake_segment>::iterator it = (*Segments).begin(); it != (*Segments).end(); it++)
		{
			(*it).Location = (*it).Location + (*it).Direction;
			if (it != (*Segments).begin())
			{
				(*it).Direction = LastLocation - (*it).Location;
			}
			LastLocation = (*it).Location;
		}
		(*Snake).Timer = 0;
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
			int32 Color;

			// Draw a cool gradient
			//Color = 10000 * (Tile.x + Tile.y);
			//DrawRectangle(Buffer, Tile, Color);

			// For now, draw 0 = black, 1 = green
			//Color = ((uint8 *)GameMap->Bytes)[y*GameMap->Height + x] * (255 << 8);          //HMRGB(0,255,0);
			//DrawRectangle(Buffer, Tile, Color);

			DrawRectangle(Buffer, Tile.x, Tile.y, 2, 2, HMRGB(127, 127, 127));

			//DrawRectangle(Buffer, (int)Tile.x, (int)Tile.y, (int)Tile.Width, (int)Tile.Height, 10000*(Tile.x + Tile.y));
			//DrawRectangle(Buffer, (int)Tile.x, (int)Tile.y, (int)Tile.Width, (int)Tile.Height, Color);
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
		vec2 Location = (*it).Location;
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


void snake::SetDirection(int x, int y)
{
	//this->Direction.SetXY(x, y);
	this->Segments.front().Direction.SetXY(x, y);
	
}

void snake::SetDirection(vec2 Direction)
{
	this->SetDirection(Direction.X, Direction.Y);
}


