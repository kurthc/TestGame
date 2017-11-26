#pragma once

#include "Global.h"

#define MAX_PROJECTILES 10

enum Direction {DirectionRight, DirectionUp, DirectionLeft, DirectionDown};

class hero
{
	public:
	float X;
	float Y;
	int Width;
	int Height;
	int Color;
	float Speed;
	Direction HeroDirection;
};

struct projectile
{
	float X;
	float Y;
	float VelocityX;
	float VelocityY;

};

struct projectiles
{
	struct projectile Projectile[MAX_PROJECTILES];
	int ProjectileCount;
};

struct game_state
{
	struct hero Hero;
	struct projectile Projectiles[MAX_PROJECTILES];
};

struct game_offscreen_buffer
{
	// NOTE(casey): Pixels are alwasy 32-bits wide, Memory Order BB GG RR XX
	void *Memory;
	int Width;
	int Height;
	int Pitch;
};



global_variable int GameAreaWidth = 1000;
global_variable int GameAreaHeight = 600;



void GameStateInitialize(game_state *GameState);
void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
internal void ClearBuffer(game_offscreen_buffer *Buffer);
internal void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color);