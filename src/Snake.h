#pragma once

#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include <list>
#include <cmath>

class snake_segment
{
public:
	vec2 Location;
	vec2 Direction;
	int Color;
	//bool IsExploding;
};

class snake
{
public:
	std::list<snake_segment> Segments;
	float Speed;
	float Timer;      // The timer counts up to 1. When it gets there, move the snake.
	vec2 Direction;

	snake(int Length, vec2 InitialPosition, vec2 InitialDirection);
	void snake::SetDirection(int x, int y);		//TODO: Lose the ints?
	void snake::SetDirection(vec2 Direction);
	void snake::AddSegments(int NewSegmentCount);
	int snake::GetColor(int SegmentNumber);
};

void ProcessSnake(snake *Snake);
