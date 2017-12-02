#pragma once

#include <stdint.h>

#define internal static 
#define local_persist static 
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define HMRGB(R, G, B) B + (G << 8) + (R << 16)
//#define MAX(x,y) = (x > y) ? x : y
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

global_variable int GameWindowWidth = 1280;
global_variable int GameWindowHeight = 720;
global_variable int TargetFPS = 60;
global_variable int32_t CountsPerSecond;

struct keys_down
{
	int Left;
	int Right;
	int Up;
	int Down;
	int Space;
};
