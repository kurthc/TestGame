#include "Math.h"

Vector2::Vector2()
{
	this->X = 0;
	this->Y = 0;
}

Vector2::Vector2(float X, float Y)
{
	this->X = X;
	this->Y = Y;
}

Vector2 operator +(Vector2 v1, Vector2 v2)
{
	Vector2 vSum;
	vSum.X = v1.X + v2.X;
	vSum.Y = v2.Y + v2.Y;
	return vSum;
}


void Vector2::SetXY(float x, float y)
{
	this->X = x;
	this->Y = y;
}