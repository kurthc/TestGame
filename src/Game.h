#pragma once

#include "Global.h"
#include "Math.h"
#include <list>

struct rectangle
{
	float x;
	float y;
	float Width;
	float Height;
};

class hero
{
	public:
		vec2 Position;
		int Width;
		int Height;
		int Color;
		float Speed;
		vec2 HeroDirection;
};

class snake_segment
{
public:
	int x;
	int y;

};

class snake
{
public:
	std::list<snake_segment> Segments;
};

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
	snake Snake;

	rectangle GameboardDisplayRegion = { 50, 50, 600, 600 };
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


game_map *CreateBlankMap(int Width, int Height);
void GameStateInitialize(game_state *GameState);
void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
internal void ClearBuffer(game_offscreen_buffer *Buffer);
internal void DrawRectangle(game_offscreen_buffer *Buffer, int X, int Y, int Width, int Height, int32_t Color);
internal void DrawRectangle(game_offscreen_buffer *Buffer, rectangle Rect, int32_t Color);
void SetCheckerboardMap(game_map *GameMap);
rectangle ConvertMapTileToDisplayRectangle(rectangle r, int MaxX, int MaxY, int x, int y);
rectangle ConvertMapTileToDisplayRectangle(float Left, float Top, float Width, float Height, int MaxX, int MaxY, int x, int y);
