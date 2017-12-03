#pragma once

class vec2
{
public:
	float X;
	float Y;

	vec2();
	vec2(float, float);

	void SetXY(float X, float Y);
	
	//vec2 operator+(vec2 v)
	//{
	//	vec2 vSum;
	//	vSum.X = this->X + v.X;
	//	vSum.Y = this->Y + v.Y;
	//	return vSum;
	//}

	//vec2 operator*(float s)
	//{
	//	vec2 vResult;
	//	vResult.X = this->X * s;
	//	vResult.Y = this->Y * s;
	//	return vResult;
	//}

	
};

vec2 operator+(vec2 v1, vec2 v2);
vec2 & operator+=(vec2 &v1, vec2 v2);
vec2 operator-(vec2 v);
vec2 operator-(vec2 v1, vec2 v2);
vec2 & operator-=(vec2 &v1, vec2 v2);
vec2 operator*(float a, vec2 v);
vec2 operator*(vec2 v, float a);


static vec2 UnitVectorX = vec2(1, 0);
static vec2 UnitVectorY = vec2(0, 1);
//
//static vec2 DirectionRight = vec2(1, 0);
//static vec2 DirectionUp = vec2(0, -1);
//static vec2 DirectionLeft = vec2(-1, 0);
//static vec2 DirectionDown = vec2(0, 1);
