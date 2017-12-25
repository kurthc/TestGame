#pragma once

#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include <list>
#include <cmath>

class snake_segment
{
public:
	intvec2 Location;
	intvec2 Direction;
	int Color;
	//bool IsExploding;
};

class snake
{
public:
	std::list<snake_segment> Segments;
	float Speed;
	float Timer;

	snake(int Length, intvec2 InitialPosition, intvec2 InitialDirection);
	void snake::SetDirection(int x, int y);
	void snake::SetDirection(vec2 Direction);
	void snake::AddSegments(int NewSegmentCount);
	int snake::GetColor(int SegmentNumber);
};

void ProcessSnake(snake *Snake);
