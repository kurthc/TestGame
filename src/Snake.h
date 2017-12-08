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

void ProcessSnake(snake *Snake);
