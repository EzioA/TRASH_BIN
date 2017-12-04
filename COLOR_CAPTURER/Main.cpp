#include <windows.h>
#include <strsafe.h>
#define ID_TIMER 0

HMODULE hinstDll;
void(*SetHook)();
void(*DelHook)();
void(*SetHWINMAIN)(HWND);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

	hwnd = CreateWindow(szAppName,
		TEXT("Hello!"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0,
		0,
		300,
		300,
		0,
		0,
		hInstance,
		NULL);

	//ShowWindow(hwnd, iCmdShow);
	//UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static TEXTMETRIC tm;
	static int xChar, yChar;
	static int cxClient, cyClient;
	TCHAR szText[10];
	POINT pt;
	COLORREF color;
	unsigned char R_G_B[3];
	HBRUSH tempBrush;
	RECT rectDisplay;

	HDC clientHdc;
	static HDC screenHdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	switch (message)
	{
	case WM_CREATE:
		clientHdc = GetDC(hwnd);

		hinstDll = LoadLibrary(TEXT("SHORTCUTS_DLL.dll"));

		SetHook = (void(*)())GetProcAddress(hinstDll, "SetHook");
		DelHook = (void(*)())GetProcAddress(hinstDll, "DelHook");
		SetHWINMAIN = (void(*)(HWND))GetProcAddress(hinstDll, "SetHWINMAIN");
		SetHWINMAIN(hwnd);

		SetHook();
		SetProcessDPIAware();

		GetTextMetrics(clientHdc, &tm);
		xChar = tm.tmMaxCharWidth;
		yChar = tm.tmHeight + tm.tmInternalLeading;
		SetTimer(hwnd, ID_TIMER, 1, NULL);

		ReleaseDC(hwnd, clientHdc);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		return 0;

	case WM_TIMER:
		clientHdc = GetDC(hwnd);
		
		GetCursorPos(&pt);
		rectDisplay.left = pt.x - 20 * xChar;
		rectDisplay.top = pt.y - 8 * yChar;
		rectDisplay.right = pt.x + 25 * xChar;
		rectDisplay.bottom = pt.y + 10 * yChar;

		//SetTextAlign(screenHdc, TA_LEFT | TA_TOP);				悬浮窗变向，很麻烦的功能，放弃
		//if (pt.x >= cxClient - 9 * xChar)
		//	SetTextAlign(screenHdc, GetTextAlign(screenHdc) | TA_RIGHT);
		//if (pt.y >= cyClient - yChar)
		//	SetTextAlign(screenHdc, GetTextAlign(screenHdc) | TA_BOTTOM);

		color = GetPixel(screenHdc, pt.x, pt.y);
		
		R_G_B[0] = GetRValue(color);
		R_G_B[1] = GetGValue(color);
		R_G_B[2] = GetBValue(color);
		
		tempBrush = CreateSolidBrush(color);
		SetBkColor(screenHdc, color);

		StringCchPrintf(szText, 10, TEXT("%02X %02X %02X "), R_G_B[0], R_G_B[1], R_G_B[2]);

		if (R_G_B[0] <= 127 || R_G_B[1] <= 127 || R_G_B[2] <= 127)
		{
			SetTextColor(screenHdc, RGB(255, 255, 255));
		}
		else
		{
			SetTextColor(screenHdc, RGB(0, 0, 0));
		}
		RedrawWindow(NULL, &rectDisplay, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);

		TextOut(screenHdc, pt.x + xChar, pt.y + yChar, szText, 9);

		DeleteObject(tempBrush);
		ReleaseDC(hwnd, clientHdc);
		return 0;

	case WM_PAINT:
		clientHdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DeleteDC(screenHdc);
		DelHook();
		FreeLibrary(hinstDll);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}