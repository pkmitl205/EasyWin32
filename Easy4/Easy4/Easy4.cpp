#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (msg)
	{
		case WM_KEYDOWN:
		{
			char s[20];
			switch (wparam)
			{
				case VK_RIGHT: strcpy(s, "right		"); break;
				case VK_LEFT: strcpy(s, "  left		"); break;
				default: strcpy(s, "				"); break;
			}

				TextOut(hdc, 10, 50, s, strlen(s));
		}break;

		case WM_PAINT:
		{

		}break;

		case WM_DESTROY:
		{
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

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (msg.wParam);
}