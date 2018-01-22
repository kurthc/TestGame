#pragma once

class vec2
{
public:
	float X;
	float Y;

	vec2();
	vec2(float, float);

	void SetXY(float X, float Y);
	//friend bool operator==(const vec2&, const vec2&);
	bool operator==(const vec2&) const;
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
