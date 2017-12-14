#include "INI.h"

extern int Flag0;
extern int Flag1;
extern int PreFlag0;
extern int PreFlag1;

void MazeUpdate_ChildWnd(HWND hwnd, HDC hdc)
{
	int *ptFlag;
	int *ptPreFlag;
	int x, y;

	if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
	{
		ptFlag = &Flag0;
		ptPreFlag = &PreFlag0;
	}
	else
	{
		ptFlag = &Flag1;
		ptPreFlag = &PreFlag1;
	}

	hdc = GetDC(hwnd);

	x = GetWindowLongPtr(hwnd, 0) & 0xFF;
	y = GetWindowLongPtr(hwnd, 0) >> 8;

	switch (*ptFlag)
	{
	case LEFT:
		MazeUpdate_Data(x, y, x - 1, y);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x - 1, y, *ptPreFlag);
		x = x - 1;
		SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));
		*ptPreFlag = *ptFlag;
		*ptFlag = FLAG_DEFAULT;
		break;

	case RIGHT:
		MazeUpdate_Data(x, y, x + 1, y);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x + 1, y, *ptPreFlag);
		x = x + 1;
		SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));
		*ptPreFlag = *ptFlag;
		*ptFlag = FLAG_DEFAULT;
		break;

	case UP:
		MazeUpdate_Data(x, y, x, y - 1);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y - 1, *ptPreFlag);
		y = y - 1;
		SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));
		*ptPreFlag = *ptFlag;
		*ptFlag = FLAG_DEFAULT;
		break;

	case DOWN:
		MazeUpdate_Data(x, y, x, y + 1);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y + 1, *ptPreFlag);
		y = y + 1;
		SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));
		*ptPreFlag = *ptFlag;
		*ptFlag = FLAG_DEFAULT;
		break;

	default:
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y, *ptPreFlag);
		break;
	}
}