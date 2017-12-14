#pragma once
#include <windows.h>

#define Width 50
#define Height 50

#define Num 16
#define xMax 10
#define yMax 16

#define DIRECTION_NUM 4
#define DIRECTION_EAST 0
#define DIRECTION_SOUTH 1
#define DIRECTION_WEST 2
#define DIRECTION_NORTH 3

#define FOOD 2
#define Permitted 1
#define Forbidden 0

#define Role1 3
#define Role2 4

#define CHILDWND_ID_0 0
#define CHILDWND_ID_1 1

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define FLAG_DEFAULT 4

#define GI_X 0
#define GI_Y 1

#define GI_FLAG 2
#define GI_PREFLAG 3

typedef struct cell
{
	BOOL door[DIRECTION_NUM];
} cell;

class Maze_Creating
{
public:
	Maze_Creating();
	void Union(int c1, int c2);
	int Find(int c1);
	BOOL is_Connected(int c1, int c2);
	BOOL All_Connected();
	void Path_Creating();

	cell Block[Num / 2][Num / 2];	//�հ׿飨ÿ��Ԫ�ؼ�¼��Ԫ������ΧԪ�ص���ͨ״̬��

private:
	int Block_Tree[(Num / 2) * (Num / 2)];	//�հ׿������ȼۼ���
};

//�Թ���
class Maze {
public:
	Maze(int X0, int Y0, int X1, int Y1);
	void Maze_Fill();	//�����Թ��������
	void HDC_Init(HDC hdc);
	void MazePaint(HWND hwnd, HDC hdc, int y, int x);
	BOOL InPath(HWND hwnd, int Flag);
	void MazeUpdate_Data(HWND hwnd, int oldX, int oldY, int newX, int newY);
	void MazeUpdate_Wnd_Part(HWND hwnd, HDC hdc, int oldX, int oldY, int newX, int newY, int PreFlag);
	void MazeUpdate_ChildWnd(HWND hwnd, HDC hdc);	//�Ӵ��ڵ�ˢ�¿���
	void HInstance_Init(HWND hwnd);	//�����ڵ��ã���������ʵ�����
	void MazeUpdate_Wnd_Full(HWND hwnd, HDC hdc);	//�ͻ�����ǿ��ȫ�����ƣ�����ԭ��ˢ��
	int GetInfo(HWND hwnd, int GI_ID);
	void SetInfo(HWND hwnd, int Data, int GI_ID);
	void RolePaint(HWND hwnd, HDC hdc, int x, int y);

private:
	int FullMaze[Num][Num];	//�����Թ�
	/*int Block_Tree[Num * (Num / 2)];*/
	int x0, y0;
	int x1, y1;
	int Flag_Food;
	int Flag0, PreFlag0;
	int Flag1, PreFlag1;
	HBITMAP hBitmap;
	HDC PermittedHdc, ForbiddenHdc, roleHdc1, roleHdc2, FoodHdc;
	HINSTANCE HInstance;

	Maze_Creating demo;
};