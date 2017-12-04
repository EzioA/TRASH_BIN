#include <Windows.h>
#include <strsafe.h>

#define ID_TIMER 0

unsigned char R_G_B[3];
COLORREF color;
HHOOK hhookKeyboard;
BOOL limitCommon = FALSE;	//用于防止响应WM_KEYUP
BOOL timer = TRUE;
HWND hWinMain;

void SetColor();

EXTERN_C _declspec(dllexport) LRESULT  CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBuffer[10];

	if (nCode == HC_ACTION)
	{
		if (GetKeyState(VK_LCONTROL) < 0 || GetKeyState(VK_RCONTROL) < 0)
		{
			switch (wParam)
			{
			case VK_OEM_4:	//按下[键，保存当前颜色
				SetColor();
				/*MessageBeep(0);*/

				break;

			case VK_OEM_6:	//按下]键，取色
				/*MessageBox(NULL, szBuffer, TEXT("The Color Stored"), MB_OK);*/
				if (!limitCommon)
				{
					R_G_B[0] = GetRValue(color);
					R_G_B[1] = GetGValue(color);
					R_G_B[2] = GetBValue(color);

					StringCchPrintf(szBuffer, 10, TEXT("#%02X %02X %02X"), R_G_B[0], R_G_B[1], R_G_B[2]);

					limitCommon = TRUE;
					MessageBox(GetDesktopWindow(), szBuffer, TEXT("The Color Stored"), MB_OK);
				}
				else
					limitCommon = FALSE;

				break;

			case VK_OEM_5:	//按下\键，取消或设置计时器
				if (!limitCommon)
				{
					if (timer)
					{
						timer = FALSE;
						KillTimer(hWinMain, ID_TIMER);
					}
					else
					{
						timer = TRUE;
						SetTimer(hWinMain, ID_TIMER, 1, NULL);
					}
					limitCommon = TRUE;
				}
				else
					limitCommon = FALSE;

				break;
			}
		}
	}
	return CallNextHookEx(hhookKeyboard, nCode, wParam, lParam);
}

EXTERN_C _declspec(dllexport) void SetHook()
{
	hhookKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(TEXT("SHORTCUTS_DLL.dll")), 0);
}

EXTERN_C _declspec(dllexport) void DelHook()
{
	UnhookWindowsHookEx(hhookKeyboard);
}

void SetColor()
{
	HDC screenHdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	POINT pt;
	GetCursorPos(&pt);
	color = GetPixel(screenHdc, pt.x, pt.y);
	DeleteDC(screenHdc);
}

EXTERN_C _declspec(dllexport) void SetHWINMAIN(HWND hwnd)
{
	hWinMain = hwnd;
}