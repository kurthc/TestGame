#pragma once

#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include <list>
#include <cmath>

class snake;

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
};

class snake
{
public:
	std::list<snake_segment> Segments;

	float Speed;
	float Timer;      // The timer counts up to 1. When it gets there, move the snake.
	vec2 Direction;
	snake_segment* Head;

	snake(int Length, vec2 InitialPosition, vec2 InitialDirection);
	void snake::SetDirection(float x, float y);
	void snake::SetDirection(vec2 Direction);
	void snake::AddSegments(int NewSegmentCount);
	int snake::GetColor(int SegmentNumber);
};
