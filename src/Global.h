#pragma once

#include <stdint.h>

/*
TODOs

x Set up rects for different parts of the screen
Set up RenderDebugOverLay()
Set up a part for text.

Fix DoRectanglesInterect()


*/

// Gameplay constants.
constexpr float SNAKE_INITIAL_SPEED = 1.5f;
constexpr float SNAKE_ADDITIVE_SPEED_INCREASE = 0.1f;
constexpr float SNAKE_MULTIPLICATIVE_SPEED_INCREASE = 1.05f;
constexpr int SEGMENTS_TO_ADD_ON_EAT_PELLET = 3;

static int GameWindowWidth = 1280;
static int GameWindowHeight = 720;
static int TargetFPS = 30;
static int CountsPerSecond;



template <typename T> inline T const& MAX(T const& X, T const& Y)
{
	return X > Y ? X : Y;
}

template <typename T> inline T const& MIN(T const& X, T const& Y)
{
	return X > Y ? Y : X;
}


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
	intrectangle() {};
	intrectangle(int x, int y, int Width, int Height) : x(x), y(y), Width(Width), Height(Height) {};
};

// TODO: Why are these int?
struct keys_down
{
	int Left;
	int Right;
	int Up;
	int Down;
	int Space;
	int One;
};


int RGB(float Red, float Green, float Blue);
bool DoRectanglesIntersect(rectangle R1, rectangle R2, float Precision);