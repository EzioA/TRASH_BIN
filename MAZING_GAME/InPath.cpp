//判断是否在路径上
#include "INI.h"

extern int FullMaze[Num][Num];

BOOL InPath(int x, int y)
{
	if (x < 0 || x >= Num || y < 0 || y >= Num)
		return FALSE;
	else
	{
		if (*(*(FullMaze + y) + x) == Permitted)
			return TRUE;
		else
			return FALSE;
	}
}