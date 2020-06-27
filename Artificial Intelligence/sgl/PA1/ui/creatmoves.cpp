#include "define.h"
#include "createmoves.h"

/*
描述：判断当前位置是否在优先集合里搜索过了
参数：x,y，优先集合
返回：是否
*/
bool JudgeInPriorityList(int x, int y, std::vector<Point> PriorityList)
{
	for (int i = 0; i < PriorityList.size(); i++)
	{
		if (x == PriorityList[i].x && y == PriorityList[i].y)
		{
			return true;
		}
	}
	return false;
}

/*
描述：获取能移动的位置集合，没有优先搜索和排除不太可行位置的规则
参数：无
返回：元素集合
*/
vector<Point> CreateMovesNaive()
{
	vector<Point> MoveList;
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			if (chessBoard[i][j] == blank)
			{
				Point NewPoint(i, j);
				MoveList.push_back(NewPoint);
			}
		}
	}
	return MoveList;
}

/*
描述：获取能移动的位置集合，没有优先搜索，但是排除不太可行位置
参数：无
返回：元素集合
*/
vector<Point> CreateMovesWithNeighbor()
{
	vector<Point> MoveList;
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			if (chessBoard[i][j] == blank && JudgeNeighbor(i, j))
			{
				Point NewPoint(i, j);
				MoveList.push_back(NewPoint);
			}
		}
	}
	return MoveList;
}

/*
描述：获取能移动的位置集合，优先搜索上次落子周围的位置，排除周围没有任何棋子的位置
参数：无
返回：元素集合
*/
vector<Point> CreateMoves()
{
	//先查找最近一次自己/对方落子之后的位置，优先
	vector<Point> PriorityList;
	if (ComputerList.size() > 0)
	{
		int x = ComputerList[ComputerList.size() - 1].x;
		int y = ComputerList[ComputerList.size() - 1].y;
		for (int Direction = 0; Direction < 4; Direction++)
		{
			int Newx = x + DirectionX[Direction];
			int Newy = y + DirectionY[Direction];
			if (GetPlace(Newx, Newy) == blank)
			{
				Point NewPoint(Newx, Newy);
				PriorityList.push_back(NewPoint);
			}
			Newx = x - DirectionX[Direction];
			Newy = y - DirectionY[Direction];
			if (GetPlace(Newx, Newy) == blank)
			{
				Point NewPoint(Newx, Newy);
				PriorityList.push_back(NewPoint);
			}
		}
	}
	if (PlayerList.size() > 0)
	{
		int x = PlayerList[PlayerList.size() - 1].x;
		int y = PlayerList[PlayerList.size() - 1].y;

		for (int Direction = 0; Direction < 4; Direction++)
		{
			int Newx = x + DirectionX[Direction];
			int Newy = y + DirectionY[Direction];
			if (GetPlace(Newx, Newy) == blank)
			{
				Point NewPoint(Newx, Newy);
				PriorityList.push_back(NewPoint);
			}
			Newx = x - DirectionX[Direction];
			Newy = y - DirectionY[Direction];
			if (GetPlace(Newx, Newy) == blank)
			{
				Point NewPoint(Newx, Newy);
				PriorityList.push_back(NewPoint);
			}
		}
	}

	//再加入其他位置,排除之前搜索过的和周围没有其他落子的
	vector<Point> OtherList;
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			if (chessBoard[i][j] == blank && JudgeNeighbor(i, j)
				&& JudgeInPriorityList(i, j, PriorityList) == false
				)
			{
				Point NewPoint(i, j);
				OtherList.push_back(NewPoint);
			}
		}
	}

	//合并
	vector<Point> MoveList;
	for (int i = 1; i < PriorityList.size(); i++)
	{
		MoveList.push_back(PriorityList[i]);
	}
	for (int i = 1; i < OtherList.size(); i++)
	{
		MoveList.push_back(OtherList[i]);
	}
	return MoveList;
}