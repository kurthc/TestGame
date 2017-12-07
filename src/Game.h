#pragma once

#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include <list>


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
	vec2 Location;
	vec2 Direction;
	int Color;

	//int x;
	//int y;

};

class snake
{
public:
	std::list<snake_segment> Segments;
	float Speed;
	int Color;
	float Timer;

	void snake::SetDirection(int x, int y);
	void snake::SetDirection(vec2 Direction);
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



game_map *CreateBlankMap(int Width, int Height);
void GameStateInitialize(game_state *GameState);
void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void SetCheckerboardMap(game_map *GameMap);
void ProcessSnake(game_state *GameState);


void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
rectangle ConvertMapTileToDisplayRectangle(rectangle r, int MaxX, int MaxY, int x, int y);
rectangle ConvertMapTileToDisplayRectangle(float Left, float Top, float Width, float Height, int MaxX, int MaxY, int x, int y);
