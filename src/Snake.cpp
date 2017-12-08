#include "Snake.h"

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
	std::list<snake_segment> *Segments = &((*Snake).Segments);

	// The timer counts up to 1. When it gets there, move the snake.
	(*Snake).Timer += (*Snake).Speed / 30;   // FPS?
	if ((*Snake).Timer >= 1)
	{
		intvec2 LastLocation = { 0, 0 };
		for (std::list<snake_segment>::iterator it = (*Segments).begin(); it != (*Segments).end(); it++)
		{
			(*it).Location = (*it).Location + (*it).Direction;
			if (it != (*Segments).begin())
			{
				(*it).Direction = LastLocation - (*it).Location;
			}
			LastLocation = (*it).Location;
		}
		(*Snake).Timer = 0;
	}

}
