//ÐÂ½¨½ÇÉ«
#include "INI.h"
#include "resource.h"

extern HINSTANCE HInstance;
extern HBITMAP hBitmap;
extern HDC roleHdc;

void RoleINI(HDC hdc)
{

	hBitmap = LoadBitmap(HInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	roleHdc = CreateCompatibleDC(hdc);
	SelectObject(roleHdc, hBitmap);


	/*GetObject(hBitmap, sizeof(BITMAP), &bitmap);*/
}