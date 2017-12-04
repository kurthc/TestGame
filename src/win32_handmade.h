#pragma once

#include <windows.h>
#include <xinput.h>
#include "Game.h"


struct win32_offscreen_buffer
{
	// NOTE(casey): Pixels are alwasy 32-bits wide, Memory Order BB GG RR XX
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int Pitch;
};

struct win32_window_dimension
{
	int Width;
	int Height;
};


global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable keys_down KeysDown;




//extern struct game_state GameState;

void SendStringToDebug(char *Message);