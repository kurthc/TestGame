#include "Global.h"
#include "WindowsLayer.h"

//
//static win32_window_dimension Win32GetWindowDimension(HWND Window)
//{
//    win32_window_dimension Result;
//    
//    RECT ClientRect;
//    GetClientRect(Window, &ClientRect);
//    Result.Width = ClientRect.right - ClientRect.left;
//    Result.Height = ClientRect.bottom - ClientRect.top;
//
//    return(Result);
//}

// Initialize the screen back buffer.
static void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	// Free Buffer.Memory if it is not already empty.
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    int BytesPerPixel = 4;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;   // Negative means this bitmap is treated as top-down. The first bytes are for the top left pixel.
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

static void Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, HDC DeviceContext)
{
	// Copy the Game Buffer into the Memory Device Context...
	StretchDIBits(MemoryDeviceContext, 0, 0, Buffer->Width, Buffer->Height, 0, 0, Buffer->Width, Buffer->Height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);

	// Add an FPS indicator to the screen.
	std::stringstream FPS;
	FPS << "FPS: " << std::setprecision(3) << ObservedFPS;
	
	RECT r = {10, 10, 100, 100};
	SetTextColor(MemoryDeviceContext, RGB(255, 255, 255));
	SetBkColor(MemoryDeviceContext, RGB(0, 0, 0));
	DrawText(MemoryDeviceContext, FPS.str().c_str(), -1, &r, DT_LEFT);

	// ... and then onto the screen.
	BitBlt(DeviceContext, 0, 0, Buffer->Width, Buffer->Height, MemoryDeviceContext, 0, 0, SRCCOPY);
}

static LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{       
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        {
            GlobalRunning = false;
        } break;

        case WM_ACTIVATEAPP:
        {
			// WParam == true if we're selecting this window, false if we're selecting a different window.
			std::cout << "WM_ACTIVATEAPP " << WParam << "\n";
        } break;

        case WM_DESTROY:
        {
            GlobalRunning = false;
        } break;

		case WM_KEYDOWN:
		{
			int VKCode = WParam;
			if (VKCode == 'W')
			{
				KeysDown.Up = 1;
			}
			else if (VKCode == 'A')
			{
				KeysDown.Left = 1;
			}
			else if (VKCode == 'S')
			{
				KeysDown.Down = 1;
			}
			else if (VKCode == 'D')
			{
				KeysDown.Right = 1;
			}
			else if (VKCode == VK_SPACE)
			{
				KeysDown.Space = 1;
			}
			else if (VKCode == '1')
			{
				KeysDown.One = 1;
			}
		} break;

		case WM_KEYUP:
		{
			uint32_t VKCode = WParam;
			if (VKCode == 'W')
			{
				KeysDown.Up = 0;
			}
			else if (VKCode == 'A')
			{
				KeysDown.Left = 0;
			}
			else if (VKCode == 'S')
			{
				KeysDown.Down = 0;
			}
			else if (VKCode == 'D')
			{
				KeysDown.Right = 0;
			}
			else if (VKCode == VK_SPACE)
			{
				KeysDown.Space = 0;
			}
			else if (VKCode == '1')
			{
				KeysDown.One = 0;
			}
		} break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
			Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext);
            //win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            //Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext,
            //                           Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return(Result);
}

timing_information GetSeconds()
{
	LARGE_INTEGER LICounts;
	int Counts;
	float Seconds;
	QueryPerformanceCounter(&LICounts);
	Counts = LICounts.QuadPart;
	Seconds = (float)Counts / CountsPerSecond;
	return {Seconds, Counts, LICounts};
}

void Win32HandleMessages()
{
	// Loop for as long as we have messages.
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		if (Message.message == WM_QUIT)
		{
			GlobalRunning = false;
		}

		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}
	return;
}

// Attach a console window for debugging.
static void Win32AddConsole()
{
	FILE *stream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "CON", "w", stdout);
}

// Create the Memory DC.
bool Win32SetUpMemoryDeviceContext(HDC DeviceContext)
{
	bool Success;
	MemoryDeviceContext = CreateCompatibleDC(DeviceContext);                             // Get a memory DC the same size/attributes as the Window one.
	MemoryDeviceContextBitmap = CreateCompatibleBitmap(DeviceContext, GameWindowWidth, GameWindowHeight);  // The memory DC needs a bitmap to write to.
	Success = SelectObject(MemoryDeviceContext, MemoryDeviceContextBitmap);                                      // Attach the bitmap to the memory DC.
	if (!Success) std::cout << "Failure setting up Device Context\n";
	return Success;
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
	// QueryPerformanceCounter() returns clock time in "counts". We can convert this to seconds with QueryPerformanceFrequency,
	// which returns counts per second (constant and determined at boot).
	// (counts/sec) / (frames/sec) = (count/frame)
	LARGE_INTEGER CountsPerSecondLarge;
	QueryPerformanceFrequency(&CountsPerSecondLarge);
	CountsPerSecond = (int)CountsPerSecondLarge.QuadPart;

	//int CountsPerFrame = (int)CountsPerSecondLarge.QuadPart / TargetFPS;

	// Attach a console window for debugging.
	Win32AddConsole();

	// Initialize the back buffer.
    Win32ResizeDIBSection(&GlobalBackBuffer, GameWindowWidth, GameWindowHeight);
    
	// Define the WindowClass
	WNDCLASSA WindowClass = {};
	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";

    if(RegisterClassA(&WindowClass))
    {
        HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "CAK Test Game", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);
        if(Window)
        {
			
			// Get the device context for the window (for writing graphics to the window)
			// and make a memory device context to pass through to it. 
			HDC DeviceContext = GetDC(Window);
			Win32SetUpMemoryDeviceContext(DeviceContext);

			game_offscreen_buffer* GameBuffer = new game_offscreen_buffer(GlobalBackBuffer.Width, GlobalBackBuffer.Height);
			GameBuffer->Memory = GlobalBackBuffer.Memory;

			game_state* GameState = new game_state(GameBuffer);
			GameBuffer->GameState = GameState;
			GlobalGameStatePointer = GameState;

			GlobalRunning = true;
			timing_information FrameStart = GetSeconds();
			timing_information PreviousFrameStart {};

			std::stringstream TimingMessage;

			//
			// ********* The Main game loop starts here *************
			//

			while(GlobalRunning)
            {
				// Handle Windows messages, process the game, and render the buffer.
				Win32HandleMessages();
				GameState->GameStateProcess(&KeysDown, GameBuffer);
				GameBuffer->RenderBuffer();

				timing_information EndProcessingTime = GetSeconds();       // Time game processing ends for a frame

				TimingMessage.str("");
				//TimingMessage << "Processing counts elapsed: " << EndProcessingTime.Counts - FrameStart.Counts;
				//TimingMessage << "Processing seconds elapsed: " << EndProcessingTime.Seconds - FrameStart.Seconds;

				timing_information CurrentTime = GetSeconds();     //TODO: You don't need this variable. Factor it out.
	
				// Do something with sleep here?
				while (CurrentTime.Seconds - FrameStart.Seconds < (1.0f / TargetFPS))
				{
					CurrentTime = GetSeconds();
				}
               
				//
				// Update the window.
				//
				Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext);            // Calls StretchDIBits()

				// Update the timing information.
				PreviousFrameStart = FrameStart;
				FrameStart = GetSeconds();

				ObservedFPS = 1.0f / (FrameStart.Seconds - PreviousFrameStart.Seconds);

				//TimingMessage << "Total frame elapsed: " << FrameStart.Counts - PreviousFrameStart.Counts;
				TimingMessage << "FPS: " << ObservedFPS << "  ";
				TimingMessage << "Total frame sec elapsed: " << FrameStart.Seconds - PreviousFrameStart.Seconds;
				std::cout << TimingMessage.str() << "\n";

			}  // ****************End of Game Loop
			
        }
        else
        {
			std::cout << "CreateWindowExA() failed.\n";
        }
    }
    else
    {
		std::cout << "RegisterClassA() failed.\n";
    }
    
    return(0);
}

