#include <Windows.h>
#include <windowsx.h>

#include "ddraw.h"

LPDIRECTDRAW7 dd = NULL;
LPDIRECTDRAWSURFACE7 primary = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
	{

	}break;

	case WM_DESTROY:
	{
		primary->Release();
		primary = NULL;
		dd->Release();
		dd = NULL;
		PostQuitMessage(0);
	}break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;

	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = "MyWin";
	wcex.lpszMenuName = NULL;
	wcex.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(hinstance, IDC_ARROW);
	wcex.hIconSm = LoadIcon(hinstance, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	HWND hwnd;
	MSG msg;

	hwnd = CreateWindowEx(NULL, "MyWin", "Win32 Window", WS_OVERLAPPEDWINDOW, 100, 100, 400, 400, NULL, NULL, hinstance, NULL);

	ShowWindow(hwnd, ncmdshow);
	UpdateWindow(hwnd);

	DirectDrawCreateEx(NULL, (LPVOID*)&dd, IID_IDirectDraw7, NULL);

	dd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	dd-> SetDisplayMode(800, 600, 16, 0, 0);

	DDSURFACEDESC2 surf;
	ZeroMemory(&surf, sizeof(surf));
	surf.dwSize = sizeof(surf);
	surf.dwFlags = DDSD_CAPS;
	surf.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	dd->CreateSurface(&surf, &primary, NULL);
	
	HDC hdc;
	hdc = GetDC(hwnd);
	primary->GetDC(&hdc);
	TextOut(hdc, 100, 100, "Hello", 5);
	primary->ReleaseDC(hdc);


	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			else
			{
				//Game Main

			}
	}
	return (msg.wParam);
}