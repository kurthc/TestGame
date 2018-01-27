#pragma once

#include <iostream>
#include <list>
#include <cmath>
#include "Global.h"
#include "Math.h"

//class pellet;
//class game_map;
//class game_round_state;
class game_state;
class snake;
class snake_segment;
//class game_offscreen_buffer;

class pellet
{
public:
	vec2 Location;
	int Color;
	~pellet()
	{
		// Just making sure these get destroyed.
		//std::cout << "pellet destroyed" << std::endl;
	}
	rectangle HitRectangle();
};

class game_map
{
public:
	int Width;
	int Height;
	void *Bytes;
	
	game_map(int Width, int Height);
};


class game_offscreen_buffer
{
	// NOTE: Pixels are always 32-bits wide, Memory Order BB GG RR XX
public:
	void *Memory;
	int TotalWidth;
	int TotalHeight;
	intrectangle MapRegionTotal;		// The part of the game screen reserved for a game map.
	intrectangle MapRegionInUse;		// The part of the game screen actually used by the game map.
	int MapBorderThickness = 5;
	int MapBorderColor = RGB(1, 0, 1);

	game_offscreen_buffer(int TotalWidth, int TotalHeight);
	void ClearBuffer();
	void DrawRectangle(int X, int Y, int Width, int Height, int32_t Color);
	void DrawRectangle(float Left, float Top, float Width, float Height, int32_t Color);
	void DrawRectangle(intrectangle Rect, int32_t Color);
	vec2 MapToDisplayCoordinates(float x, float y, game_map *GameMap);
	rectangle MapToDisplayRectangle(float x, float y, float Width, float Height, game_map *GameMap);
	
	//TODO: Should we have a pointer to GameState in this object so I can remove all these parameters?
	void DrawBorder(game_state *GameState);
	void DrawSnake(game_state *GameState);
	void DrawMap(game_state *GameState);
	void DrawScore(game_state *GameState);
	void RenderBuffer(game_state *GameState);
	void DrawDebugOverLay(game_state *GameState);
};




class game_round_state
{
public:
	snake* Snake;
	int Score;

	float NewPelletTimer;
	std::list<pellet> Pellets;

	game_state* GameState;

	game_round_state();
	game_round_state(game_state* GameState);
	void AddPellet();
};

class game_window_regions
{
public:
	rectangle ActionRegion;
	rectangle ScoreRegion;

	game_window_regions() : ActionRegion{10,10,100,100}, ScoreRegion{10,10,20,20} { };
};

class game_state
{
public:
	game_round_state CurrentRound;
	
	bool IsGameOver;
	bool DebugBufferMode = false;

	game_map* GameMap;  //TODO: This doesn't need to be a pointer
	
	game_offscreen_buffer *Buffer;
	keys_down *KeysDown;

	game_state(game_offscreen_buffer* Buffer);
	void ProcessInput(keys_down *KeysDown);
	void ProcessSnake(snake *Snake);
	void ProcessTimers();
	void GameStateProcess(keys_down *KeysDown, game_offscreen_buffer *Buffer);
};



class snake_segment
{
public:
	vec2 Location;
	vec2 Direction;
	int Color;
	snake* Snake;
	float IntangibleTimer;  //counts down. Segment cannot cause a collision until it hits zero.

	snake_segment(vec2 Location, vec2 Direction, snake* Snake);
	vec2 RealLocation();
	rectangle HitRectangle();
};

class snake
{
public:
	std::list<snake_segment> Segments;

	float Speed;
	float Timer;      // The timer counts up to 1. When it gets there, move the snake.
	vec2 Direction;

	snake(int Length, vec2 InitialPosition, vec2 InitialDirection);
	void snake::SetDirection(float x, float y);
	void snake::SetDirection(vec2 Direction);
	void snake::AddSegments(int NewSegmentCount);
	int snake::GetColor(int SegmentNumber);
};





//void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
//void ProcessTimers(game_state *GameState);

