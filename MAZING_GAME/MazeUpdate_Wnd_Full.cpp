#include "INI.h"

void MazeUpdate_Wnd_Full(HWND hwnd, HDC hdc)
{
	int x, y;

	x = GetWindowLongPtr(hwnd, 0) & 0xFF;
	y = GetWindowLongPtr(hwnd, 0) >> 8;

	if (x <= xMax / 2)
	{
		if (y <= yMax / 2)
		{
			MazePaint(hwnd, hdc, 0, 0);
		}
		else if (y >= Num - yMax / 2)
		{
			MazePaint(hwnd, hdc, Num - yMax, 0);
		}
		else
			MazePaint(hwnd, hdc, y - yMax / 2, 0);
	}
	else if (x >= Num - xMax / 2)
	{
		if (y <= yMax / 2)
		{
			MazePaint(hwnd, hdc, 0, Num - xMax);
		}
		else if (y >= Num - yMax / 2)
		{
			MazePaint(hwnd, hdc, Num - yMax, Num - xMax);
		}
		else
			MazePaint(hwnd, hdc, y - yMax / 2, Num - xMax);
	}
	else
	{
		if (y <= yMax / 2)
		{
			MazePaint(hwnd, hdc, 0, x - xMax / 2);
		}
		else if (y >= Num - yMax / 2)
		{
			MazePaint(hwnd, hdc, Num - yMax, x - xMax / 2);
		}
		else
			MazePaint(hwnd, hdc, y - yMax / 2, x - xMax / 2);
	}
}