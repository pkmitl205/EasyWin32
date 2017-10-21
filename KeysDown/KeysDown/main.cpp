// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 2 Keyboard State with Windows API v1.0
// winmain.cpp

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// Function prototypes
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// Global variables
HINSTANCE hinst;
HDC    hdc;                 // handle to device context
TCHAR ch = ' ';             // character entered
RECT rect;                  // rectangle
PAINTSTRUCT ps;             // used in WM_PAINT
bool vkKeys[256];           // state of virtual keys, false or true

							// Constants
const char CLASS_NAME[] = "Keyboard";
const char APP_TITLE[] = "Keys Down";
const int  WINDOW_WIDTH = 400;  // width of window
const int  WINDOW_HEIGHT = 400;  // height of window

								 //=============================================================================
								 // Starting point for a Windows application
								 //=============================================================================
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG     msg;

	// Create the main window
	if (!CreateMainWindow(hInstance, nCmdShow))
		return false;

	for (int i = 0; i<256; i++)   // initialize virtual key array
		vkKeys[i] = false;

	// main message loop
	int done = 0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//look for quit message
			if (msg.message == WM_QUIT)
				done = 1;

			//decode and pass messages on to WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//=============================================================================
// window event callback function
//=============================================================================
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short nVirtKey;                 // virtual-key code 
	const short SHIFTED = (short)0x8000;
	TEXTMETRIC tm;                  // structure for text metrics 
	DWORD chWidth = 20;             // width of characters
	DWORD chHeight = 20;            // height of characters

	switch (msg)
	{
	case WM_CREATE:
		// get the text metrics
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hwnd, hdc);
		chWidth = tm.tmAveCharWidth;        // average character width
		chHeight = tm.tmHeight;             // character height
		return 0;

	case WM_DESTROY:
		//tell Windows to kill this program
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:                                // key down
		vkKeys[wParam] = true;
		switch (wParam)
		{
		case VK_SHIFT:                          // shift key
			nVirtKey = GetKeyState(VK_LSHIFT);  // get state of left shift
			if (nVirtKey & SHIFTED)             // if left shift
				vkKeys[VK_LSHIFT] = true;
			nVirtKey = GetKeyState(VK_RSHIFT);  // get state of right shift
			if (nVirtKey & SHIFTED)             // if right shift
				vkKeys[VK_RSHIFT] = true;
			break;
		case VK_CONTROL:                        // control key
			nVirtKey = GetKeyState(VK_LCONTROL);
			if (nVirtKey & SHIFTED)             // if left control
				vkKeys[VK_LCONTROL] = true;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if (nVirtKey & SHIFTED)             // if right control
				vkKeys[VK_RCONTROL] = true;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);           // force WM_PAINT
		return 0;
		break;

	case WM_KEYUP:                                  // key up
		vkKeys[wParam] = false;
		switch (wParam)
		{
		case VK_SHIFT:                          // shift key
			nVirtKey = GetKeyState(VK_LSHIFT);
			if ((nVirtKey & SHIFTED) == 0)      // if left shift
				vkKeys[VK_LSHIFT] = false;
			nVirtKey = GetKeyState(VK_RSHIFT);
			if ((nVirtKey & SHIFTED) == 0)      // if right shift
				vkKeys[VK_RSHIFT] = false;
			break;
		case VK_CONTROL:                        // control key
			nVirtKey = GetKeyState(VK_LCONTROL);
			if ((nVirtKey & SHIFTED) == 0)      // if left control
				vkKeys[VK_LCONTROL] = false;
			nVirtKey = GetKeyState(VK_RCONTROL);
			if ((nVirtKey & SHIFTED) == 0)      // if right control
				vkKeys[VK_RCONTROL] = false;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);    // force WM_PAINT
		return 0;
		break;

	case WM_CHAR:               // a character was entered by the keyboard
		switch (wParam)         // the character is in wParam
		{
		case 0x08:              // backspace
		case 0x09:              // tab
		case 0x0A:              // linefeed
		case 0x0D:              // carriage return
		case 0x1B:              // escape
			return 0;           // non displayable character
		default:                // displayable character
			ch = (TCHAR)wParam;    // get the character
			InvalidateRect(hwnd, NULL, TRUE);   // force WM_PAINT
			return 0;
		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);    // get handle to device context
		TextOut(hdc, 0, 0, &ch, 1);     // display the character

										// Display the state of vkKeys array
										// Display 'T' if key is down and 'F' is key is up
		for (int r = 0; r<16; r++)
		{
			for (int c = 0; c<16; c++)
			{
				if (vkKeys[r * 16 + c])
				{
					SetBkMode(hdc, OPAQUE);         // opaque text background
					TextOut(hdc, c*chWidth + chWidth * 2, r*chHeight + chHeight * 2, "T ", 2);
				}
				else {
					SetBkMode(hdc, TRANSPARENT);    // transparent text background
					TextOut(hdc, c*chWidth + chWidth * 2, r*chHeight + chHeight * 2, "F ", 2);
				}
			}
		}

		EndPaint(hwnd, &ps);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

//=============================================================================
// Create the window
// Returns: false on error
//=============================================================================
bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	// Fill in the window class structure with parameters 
	// that describe the main window. 
	wcx.cbSize = sizeof(wcx);           // size of structure 
	wcx.style = CS_HREDRAW | CS_VREDRAW;    // redraw if size changes 
	wcx.lpfnWndProc = WinProc;          // points to window procedure 
	wcx.cbClsExtra = 0;                 // no extra class memory 
	wcx.cbWndExtra = 0;                 // no extra window memory 
	wcx.hInstance = hInstance;          // handle to instance 
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);    // gray background brush 
	wcx.lpszMenuName = NULL;           // name of menu resource 
	wcx.lpszClassName = CLASS_NAME;	    // name of window class 
	wcx.hIconSm = NULL;                 // small class icon 

										// Register the window class. 
										// RegisterClassEx returns 0 on error.
	if (RegisterClassEx(&wcx) == 0)    // if error
		return false;

	// Create window
	hwnd = CreateWindow(
		CLASS_NAME,             // name of the window class
		APP_TITLE,              // title bar text
		WS_OVERLAPPEDWINDOW,    // window style
		CW_USEDEFAULT,          // default horizontal position of window
		CW_USEDEFAULT,          // default vertical position of window
		WINDOW_WIDTH,           // width of window
		WINDOW_HEIGHT,          // height of the window
		(HWND)NULL,            // no parent window
		(HMENU)NULL,           // no menu
		hInstance,              // handle to application instance
		(LPVOID)NULL);         // no window parameters

							   // if there was an error creating the window
	if (!hwnd)
		return false;

	// Show the window
	ShowWindow(hwnd, nCmdShow);

	// Send a WM_PAINT message to the window procedure
	UpdateWindow(hwnd);
	return true;
}
