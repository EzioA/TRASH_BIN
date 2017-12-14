//更新地图数据
#include "INI.h"

extern int FullMaze[Num][Num];

void MazeUpdate_Data(int oldX, int oldY, int newX, int newY)
{
	int temp;

	temp = FullMaze[oldY][oldX];
	FullMaze[oldY][oldX] = FullMaze[newY][newX];
	FullMaze[newY][newX] = temp;
}