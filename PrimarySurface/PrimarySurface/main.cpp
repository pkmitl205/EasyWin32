#include <Windows.h>
#include <windowsx.h>

#include "ddraw.h"

LPDIRECTDRAW7 dd = NULL;
LPDIRECTDRAWSURFACE7 primary = NULL;

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
	int surf_height =  bitmap.bmHeight;

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
	
	RECT r;
	r.left = 100;
	r.top = 100;
	r.right = r.left + 100;
	r.bottom = r.top + 120;;

//	primary->Blt(&r, GetBmp(dd, "SR01.bmp"), NULL, DDBLT_WAIT, NULL);

	RECT dest_r;
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
				x += 1;
				dest_r.left = x;
				dest_r.top = 100;
				dest_r.right = dest_r.left + 105;
				dest_r.bottom = dest_r.top + 115;

				primary->Blt(&dest_r, GetBmp(dd, "SR01.bmp"), NULL, DDBLT_WAIT, NULL);

			}
	}
	return (msg.wParam);
}