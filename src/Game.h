#pragma once

#include "Global.h"
#include "Math.h"

#define MAX_PROJECTILES 10

struct rect
{
	int x;
	int y;
	int Width;
	int Height;
};

struct floatrect
{
	float x;
	float y;
	float Width;
	float Height;
};

class hero
{
	public:
		Vector2 Position;
		int Width;
		int Height;
		int Color;
		float Speed;
		Vector2 HeroDirection;
		//Direction HeroDirection;
};

//class projectile
//{
//	Vector2 Position;
//	Vector2 Velocity;
//
//};
//
//class projectiles
//{
//	projectile Projectile[MAX_PROJECTILES];
//	int ProjectileCount;
//};

class game_map
{
	public:
		int Width;
		int Height;
		void *Bytes;
};

class game_state
{
	public:
		hero Hero;
		//projectile Projectiles[MAX_PROJECTILES];
		int GameDisplayLeft = 50;
		int GameDisplayTop = 50;
		int GameDisplayWidth = 600;
		int GameDisplayHeight = 600;

		game_map *GameMap;
};

class game_offscreen_buffer
{
	// NOTE: Pixels are always 32-bits wide, Memory Order BB GG RR XX
	public:
		void *Memory;
		int Width;
		int Height;
		int Pitch;
};



//global_variable int GameAreaWidth = 1000;
//global_variable int GameAreaHeight = 600;


game_map *CreateBlankMap(int Width, int Height);
void GameStateInitialize(game_state *GameState);
void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
internal void ClearBuffer(game_offscreen_buffer *Buffer);
internal void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color);
void SetCheckerboardMap(game_map *GameMap);