#include "Global.h"
#include "win32_handmade.h"


static win32_window_dimension Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;
    
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return(Result);
}

static void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	// Initialize the buffer.

	// Free Buffer.Memory if it is not already empty.
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    int BytesPerPixel = 4;

    // NOTE(casey): When the biHeight field is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for the top left pixel
    // in the bitmap, not the bottom left!
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

static void Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    StretchDIBits(DeviceContext, 0, 0, Buffer->Width, Buffer->Height,
			0, 0, Buffer->Width, Buffer->Height,
			Buffer->Memory,
			&Buffer->Info,
			DIB_RGB_COLORS, SRCCOPY);

	//char *Something = "This is a test";
	//RECT r = { 10, 10, 100, 100 };
	//DrawText(DeviceContext, Something, -1, &r, DT_LEFT);
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
            OutputDebugStringA("WM_ACTIVATEAPP\n");
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
		} break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext,
                                       Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return(Result);
}

//void SendIntToDebug(int n)
//{
//	char MsgBuffer[256];
//	wsprintf(MsgBuffer, "%d\n", n);
//	OutputDebugStringA(MsgBuffer);
//}
//
//void SendStringToDebug(char *Message)
//{
//	OutputDebugStringA(Message);
//}

float GetSeconds()
{
	LARGE_INTEGER Counts;
	QueryPerformanceCounter(&Counts);
	return ((float)Counts.QuadPart) / CountsPerSecond;
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

static void Win32AddConsole()
{
	FILE *stream;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&stream, "CON", "w", stdout);
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
			
			// Since we specified CS_OWNDC, we can just get one device context and use it forever because we are not sharing it with anyone.
            HDC DeviceContext = GetDC(Window);

			//game_offscreen_buffer GameBuffer(GlobalBackBuffer.Width, GlobalBackBuffer.Height);
			game_offscreen_buffer* GameBuffer = new game_offscreen_buffer(GlobalBackBuffer.Width, GlobalBackBuffer.Height);
			GameBuffer->Memory = GlobalBackBuffer.Memory;

			game_state* GameState = new game_state(GameBuffer);
			

			GlobalRunning = true;
			float LastFrameStart = GetSeconds();
			float CurrentTime = 0;
			int FrameCount = 0;

			//
			// ********* The Main game loop starts here *************
			//
            while(GlobalRunning)
            {
				// Handle Windows messages, process the game, and render the buffer.
				Win32HandleMessages();
				GameStateProcess(GameState, &KeysDown, GameBuffer);
				//RenderBuffer(GameState, GameBuffer);
				GameState->RenderBuffer();

				// Do something with sleep here?
				CurrentTime = GetSeconds();
				while (CurrentTime - LastFrameStart < (1 / TargetFPS))
				{
					CurrentTime = GetSeconds();
				}
                
				//
				// Update the window.
				//
                win32_window_dimension Dimension = Win32GetWindowDimension(Window);
                Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext,
                                           Dimension.Width, Dimension.Height);

				LastFrameStart = GetSeconds();
				
				// Output the number of frames if desired.
				//std::cout << "Frame Count: " << FrameCount++ << std::endl;
				
			}  // ****************End of Game Loop
			
        }
        else
        {
            // TODO(casey): Logging
        }
    }
    else
    {
        // TODO(casey): Logging
    }
    
    return(0);
}

