#include "Snake.h"

snake::snake(int Length, vec2 InitialPosition, vec2 InitialDirection)
{
	// Create the initial snake.
	vec2 LastSegmentLocation = { 0, 0 };
	for (int i = 0; i < Length; i++)
	{
		snake_segment *ss = new snake_segment();
		ss->Location = InitialPosition;
		ss->Color = this->GetColor(i);

		if (i == 0)
		{
			ss->Direction = InitialDirection;
		}
		else
		{
			ss->Direction = LastSegmentLocation - ss->Location;
		}
		LastSegmentLocation = ss->Location;
		this->Segments.push_back(*ss);
	}

	this->Speed = 1.5;
	this->Timer = 0;
}

void snake::SetDirection(int x, int y)
{
	
	this->Segments.front().Direction.SetXY(x, y);
}

int snake::GetColor(int SegmentNumber)
{
	// Color the snake with a sinusoidal green pattern from MinColor to MaxColor
	constexpr float MaxColor = 1.0f;
	constexpr float MinColor = 0.4f;

	float GreenComponent = (MaxColor + MinColor) / 2.0 + (MaxColor - MinColor) / 2.0 * sin((double)SegmentNumber * 2 * 3.14159265358979 / 12.0f);
	int SegmentColor = RGB(0, GreenComponent, 0);
	return SegmentColor;
}

void snake::SetDirection(vec2 Direction)
{
	this->SetDirection(Direction.X, Direction.Y);
}

void ProcessSnake(snake *Snake)
{
	std::list<snake_segment> *Segments = &(Snake->Segments);

	// The timer counts up to 1. When it gets there, move the snake.
	Snake->Timer += Snake->Speed / 30;   // FPS?
	if (Snake->Timer >= 1)
	{
		vec2 LastLocation = { 0, 0 };
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
	int ExistingSegmentCount = this->Segments.size();
	for (int i = 0; i < NewSegmentCount; i++)
	{
		// Add new segments at the location of the last segment moving toward the last segment (i.e. not moving)
		snake_segment *NewSegment = new snake_segment();
		NewSegment->Location = this->Segments.back().Location;
		NewSegment->Direction.SetXY(0, 0);
		NewSegment->Color = this->GetColor(ExistingSegmentCount + i);
		this->Segments.push_back(*NewSegment);
	}
}
