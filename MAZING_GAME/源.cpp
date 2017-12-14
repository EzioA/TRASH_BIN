#include <strsafe.h>
#include <windows.h>
#include "Init.h"
#include "resource.h"

//要设置子窗口，在WM_KEYDOWN中配置：按下WASD时激活子窗口1，按下方向键时激活子窗口2
//窗口更新函数也要改动：更新时将另一角色显示出来

HWND hwndChild[2];
static TCHAR szChildClass[] = TEXT("ChildWindow");
TCHAR szCaption[] = TEXT("啊哈！！");
TCHAR szBegin[] = TEXT("和小伙伴一起踏上寻找狗粮的道路吧！另外，小心不要掉到火坑里去哦！");
TCHAR szFire[] = TEXT("啊哦，游戏结束了，狗粮只能被小伙伴独吞了！");
TCHAR szWin[] = TEXT("恭喜你们！一起分享狗粮吧！");
Maze MAZE(2, 2, Num - 2, Num - 2);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MyWindows");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("这个程序需要在 Windows NT 才能执行！"), szAppName, MB_ICONERROR);
		return 0;
	}

	wndclass.hIcon = NULL;
	wndclass.lpfnWndProc = ChildProc;
	/*wndclass.cbWndExtra = sizeof(long);*/
	wndclass.lpszClassName = szChildClass;
	RegisterClass(&wndclass);

	hwnd = CreateWindow(szAppName,
		TEXT("Hello!"),
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1066,
		839,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	/*PAINTSTRUCT ps;
	RECT rect;*/
	/*static TCHAR szBuffer[20];*/
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_CREATE:
		//初始化角色、迷宫数据
		//初始化子窗口
		//不负责绘图

		hdc = GetDC(hwnd);

		MessageBox(hwnd, szBegin, szCaption, MB_OK);
		/*HInstance = ((LPCREATESTRUCT)lParam)->hInstance;*/

		hwndChild[0] = CreateWindow(szChildClass,
			NULL,
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd,
			(HMENU)(CHILDWND_ID_0),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE),
			NULL);

		hwndChild[1] = CreateWindow(szChildClass,
			NULL,
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd,
			(HMENU)(CHILDWND_ID_1),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWL_HINSTANCE),
			NULL);
		/*HInstance = ((LPCREATESTRUCT)lParam)->hInstance;*/
		MAZE.Maze_Fill();
		MAZE.HInstance_Init(hwnd);
		MAZE.HDC_Init(hdc);
		/*RoleINI(hdc);*/
		
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		MoveWindow(hwndChild[0], 0, 0, 500, cyClient, TRUE);
		MoveWindow(hwndChild[1], 500 + Width, 0, cxClient / 2, cyClient, TRUE);

		return 0;

	case WM_KEYDOWN:
		hdc = GetDC(hwndChild[1]);
		switch (wParam)
		{
		case VK_UP:
			if (MAZE.InPath(hwndChild[1], UP))
			{
				/*MazeUpdate_Data(x2, y2, x2, y2 - 1);
				MazeUpdate_Wnd_Part(hwndChild[1], hdc, x2, y2, x2, y2 - 1);*/
				/*NewX = x1;
				NewY = y1 - 1;*/
				MAZE.SetInfo(hwndChild[1], UP, GI_FLAG);
				SetFocus(hwndChild[1]);
				/*MAZE.SetInfo(hwndChild[1], MAZE.GetInfo(hwndChild[1], GI_Y) - 1, GI_Y);*/
			}
			break;

		case VK_DOWN:
			if (MAZE.InPath(hwndChild[1], DOWN))
			{
				/*MazeUpdate_Data(x2, y2, x2, y2 + 1);
				MazeUpdate_Wnd_Part(hwndChild[1], hdc, x2, y2, x2, y2 + 1);*/
				/*NewX = x1;
				NewY = y1 + 1;*/
				MAZE.SetInfo(hwndChild[1], DOWN, GI_FLAG);
				SetFocus(hwndChild[1]);
				/*MAZE.SetInfo(hwndChild[1], MAZE.GetInfo(hwndChild[1], GI_Y) + 1, GI_Y);*/
			}
			break;

		case VK_LEFT:
			if (MAZE.InPath(hwndChild[1], LEFT))
			{
				/*MazeUpdate_Data(x2, y2, x2 - 1, y2);
				MazeUpdate_Wnd_Part(hwndChild[1], hdc, x2, y2, x2 - 1, y2);*/
				/*NewX = x1 - 1;
				NewY = y1;*/
				MAZE.SetInfo(hwndChild[1], LEFT, GI_FLAG);
				SetFocus(hwndChild[1]);
				/*MAZE.SetInfo(hwndChild[1], MAZE.GetInfo(hwndChild[1], GI_X) - 1, GI_X);*/
			}
			break;

		case VK_RIGHT:
			if (MAZE.InPath(hwndChild[1], RIGHT))
			{
				/*MazeUpdate_Data(x2, y2, x2 + 1, y2);
				MazeUpdate_Wnd_Part(hwndChild[1], hdc, x2, y2, x2 + 1, y2);*/
				/*NewX = x1 + 1;
				NewY = y1;*/
				MAZE.SetInfo(hwndChild[1], RIGHT, GI_FLAG);
				SetFocus(hwndChild[1]);
				/*MAZE.SetInfo(hwndChild[1], MAZE.GetInfo(hwndChild[1], GI_X) + 1, GI_X);*/
			}
			break;
		}
		SetFocus(hwndChild[0]);	//使子窗口0原地刷新
		ReleaseDC(hwndChild[1], hdc);
		return 0;

	case WM_CHAR:
		switch (wParam)
		{
		case 'w':
		case 'W':
			if (MAZE.InPath(hwndChild[0], UP))
			{
				MAZE.SetInfo(hwndChild[0], UP, GI_FLAG);
				SetFocus(hwndChild[0]);
				/*MAZE.SetInfo(hwndChild[0], MAZE.GetInfo(hwndChild[0], GI_Y) - 1, GI_Y);*/
			}
			break;

		case 'a':
		case 'A':
			if (MAZE.InPath(hwndChild[0], LEFT))
			{
				MAZE.SetInfo(hwndChild[0], LEFT, GI_FLAG);
				SetFocus(hwndChild[0]);
				/*MAZE.SetInfo(hwndChild[0], MAZE.GetInfo(hwndChild[0], GI_X) - 1, GI_X);*/
			}
			break;

		case 's':
		case 'S':
			if (MAZE.InPath(hwndChild[0], DOWN))
			{
				MAZE.SetInfo(hwndChild[0], DOWN, GI_FLAG);
				SetFocus(hwndChild[0]);
				/*MAZE.SetInfo(hwndChild[0], MAZE.GetInfo(hwndChild[0], GI_Y) + 1, GI_Y);*/
			}
			break;

		case 'd':
		case 'D':
			if (MAZE.InPath(hwndChild[0], RIGHT))
			{
				MAZE.SetInfo(hwndChild[0], RIGHT, GI_FLAG);
				SetFocus(hwndChild[0]);
				/*MAZE.SetInfo(hwndChild[0], MAZE.GetInfo(hwndChild[0], GI_X) + 1, GI_X);*/
			}
			break;
		}
		SetFocus(hwndChild[1]);		//使子窗口1原地刷新
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	/*int x, y;*/

	switch (message)
	{
	case WM_SETFOCUS:
		hdc = GetDC(hwnd);

		MAZE.MazeUpdate_ChildWnd(hwnd, hdc);

		ReleaseDC(hwnd, hdc);
		SetFocus(GetParent(hwnd));
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		/*if (GetWindowLongPtr(hwnd, GWL_ID) == CHILDWND_ID_0)
		{
			MazePaint(hwnd, hdc, 0, 0);
			RolePaint(hwnd, hdc, 0, 0);
		}
		else
		{
			MazePaint(hwnd, hdc, Num - yMax, Num - xMax);
			RolePaint(hwnd, hdc, xMax - 1, yMax - 1);
		}*/
		MAZE.MazeUpdate_Wnd_Full(hwnd, hdc);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}