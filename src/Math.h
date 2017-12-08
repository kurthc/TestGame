#pragma once

class vec2
{
public:
	float X;
	float Y;

	vec2();
	vec2(float, float);

	void SetXY(float X, float Y);
};

class intvec2
{
public:
	int X;
	int Y;

	intvec2();
	intvec2(int, int);

	void SetXY(int X, int Y);
};


vec2 operator+(vec2 v1, vec2 v2);
vec2 & operator+=(vec2 &v1, vec2 v2);
vec2 operator-(vec2 v);
vec2 operator-(vec2 v1, vec2 v2);
vec2 & operator-=(vec2 &v1, vec2 v2);
vec2 operator*(float a, vec2 v);
vec2 operator*(vec2 v, float a);

intvec2 operator+(intvec2 v1, intvec2 v2);
intvec2 & operator+=(intvec2 &v1, intvec2 v2);
intvec2 operator-(intvec2 v);
intvec2 operator-(intvec2 v1, intvec2 v2);
intvec2 & operator-=(intvec2 &v1, intvec2 v2);
intvec2 operator*(int a, intvec2 v);
intvec2 operator*(intvec2 v, int a);

static vec2 UnitVectorX = vec2(1, 0);
static vec2 UnitVectorY = vec2(0, 1);

static intvec2 intUnitVectorX = intvec2(1, 0);
static intvec2 intUnitVectorY = intvec2(0, 1);
