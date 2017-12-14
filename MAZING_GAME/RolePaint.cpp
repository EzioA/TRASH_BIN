#include "INI.h"

HDC roleHdc;
HBITMAP hBitmap;

//在当前客户区内相对坐标绘制角色

void Maze::RolePaint(HWND hwnd, HDC hdc, int x,int y)
{
	/*roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);*/
	BitBlt(hdc, x*Width, y*Height, Width, Height, roleHdc, 0, 0, SRCCOPY);
}