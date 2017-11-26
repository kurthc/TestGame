
#include "Game.h"
//#include "win32_handmade.h"


void GameStateInitialize(game_state *GameState)
{
	GameState->Hero.X = 100;
	GameState->Hero.Y = 100;
	GameState->Hero.Width = 20;
	GameState->Hero.Height = 20;
	GameState->Hero.Color = HMRGB(255,0,0);
	GameState->Hero.Speed = 8;
}

void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *GameBuffer)
{
	//win32_offscreen_buffer Buffer = GlobalBackBuffer;
	
	hero Hero = GameState->Hero;
	
	if (KeysDown->Left)
	{
		GameState->Hero.X -= GameState->Hero.Speed;
		GameState->Hero.HeroDirection = DirectionLeft;
	}
	if (KeysDown->Right)
	{
		GameState->Hero.X += GameState->Hero.Speed;
		GameState->Hero.HeroDirection = DirectionRight;
	}
	if (KeysDown->Up)
	{
		GameState->Hero.Y -= GameState->Hero.Speed;
		GameState->Hero.HeroDirection = DirectionUp;
	}
	if (KeysDown->Down)
	{
		GameState->Hero.Y += GameState->Hero.Speed;
		GameState->Hero.HeroDirection = DirectionDown;
	}
	if (KeysDown->Space)
	{
		GameState->Hero.Color = 10000 * (int)GameState->Hero.HeroDirection;
		//GameState->Hero.HeroDirection = DirectionDown;
	}

}

void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	ClearBuffer(Buffer);
	//DrawRectangle(&GlobalBackbuffer, 100 + XOffset, 100 + YOffset, 100, 100, HMRGB(255, 0, 0));
	hero *Hero = &(GameState->Hero);
	DrawRectangle(Buffer, Hero->X, Hero->Y, Hero->Width, Hero->Height, Hero->Color);

}


internal void ClearBuffer(game_offscreen_buffer *Buffer)
{
	int32_t *Pixel = (int32_t*)Buffer->Memory;

	for (int i = 0; i < Buffer->Width * Buffer->Height; i++)
	{
		//int32_t Red = RGB(255,0,0);
		//int32_t Green = RGB(0, 255, 0);
		//int32_t Blue = RGB(0, 0, 255);

		Pixel[i] = 0;   //RGB(0,0,0);

	}
}

internal void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color)
{

	int32_t* Pixel = (int32_t*)Buffer->Memory;
	
	(X > Y) ? X : Y;
	for (int j = MAX(Y, 0); j < Y + Height && j < Buffer->Height; j++)
	{
		for (int i = MAX(X, 0); i < X + Width && i < Buffer->Width; i++)
		{
			Pixel[j*Buffer->Width + i] = Color;
		}
	}
}