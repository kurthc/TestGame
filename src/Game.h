#pragma once

#include <iostream>
#include <list>
#include <cmath>
#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include "Snake.h"

class pellet;
class game_map;
class game_round_state;
class game_state;

class pellet
{
public:
	vec2 Location;
	int Color;
};

class game_map
{
public:
	int Width;
	int Height;
	void *Bytes;
	
	game_map(int Width, int Height);
};

class game_round_state
{
public:
	snake* Snake;

	float NewPelletTimer;
	std::list<pellet> Pellets;

	game_state* GameState;

	game_round_state();
	game_round_state(game_state* GameState);
	void AddPellet();
};

class game_state
{
public:
	game_round_state CurrentRound;
	
	bool IsGameOver;

	//rectangle GameboardDisplayRegion = { 50, 50, 600, 600 };
		
	game_map *GameMap;  //TODO: This doesn't need to be a pointer
	
	game_offscreen_buffer *Buffer;
	keys_down *KeysDown;

	game_state(game_offscreen_buffer* Buffer);
	void ProcessInput(keys_down *KeysDown);
	void RenderBuffer();
};


void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void ProcessSnake(game_state *GameState, snake *Snake);
void ProcessTimers(game_state *GameState);
//void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
//intrectangle ConvertMapTileToDisplayRectangle(intrectangle r, int MaxX, int MaxY, int x, int y);
//intrectangle ConvertMapTileToDisplayRectangle(int Left, int Top, int Width, int Height, int MaxX, int MaxY, int x, int y);
