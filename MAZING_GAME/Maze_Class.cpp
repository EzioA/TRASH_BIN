#include "Init.h"
#include "resource.h"

extern TCHAR szCaption[];
extern TCHAR szBegin[];
extern TCHAR szFire[];
extern TCHAR szWin[];
extern HWND hwndChild[2];

Maze::Maze(int X0, int Y0, int X1, int Y1):x0(X0), y0(Y0), x1(X1), y1(Y1), demo()
{
	Flag0 = Flag1 = FLAG_DEFAULT;
	PreFlag0 = PreFlag1 = FLAG_DEFAULT;
	Flag_Food = TRUE;
}

BOOL Maze::InPath(HWND hwnd, int Flag)
{
	int x, y;

	x = GetInfo(hwnd, GI_X);
	y = GetInfo(hwnd, GI_Y);

	switch (Flag)
	{
	case UP:
		y--;
		break;

	case DOWN:
		y++;
		break;

	case LEFT:
		x--;
		break;

	case RIGHT:
		x++;
		break;
	}

	if (x < 0 || x >= Num || y < 0 || y >= Num)
		return FALSE;
	else			//踩到可行走路线
	{
		if (*(*(FullMaze + y) + x) == Permitted)
			return TRUE;
		else
		{
			if (*(*(FullMaze + y) + x) == Forbidden)		//踩火
			{
				MessageBox(GetParent(hwnd), szFire, szCaption, MB_OK);
				SendMessage(GetParent(hwnd), WM_DESTROY, 0, 0);
			}
			else if (*(*(FullMaze + y) + x) == FOOD)		//找到狗粮
			{
				*(*(FullMaze + y) + x) = Permitted;
				Flag_Food = FALSE;
				return TRUE;
			}
			else if ((*(*(FullMaze + y) + x) == Role1 || *(*(FullMaze + y) + x) == Role2) && Flag_Food == FALSE)	//分享狗粮
			{
				MessageBox(GetParent(hwnd), szWin, szCaption, MB_OK);
				SendMessage(GetParent(hwnd), WM_DESTROY, 0, 0);
			}
			else
				return FALSE;
		}
	}
}

void Maze::Maze_Fill()
{
	int i, k;
	int index_col, index_row;
	int x, y;

	demo.Path_Creating();

	for (i = 0; i < Num; i++)		//整个迷宫禁止通行
	{
		for (k = 0; k < Num; k++)
		{
			FullMaze[i][k] = Forbidden;
		}
	}

	for (i = 0, index_row = 0; i < Num; i += 2, index_row++)	//根据demo的数据进行填充
	{
		for (k = 0, index_col = 0; k < Num; k += 2, index_col++)
		{
			FullMaze[i][k] = Permitted;
			if (i == Num / 2 && k == Num / 2)		//设置食物
			{
				FullMaze[i][k] = FOOD;
			}
			if (demo.Block[index_row][index_col].door[DIRECTION_EAST] && k + 1 < Num)
				FullMaze[i][k+1] = Permitted;
			if (demo.Block[index_row][index_col].door[DIRECTION_SOUTH] && i + 1 < Num)
				FullMaze[i+1][k] = Permitted;
		}
	}

	x = GetInfo(hwndChild[0], GI_X);
	y = GetInfo(hwndChild[0], GI_Y);
	FullMaze[x][y] = Role1;

	x = GetInfo(hwndChild[1], GI_X);
	y = GetInfo(hwndChild[1], GI_Y);
	FullMaze[x][y] = Role2;

	/*HInstance = hInstance;

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP2));

	PermittedHdc = CreateCompatibleDC(hdc);
	SelectObject(PermittedHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP3));

	ForbiddenHdc = CreateCompatibleDC(hdc);
	SelectObject(ForbiddenHdc, hBitmap);*/
}

void Maze::HDC_Init(HDC hdc)
{

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	roleHdc1 = CreateCompatibleDC(hdc);
	SelectObject(roleHdc1, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	roleHdc2 = CreateCompatibleDC(hdc);
	SelectObject(roleHdc2, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP2));

	PermittedHdc = CreateCompatibleDC(hdc);
	SelectObject(PermittedHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP3));

	ForbiddenHdc = CreateCompatibleDC(hdc);
	SelectObject(ForbiddenHdc, hBitmap);

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP4));

	FoodHdc = CreateCompatibleDC(hdc);
	SelectObject(FoodHdc, hBitmap);
}

void Maze::MazeUpdate_Data(HWND hwnd, int oldX, int oldY, int newX, int newY)
{
	int temp;

	temp = FullMaze[oldY][oldX];
	FullMaze[oldY][oldX] = FullMaze[newY][newX];
	FullMaze[newY][newX] = temp;
}

void Maze::MazeUpdate_ChildWnd(HWND hwnd, HDC hdc)
{
	int ptFlag;
	int ptPreFlag;
	int x, y;

	//if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
	//{
	//	ptFlag = GetInfo(hwnd, GI_FLAG);
	//	ptPreFlag = GetInfo(hwnd, GI_PREFLAG);
	//}
	//else
	//{
	//	ptFlag = &Flag1;
	//	ptPreFlag = &PreFlag1;
	//}
	

	/*hdc = GetDC(hwnd);

	x = GetWindowLongPtr(hwnd, 0) & 0xFF;
	y = GetWindowLongPtr(hwnd, 0) >> 8;*/
	ptFlag = GetInfo(hwnd, GI_FLAG);
	ptPreFlag = GetInfo(hwnd, GI_PREFLAG);
	x = GetInfo(hwnd, GI_X);
	y = GetInfo(hwnd, GI_Y);

	switch (ptFlag)
	{
	case LEFT:
		MazeUpdate_Data(hwnd, x, y, x - 1, y);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x - 1, y, ptPreFlag);
		x = x - 1;
		/*SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));*/
		ptPreFlag = ptFlag;
		ptFlag = FLAG_DEFAULT;
		break;

	case RIGHT:
		MazeUpdate_Data(hwnd, x, y, x + 1, y);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x + 1, y, ptPreFlag);
		x = x + 1;
		/*SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));*/
		ptPreFlag = ptFlag;
		ptFlag = FLAG_DEFAULT;
		break;

	case UP:
		MazeUpdate_Data(hwnd, x, y, x, y - 1);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y - 1, ptPreFlag);
		y = y - 1;
		/*SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));*/
		ptPreFlag = ptFlag;
		ptFlag = FLAG_DEFAULT;
		break;

	case DOWN:
		MazeUpdate_Data(hwnd, x, y, x, y + 1);
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y + 1, ptPreFlag);
		y = y + 1;
		/*SetWindowLongPtr(hwnd, 0, (LONG)(y << 8 | x));*/
		ptPreFlag = ptFlag;
		ptFlag = FLAG_DEFAULT;
		break;

	default:
		MazeUpdate_Wnd_Part(hwnd, hdc, x, y, x, y, ptPreFlag);	//原地刷新
		break;
	}
	SetInfo(hwnd, ptFlag, GI_FLAG);
	SetInfo(hwnd, ptPreFlag, GI_PREFLAG);
	SetInfo(hwnd, x, GI_X);
	SetInfo(hwnd, y, GI_Y);
}

void Maze::MazeUpdate_Wnd_Part(HWND hwnd, HDC hdc, int oldX, int oldY, int newX, int newY, int PreFlag)
{
	int relX = 0, relY = 0;		//相对当前客户区左上角的坐标

	if (oldX == newX && oldY == newY)		//原地刷新
	{
		switch (PreFlag)		//此处使用switch语句没什么实际意义，仅为记录旧版本的思路
		{
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			/*MazeUpdate_Wnd_Part(hwnd, hdc, oldX, oldY - 1, oldX, oldY, PreFlag);*/
			/*MazeUpdate_Wnd_Full(hwnd, hdc);
			break;*/
			MazeUpdate_Wnd_Full(hwnd, hdc);
			break;

		default:
			/*MazePaint_INI(hwnd, hdc);*/
			MazeUpdate_Wnd_Full(hwnd, hdc);
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

void Maze::MazePaint(HWND hwnd, HDC hdc, int y, int x)
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
						BitBlt(hdc, k*Width, i*Height, Width, Height, roleHdc1, 0, 0, SRCCOPY);
					else if (*(*(FullMaze + y) + x) == Role2)
						BitBlt(hdc, k*Width, i*Height, Width, Height, roleHdc2, 0, 0, SRCCOPY);
					else
						BitBlt(hdc, k*Width, i*Height, Width, Height, FoodHdc, 0, 0, SRCCOPY);
				}
			}
		}
		x = tempX;
	}
}	  //将FullMaze[y][x]映射到客户区（0， 0）处

void Maze::MazeUpdate_Wnd_Full(HWND hwnd, HDC hdc)
{
	int x, y;

	/*x = GetWindowLongPtr(hwnd, 0) & 0xFF;
	y = GetWindowLongPtr(hwnd, 0) >> 8;*/

	x = GetInfo(hwnd, GI_X);
	y = GetInfo(hwnd, GI_Y);

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

int Maze::GetInfo(HWND hwnd, int GI_ID)		//通过传入的句柄和标志判断读取哪个数据
{
	int temp;

	switch (GI_ID)
	{
	case GI_X:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			temp = x0;
		else
			temp = x1;
		break;

	case GI_Y:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			temp = y0;
		else
			temp = y1;
		break;

	case GI_FLAG:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			temp = Flag0;
		else
			temp = Flag1;
		break;

	case GI_PREFLAG:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			temp = PreFlag0;
		else
			temp = PreFlag1;
		break;
	}

	return temp;
}  

void Maze::SetInfo(HWND hwnd, int Data, int GI_ID)
{
	switch (GI_ID)
	{
	case GI_X:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			x0 = Data;
		else
			x1 = Data;
		break;

	case GI_Y:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			y0 = Data;
		else
			y1 = Data;
		break;

	case GI_FLAG:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			Flag0 = Data;
		else
			Flag1 = Data;
		break;

	case GI_PREFLAG:
		if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
			PreFlag0 = Data;
		else
			PreFlag1 = Data;
		break;
	}
}

void Maze::HInstance_Init(HWND hwnd)
{
	HInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE);
}

void Maze::RolePaint(HWND hwnd, HDC hdc, int x, int y)
{
	/*roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);*/
	if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
		BitBlt(hdc, x*Width, y*Height, Width, Height, roleHdc1, 0, 0, SRCCOPY);
	else
		BitBlt(hdc, x*Width, y*Height, Width, Height, roleHdc2, 0, 0, SRCCOPY);
}