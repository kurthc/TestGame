#pragma once

#include <stdint.h>

/*
TODOs

Fix DoRectanglesInterect()
Score


*/

//template <typename T> inline T const& MAX(T const& X, T const& Y)
template <typename T> inline T const& MAX(T const& X, T const& Y)
{
	return X > Y ? X : Y;
}

template <typename T> inline T const& MIN(T const& X, T const& Y)
{
	return X > Y ? Y : X;
}

static int GameWindowWidth = 1280;
static int GameWindowHeight = 720;
static int TargetFPS = 30;
static int CountsPerSecond;

class rectangle
{
public:
	float x;
	float y;
	float Width;
	float Height;
	rectangle(float, float, float, float);
};

struct intrectangle
{
	int x;
	int y;
	int Width;
	int Height;
};

struct keys_down
{
	int Left;
	int Right;
	int Up;
	int Down;
	int Space;
};


int RGB(float Red, float Green, float Blue);
bool DoRectanglesIntersect(rectangle R1, rectangle R2, float Precision);