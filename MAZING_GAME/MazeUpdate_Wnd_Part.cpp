//更新窗口内容
#include "INI.h"

extern int FullMaze[Num][Num];
extern HDC PermittedHdc;
extern HDC ForbiddenHdc;

void MazeUpdate_Wnd_Part(HWND hwnd, HDC hdc, int oldX, int oldY, int newX, int newY, int PreFlag)
{
	int relX = 0, relY = 0;		//相对当前客户区左上角的坐标

	if (oldX == newX && oldY == newY)		//原地刷新
	{
		switch (PreFlag)
		{
		case LEFT:
			/*MazeUpdate_Wnd_Part(hwnd, hdc, oldX + 1, oldY, oldX, oldY, PreFlag);*/
			/*MazeUpdate_Wnd_Full(hwnd, hdc);
			break;*/

		case RIGHT:
			/*MazeUpdate_Wnd_Part(hwnd, hdc, oldX - 1, oldY, oldX, oldY, PreFlag);*/
			/*MazeUpdate_Wnd_Full(hwnd, hdc);
			break;*/

		case UP:
			/*MazeUpdate_Wnd_Part(hwnd, hdc, oldX, oldY + 1, oldX, oldY, PreFlag);*/
			/*MazeUpdate_Wnd_Full(hwnd, hdc);
			break;*/

		case DOWN:
			/*MazeUpdate_Wnd_Part(hwnd, hdc, oldX, oldY - 1, oldX, oldY, PreFlag);*/
			/*MazeUpdate_Wnd_Full(hwnd, hdc);
			break;*/
			MazeUpdate_Wnd_Full(hwnd, hdc);
			break;

		default:
			MazePaint_INI(hwnd, hdc);
			break;
		}
	}
	else
	{
		if ((newX >= xMax / 2 + 1 && newX <= Num - xMax / 2 - 1) || (newY >= yMax / 2 + 1 && newY <= Num - yMax / 2 - 1))
		{
			if ((newX >= xMax / 2 + 1 && newX <= Num - xMax / 2 - 1) && !(newY >= yMax / 2 + 1 && newY <= Num - yMax / 2 - 1))	//由x的变化引起
			{
				if (newY <= yMax / 2)
				{
					relX = xMax / 2;
					relY = newY;
					MazePaint(hwnd, hdc, 0, newX - xMax / 2);
					/*RolePaint(hwnd, hdc, relX, relY);*/
				}
				else
				{
					relX = xMax / 2;
					relY = newY - Num + yMax;
					MazePaint(hwnd, hdc, Num - yMax, newX - xMax / 2);
					/*RolePaint(hwnd, hdc, relX, relY);*/
				}
			}
			else if (!(newX >= xMax / 2 + 1 && newX <= Num - xMax / 2 - 1) && (newY >= yMax / 2 + 1 && newY <= Num - yMax / 2 - 1))	//由y的变化引起
			{
				if (newX <= xMax / 2)
				{
					relX = newX;
					relY = yMax / 2;
					MazePaint(hwnd, hdc, newY - yMax / 2, 0);
					/*RolePaint(hwnd, hdc, relX, relY);*/
				}
				else
				{
					relX = newX - Num + xMax;
					relY = yMax / 2;
					MazePaint(hwnd, hdc, newY - yMax / 2, Num - xMax);
					/*RolePaint(hwnd, hdc, relX, relY);*/
				}
			}
			else															//x和y都落在该区间内
			{
				relX = xMax / 2;
				relY = yMax / 2;
				MazePaint(hwnd, hdc, newY - yMax / 2, newX - xMax / 2);
				/*RolePaint(hwnd, hdc, relX, relY);*/
			}
		}
		else if (newX == xMax / 2 || newX == Num - xMax / 2 || newY == yMax / 2 || newY == Num - yMax / 2) //踩线刷新
		{
			if ((newX == xMax / 2 || newX == Num - xMax / 2))
			{
				if ((newX == xMax / 2) && !(newX == Num - xMax / 2))
				{
					relX = xMax / 2;
					relY = newY;
					if (newY <= yMax / 2)
					{
						relX = newX;
						relY = newY;
						MazePaint(hwnd, hdc, 0, newX - xMax / 2);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
					/*else if (newY >= Num - Max)
						MazePaint(hwnd, hdc, Num - Max, newX - Max / 2);
					else
						MazePaint(hwnd, hdc, newY - Max / 2, newX - Max / 2);*/
					else
					{
						relX = xMax / 2;
						relY = newY - Num + yMax;
						MazePaint(hwnd, hdc, Num - yMax, newX - xMax / 2);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
				}
				else
				{
					relX = xMax / 2;
					relY = newY;
					if (newY <= yMax / 2)
					{
						relX = xMax / 2;
						relY = newY;
						MazePaint(hwnd, hdc, 0, newX - xMax / 2);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
					/*else if (newY >= Num - Max)
						MazePaint(hwnd, hdc, Num - Max, newX - Max / 2);
					else
						MazePaint(hwnd, hdc, newY - Max / 2, newX - Max / 2);*/
					else
					{
						relX = xMax / 2;
						relY = newY - Num + yMax;
						MazePaint(hwnd, hdc, Num - yMax, newX - xMax / 2);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
					/*RolePaint(hwnd, hdc, relX, relY);*/
				}
			}
			else if ((newY == yMax / 2 || newY == Num - yMax / 2))
			{
				if ((newY == yMax / 2) && !(newY == Num - yMax / 2))
				{
					relX = newX;
					relY = yMax / 2;
					if (newX <= xMax / 2)
					{
						relX = newX;
						relY = yMax / 2;
						MazePaint(hwnd, hdc, newY - yMax / 2, 0);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
					/*else if (newX >= Num - Max)
						MazePaint(hwnd, hdc, newY - Max / 2, Num - Max);
					else
						MazePaint(hwnd, hdc, newY - Max / 2, newX - Max / 2);*/
					else
					{
						relX = newX - Num + xMax;
						relY = yMax / 2;
						MazePaint(hwnd, hdc, newY - yMax / 2, Num - xMax);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
				}
				else
				{
					relX = newX;
					relY = yMax / 2;
					if (newX <= xMax / 2)
					{
						relX = newX;
						relY = yMax / 2;
						MazePaint(hwnd, hdc, newY - yMax / 2, 0);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
					/*else if (newX >= Num - Max)
						MazePaint(hwnd, hdc, newY - Max / 2, Num - Max);
					else
						MazePaint(hwnd, hdc, newY - Max / 2, newX - Max / 2);*/
					else
					{
						relX = newX - Num + xMax;
						relY = yMax / 2;
						MazePaint(hwnd, hdc, newY - yMax / 2, Num - xMax);
						/*RolePaint(hwnd, hdc, relX, relY);*/
					}
				}
			}
		}
		else			//在最外层的刷新（newX与newY在此处只用作判断）
		{
			if (newX < xMax / 2)
			{
				if (newY < yMax / 2)
				{
					/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Rectangle(hdc, oldX*Width, oldY*Height, (oldX + 1)*Width, (oldY + 1)*Height);*/
					BitBlt(hdc, oldX*Width, oldY*Height, Width, Height, PermittedHdc, 0, 0, SRCCOPY);
					RolePaint(hwnd, hdc, newX, newY);
				}
				else
				{
					/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Rectangle(hdc, oldX*Width, (oldY - Num + Max)*Height, (oldX + 1)*Width, (oldY - Num + Max + 1)*Height);*/
					BitBlt(hdc, oldX*Width, (oldY - Num + yMax)*Height, Width, Height, PermittedHdc, 0, 0, SRCCOPY);
					RolePaint(hwnd, hdc, newX, newY - Num + yMax);
				}
			}
			else
			{
				if (newY < yMax / 2)
				{
					/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Rectangle(hdc, (oldX - Num + Max)*Width, oldY*Height, (oldX - Num + Max + 1)*Width, (oldY + 1)*Height);*/
					BitBlt(hdc, (oldX - Num + xMax)*Width, oldY*Height, Width, Height, PermittedHdc, 0, 0, SRCCOPY);
					RolePaint(hwnd, hdc, newX - Num + xMax, newY);
				}
				else
				{
					/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
					Rectangle(hdc, (oldX - Num + Max)*Width, (oldY - Num + Max)*Height, (oldX - Num + Max + 1)*Width, (oldY - Num + Max + 1)*Height);*/
					BitBlt(hdc, (oldX - Num + xMax)*Width, (oldY - Num + yMax)*Height, Width, Height, PermittedHdc, 0, 0, SRCCOPY);
					RolePaint(hwnd, hdc, newX - Num + xMax, newY - Num + yMax);
				}
			}
			/*SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			Rectangle(hdc, oldX*Width, oldY*Height, (oldX + 1)*Width, (oldY + 1)*Height);
			RolePaint(hwnd, hdc, newX, newY);*/
		}
	}
}