#include "Snake.h"

snake::snake(int Length)
{
	// Create the initial snake.
	intvec2 LastSegmentLocation = { 0, 0 };
	for (int i = 0; i < Length; i++)
	{
		snake_segment *ss = new snake_segment();
		ss->Location.SetXY(6, 15 - i);
		ss->Color = RGB(0, .5 + .05*i, .1);

		if (i == 0)
		{
			ss->Direction = intUnitVectorY;
		}
		else
		{
			ss->Direction = LastSegmentLocation - ss->Location;
		}
		LastSegmentLocation = ss->Location;
		this->Segments.push_back(*ss);
	}
	this->Color = HMRGB(255, 255, 0);
	this->Speed = 2;
	this->Timer = 0;
}

void snake::SetDirection(int x, int y)
{
	//this->Direction.SetXY(x, y);
	this->Segments.front().Direction.SetXY(x, y);

}

void snake::SetDirection(vec2 Direction)
{
	this->SetDirection(Direction.X, Direction.Y);
}

void ProcessSnake(snake *Snake)
{
	//snake *Snake = &(GameState->Snake);
	std::list<snake_segment> *Segments = &(Snake->Segments);

	// The timer counts up to 1. When it gets there, move the snake.
	Snake->Timer += Snake->Speed / 30;   // FPS?
	if (Snake->Timer >= 1)
	{
		intvec2 LastLocation = { 0, 0 };
		for (std::list<snake_segment>::iterator it = Segments->begin(); it != Segments->end(); it++)
		{
			it->Location = it->Location + it->Direction;
			if (it != Segments->begin())
			{
				it->Direction = LastLocation - it->Location;
			}
			LastLocation = it->Location;
		}
		Snake->Timer = 0;
	}

}

void snake::AddSegments(int NewSegmentCount)
{
	for (int i = 0; i < NewSegmentCount; i++)
	{
		// Add new segments at the location of the last segment moving toward the last esgment (i.e. not moving)
		snake_segment *NewSegment = new snake_segment();
		NewSegment->Location = this->Segments.back().Location;
		NewSegment->Direction.SetXY(0, 0);
		//NewSegment->Color = HMRGB(0, 255, 0);
		NewSegment->Color = (rand() % 256) * 256 * 256 + (rand() % 256) * 256 + (rand() % 256);
		this->Segments.push_back(*NewSegment);
	}
}
