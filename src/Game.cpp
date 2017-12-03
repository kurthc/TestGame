
#include "Game.h"
//#include "win32_handmade.h"


void GameStateInitialize(game_state *GameState)
{
	GameState->Hero.Position.SetXY(100, 100);
	GameState->Hero.Width = 20;
	GameState->Hero.Height = 20;
	GameState->Hero.Color = HMRGB(255,0,0);
	GameState->Hero.Speed = 2;

	
	GameState->GameMap = CreateBlankMap(5, 5);
	SetCheckerboardMap(GameState->GameMap);

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
		GameState->Hero.HeroDirection = DirectionLeft;
		GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Right)
	{
		GameState->Hero.HeroDirection = DirectionRight;
		GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Up)
	{
		GameState->Hero.HeroDirection = DirectionUp;
		GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Down)
	{
		GameState->Hero.HeroDirection = DirectionDown;
		GameState->Hero.Position = GameState->Hero.Position = GameState->Hero.Position + GameState->Hero.HeroDirection * (GameState->Hero.Speed);
	}
	if (KeysDown->Space)
	{
		//GameState->Hero.Color = 10000;
		//GameState->Hero.HeroDirection = DirectionDown;
	}

}


floatrect MapToSquare(float Left, float Top, float Width, float Height, int MaxX, int MaxY, int x, int y)
{
	//int OutX, OutY;
	//int OutWidth, OutHeight;

	floatrect Rect;

	Rect.x = (float)x / MaxX * Width + Left;
	Rect.y = (float)y / MaxY * Height + Top;
	Rect.Width = Width / MaxX;
	Rect.Height = Height / MaxY;
	return Rect;
}

void DrawMap(game_state *GameState, game_offscreen_buffer *Buffer)
{
	float GameMapLeft = GameState->GameDisplayLeft;
	float GameMapTop = GameState->GameDisplayTop;
	float GameMapWidth = GameState->GameDisplayWidth;
	float GameMapHeight = GameState->GameDisplayHeight;

	game_map *GameMap = GameState->GameMap;
	for (int y = 0; y < GameMap->Height; y++)
	{
		for (int x = 0; x < GameMap->Width; x++)
		{
			floatrect Tile = MapToSquare(GameMapLeft, GameMapTop, GameMapWidth, GameMapHeight, GameMap->Width, GameMap->Height, x, y);
			int32 Color;
			//if (GameMap->Bytes[y*GameMap->Width + x])
			DrawRectangle(Buffer, (int)Tile.x, (int)Tile.y, (int)Tile.Width, (int)Tile.Height, 10000*(Tile.x + Tile.y));
		}
	}

}


void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	ClearBuffer(Buffer);

	DrawMap(GameState, Buffer);

	hero *Hero = &(GameState->Hero);


	DrawRectangle(Buffer, (int)Hero->Position.X, (int)Hero->Position.Y, Hero->Width, Hero->Height, Hero->Color);



}


internal void ClearBuffer(game_offscreen_buffer *Buffer)
{
	int32_t *Pixel = (int32_t*)Buffer->Memory;

	for (int i = 0; i < Buffer->Width * Buffer->Height; i++)
	{
		Pixel[i] = 0;   //RGB(0,0,0);
	}
}

internal void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color)
{

	int32_t* Pixel = (int32_t*)Buffer->Memory;
	
	//(X > Y) ? X : Y;
	for (int j = MAX(Y, 0); j < Y + Height && j < Buffer->Height; j++)
	{
		for (int i = MAX(X, 0); i < X + Width && i < Buffer->Width; i++)
		{
			Pixel[j*Buffer->Width + i] = Color;
		}
	}
}