#include "INI.h"

HDC roleHdc;
HBITMAP hBitmap;

//�ڵ�ǰ�ͻ��������������ƽ�ɫ

void Maze::RolePaint(HWND hwnd, HDC hdc, int x,int y)
{
	/*roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);*/
	BitBlt(hdc, x*Width, y*Height, Width, Height, roleHdc, 0, 0, SRCCOPY);
}