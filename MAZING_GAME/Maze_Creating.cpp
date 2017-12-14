#include "Init.h"

Maze_Creating::Maze_Creating()
{
	int i, k, j;

	for (i = 0; i < Num / 2; i++)
	{
		for (k = 0; k < Num / 2; k++)
		{
			for (j = 0; j < DIRECTION_NUM; j++)
			{
				Block[i][k].door[j] = FALSE;
			}
		}
	}

	for (i = 0; i < (Num / 2)*(Num / 2); i++)
	{
		Block_Tree[i] = -1;
	}
}

void Maze_Creating::Path_Creating()
{
	int c1, c2;	//记录随机选中的方块
	int direction;
	int CELL_NUM = (Num / 2)*(Num / 2);
	int ROW = Num / 2;
	int COL = Num / 2;

	srand((UINT)GetCurrentTime());

	while (TRUE)
	{
		c1 = rand() % CELL_NUM;
		direction = rand() % DIRECTION_NUM;

		switch (direction)
		{
		case DIRECTION_EAST:
			if (c1 % COL == COL - 1)	//c1是某行最后一列
			{
				c2 = -1;
			}
			else
				c2 = c1 + 1;
			break;

		case DIRECTION_SOUTH:
			if (c1 / COL == ROW - 1)	//c1是最后一行
			{
				c2 = -1;
			}
			else
				c2 = c1 + COL;
			break;

		case DIRECTION_WEST:
			if (c1 % COL == 0)	//c1在某行第一列
			{
				c2 = -1;
			}
			else
				c2 = c1 - 1;
			break;

		case DIRECTION_NORTH:
			if (c1 / COL == 0)
			{
				c2 = -1;
			}
			else
				c2 = c1 - COL;
			break;
		}
		if (c2 < 0)		//c2定位失败，重新进入循环
			continue;

		if (!is_Connected(c1, c2))
		{
			Union(c1, c2);
			Block[c1 / COL][c1%COL].door[direction] = TRUE;
			Block[c2 / COL][c2%COL].door[(direction + 2) % DIRECTION_NUM] = TRUE;
		}

		//if (is_Connected(0, ROW*COL - 1))		//起点到终点已连通，退出循环
		//{
		//	break;
		//}

		if (All_Connected())
			break;
	}
}

int Maze_Creating::Find(int c1)
{
	while (Block_Tree[c1] >= 0)
		c1 = Block_Tree[c1];

	return c1;
}

BOOL Maze_Creating::is_Connected(int c1, int c2)
{
	c1 = Find(c1);
	c2 = Find(c2);

	if (c1 == c2)
		return TRUE;
	else
		return FALSE;
}

BOOL Maze_Creating::All_Connected()
{
	int i, count;	//记录根数
	int ROW = Num / 2;
	int COL = Num / 2;

	for (i = 0, count = 0; i < ROW*COL; i++)
	{
		if (Block_Tree[i] < 0)
			count++;
	}

	if (count == 1)
		return TRUE;
	else
		return FALSE;
}

void Maze_Creating::Union(int c1, int c2)
{
	c1 = Find(c1);
	c2 = Find(c2);

	if (Block_Tree[c1] < Block_Tree[c2])
		Block_Tree[c2] = c1;
	else
	{
		if (Block_Tree[c1] == Block_Tree[c2])
			Block_Tree[c2]--;
		Block_Tree[c1] = c2;
	}
}