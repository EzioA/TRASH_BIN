#include "INI.h"

void MazePaint_INI(HWND hwnd, HDC hdc)
{
	if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
	{
		MazePaint(hwnd, hdc, 0, 0);
		RolePaint(hwnd, hdc, 0, 0);
	}
	else
	{
		MazePaint(hwnd, hdc, Num - yMax, Num - xMax);
		RolePaint(hwnd, hdc, xMax - 1, yMax - 1);
	}
}