#include "INI.h"
#include "resource.h"
//#include <time.h>
//调用迷宫构造函数生成迷宫数组

extern int FullMaze[Num][Num];
extern HINSTANCE HInstance;
extern HDC PermittedHdc;
extern HDC ForbiddenHdc;
extern HBITMAP hBitmap;

void MazeINI(HDC hdc)
{
	int i, k;
	
	///*/*/*/*srand((UINT)time(NULL));*/*/*/*/

	//srand(GetCurrentTime());
	//for (i = 0; i < row; i++)
	//{
	//	for (k = 0; k < col; k++)
	//	{
	//		*(*(FullMaze + i) + k) = rand() % 2;
	//	}
	//}

	for (i = 0; i < Num; i++)
	{
		for (k = 0; k < Num; k++)
		{
			if (i == k)
			{
				*(*(FullMaze + i) + k) = Forbidden;
			}
			else
				*(*(FullMaze + i) + k) = Permitted;
		}
	}
	**FullMaze = Role1;
	*(*(FullMaze + Num - 1) + Num - 1) = Role2;

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP2));

	PermittedHdc = CreateCompatibleDC(hdc);
	SelectObject(PermittedHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP3));

	ForbiddenHdc = CreateCompatibleDC(hdc);
	SelectObject(ForbiddenHdc, hBitmap);
}