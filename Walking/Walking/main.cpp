#include <Windows.h>
#include <windowsx.h>

#include "ddraw.h"
#define WALKTIME	100

LPDIRECTDRAW7 dd = NULL;
LPDIRECTDRAWSURFACE7 primary = NULL;
LPDIRECTDRAWSURFACE7 temp_surf[20];

LPDIRECTDRAWSURFACE7 GetBmp(LPDIRECTDRAW7 directdraw, LPCTSTR fileName)
{
	HDC hdc;
	HBITMAP bit;
	LPDIRECTDRAWSURFACE7 surf;

	bit = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (!bit) return NULL;

	BITMAP bitmap;
	GetObject(bit, sizeof(BITMAP), &bitmap);
	int surf_width = bitmap.bmWidth;
	int surf_height = bitmap.bmHeight;

	HRESULT result;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = surf_width;
	ddsd.dwHeight = surf_height;
	result = directdraw->CreateSurface(&ddsd, &surf, NULL);

	if (result != DD_OK)
	{
		DeleteObject(bit);
		return NULL;
	}
	else
	{
		surf->GetDC(&hdc);
		HDC bit_dc = CreateCompatibleDC(hdc);
		SelectObject(bit_dc, bit);
		BitBlt(hdc, 0, 0, surf_width, surf_height, bit_dc, 0, 0, SRCCOPY);

		surf->ReleaseDC(hdc);
		DeleteDC(bit_dc);
	}
	return surf;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)
		{
			PostMessage(hwnd, WM_DESTROY, 0, 0);
		}
	}break;

	case WM_TIMER:
	{
		if (wparam == WALKTIME)
		{
			static int i = 0;
			static int x = 0;

			RECT r;
			r.left = x;
			r.top = 100;
			r.right = r.left + 200;
			r.bottom = r.top + 200;;

			primary->Blt(&r, temp_surf[i], NULL, DDBLT_WAIT, NULL);

			i++;
			if (i == 3) i = 0;
			x += 10;
			if (x > 1000)x = 0;
		}
	}break;

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

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
	//dd-> SetDisplayMode(800, 600, 16, 0, 0);

	DDSURFACEDESC2 surf;
	ZeroMemory(&surf, sizeof(surf));
	surf.dwSize = sizeof(surf);
	surf.dwFlags = DDSD_CAPS;
	surf.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	dd->CreateSurface(&surf, &primary, NULL);

	temp_surf[0] = GetBmp(dd, "W05.bmp");
	temp_surf[1] = GetBmp(dd, "W06.bmp");
	temp_surf[2] = GetBmp(dd, "W07.bmp");
	temp_surf[3] = GetBmp(dd, "W08.bmp");

	SetTimer(hwnd, WALKTIME, 50, NULL);

	int x = -50;

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