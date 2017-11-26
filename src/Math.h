#pragma once

class Vector2
{
public:
	float X;
	float Y;

	Vector2();
	Vector2(float, float);

	void SetXY(float X, float Y);
	
	Vector2 operator+(Vector2 v)
	{
		Vector2 vSum;
		vSum.X = this->X + v.X;
		vSum.Y = this->Y + v.Y;
		return vSum;
	}

	Vector2 operator*(float s)
	{
		Vector2 vResult;
		vResult.X = this->X * s;
		vResult.Y = this->Y * s;
		return vResult;
	}
};


static Vector2 DirectionRight = Vector2(1, 0);
static Vector2 DirectionUp = Vector2(0, -1);
static Vector2 DirectionLeft = Vector2(-1, 0);
static Vector2 DirectionDown = Vector2(0, 1);
