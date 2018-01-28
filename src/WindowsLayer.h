#pragma once

#include <windows.h>
#include <iostream>
#include "Game.h"


struct win32_offscreen_buffer
{
	// Pixels are always 32-bits wide, Memory Order BB GG RR XX
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
};

struct win32_window_dimension
{
	int Width;
	int Height;
};

static bool GlobalRunning;
static win32_offscreen_buffer GlobalBackBuffer;
static keys_down KeysDown;
static HDC MemoryDeviceContext;
static HBITMAP MemoryDeviceContextBitmap;


bool Win32SetUpMemoryDeviceContext(HDC DeviceContext);