#include "Game.h"

snake_segment::snake_segment(vec2 Location, vec2 Direction, snake* Snake)
{
	this->Location = Location;
	this->Direction = Direction;
	this->IntangibleTimer = 1.0f;
	this->Snake = Snake;
	this->Color = Snake->GetColor(Snake->Segments.size() + 1);
}

vec2 snake_segment::RealLocation()
{
	snake* Snake = this->Snake;
	float RealX = this->Location.X + (*Snake).Timer * this->Direction.X;
	float RealY = this->Location.Y + (*Snake).Timer * this->Direction.Y;
	return { RealX, RealY };
}


rectangle snake_segment::HitRectangle()
{
	vec2 RealLocation = this->RealLocation();
	return {RealLocation.X, RealLocation.Y, 1.0f, 1.0f};
}

snake::snake(int Length, vec2 InitialPosition, vec2 InitialDirection)
{
	
	// Create the initial snake.
	this->Direction = InitialDirection;

	snake_segment* ssHead = new snake_segment(InitialPosition, InitialDirection, this);
	this->Segments.push_back(*ssHead);

	this->AddSegments(Length - 1);

	this->Speed = 1.5;
	this->Timer = 0;
}

void snake::SetDirection(float x, float y)
{
	
	//this->Segments.front().Direction.SetXY(x, y);
	vec2 Direction = vec2((float)x, (float)y);
	this->Direction = Direction;
}

void snake::SetDirection(vec2 Direction)
{
	this->SetDirection(Direction.X, Direction.Y);
}


void snake::AddSegments(int NewSegmentCount)
{
	int ExistingSegmentCount = this->Segments.size();
	for (int i = 0; i < NewSegmentCount; i++)
	{
		// Add new segments at the location of the last segment moving toward the last segment (i.e. not moving)
		snake_segment *NewSegment = new snake_segment(this->Segments.back().Location, vec2(0,0), this);
		this->Segments.push_back(*NewSegment);
	}
}

int snake::GetColor(int SegmentNumber)
{
	// Color the snake with a sinusoidal green pattern from MinColor to MaxColor
	constexpr float MaxColor = 1.0f;
	constexpr float MinColor = 0.4f;

	float GreenComponent = (MaxColor + MinColor) / 2.0f + (MaxColor - MinColor) / 2.0f * (float)sin(SegmentNumber * 2.0 * 3.14159265358979 / 12.0f);
	int SegmentColor = RGB(0, GreenComponent, 0);
	return SegmentColor;
}
