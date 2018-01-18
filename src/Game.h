#pragma once

#include <iostream>
#include <list>
#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include "Snake.h"


//class hero
//{
//public:
//	vec2 Position;
//	int Width;
//	int Height;
//	int Color;
//	float Speed;
//	vec2 HeroDirection;
//};

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
};

class game_state
{
public:
	//hero Hero;
	snake *Snake;
	std::list<pellet> Pellets;
	float NewPelletTimer;

	//rectangle GameboardDisplayRegion = { 50, 50, 600, 600 };
	game_map *GameMap;
};



game_map *CreateBlankMap(int Width, int Height);
void GameStateInitialize(game_state *GameState, game_offscreen_buffer *Buffer);
void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *Buffer);
void ProcessInput(game_state *GameState, keys_down *KeysDown);
void ProcessTimers(game_state *GameState);
void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer);
//intrectangle ConvertMapTileToDisplayRectangle(intrectangle r, int MaxX, int MaxY, int x, int y);
//intrectangle ConvertMapTileToDisplayRectangle(int Left, int Top, int Width, int Height, int MaxX, int MaxY, int x, int y);
void AddPellet(game_state *GameState);