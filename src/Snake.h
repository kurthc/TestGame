#pragma once

#include "Global.h"
#include "Math.h"
#include "Buffer.h"
#include <list>

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
	int Color;
	float Timer;

	snake(int Length);
	void snake::SetDirection(int x, int y);
	void snake::SetDirection(vec2 Direction);
	void snake::AddSegments(int NewSegmentCount);
};

void ProcessSnake(snake *Snake);
