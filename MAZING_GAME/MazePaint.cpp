//调用判断函数
//刷新视野，重绘客户区
#include "INI.h"

HDC ForbiddenHdc;
HDC PermittedHdc;
extern HDC roleHdc;
extern int FullMaze[Num][Num];

void MazePaint(HWND hwnd, HDC hdc, int y, int x)
{
	int i, k;
	int tempX = x, tempY = y;
	
	for (i = 0, y = tempY; i < yMax; i++, y++)
	{
		for (k = 0; k < xMax; k++, x++)
		{
			if (*(*(FullMaze + y) + x) == Forbidden)
			{
				/*SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				Rectangle(hdc, k*Width, i*Height, (k + 1)*Width, (i + 1)*Height);*/
				BitBlt(hdc, k*Width, i*Height, Width, Height, ForbiddenHdc, 0, 0, SRCCOPY);
			}
			else
			{
				/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
				Rectangle(hdc, k*Width, i*Height, (k + 1)*Width, (i + 1)*Height);*/
				if (*(*(FullMaze + y) + x) == Permitted)
					BitBlt(hdc, k*Width, i*Height, Width, Height, PermittedHdc, 0, 0, SRCCOPY);
				else
				{
					if (*(*(FullMaze + y) + x) == Role1)
						BitBlt(hdc, k*Width, i*Height, Width, Height, roleHdc, 0, 0, SRCCOPY);
					else
						BitBlt(hdc, k*Width, i*Height, Width, Height, roleHdc, 0, 0, SRCCOPY);
				}
			}
		}
		x = tempX;
	}
}