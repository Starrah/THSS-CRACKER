#include "define.h"
#include "evaluate.h"
#include "gameover.h"

/*
描述：判断当前位置棋型是否胜利
参数：被判断的棋类型, 棋型
返回：是否
*/
bool JudgeOneShapeSuccess(int Evaluated, ChessShape TheShape)
{
	//11111 10000000
	if (TheShape.TotalNum == 5
		&& TheShape.List[0] == Evaluated
		&& TheShape.List[1] == Evaluated
		&& TheShape.List[2] == Evaluated
		&& TheShape.List[3] == Evaluated
		&& TheShape.List[4] == Evaluated)
	{
		return true;
	}
	return false;
}

/*
描述：判断游戏结束
参数：无
返回：0：没赢 -1：和棋 1：黑胜利 2：白胜利
*/
int JudgeWin()
{
	for (int i = 0; i < PlayerList.size(); i++)
	{
		int x = PlayerList[i].x;
		int y = PlayerList[i].y;
		for (int Direction = 0; Direction < 4; Direction++)
		{
			ChessShape Shape;
			Shape.TotalNum = 5;

			//bias位置
			//获取当前棋型
			for (int bias = 0; bias < 5; bias++)
			{
				int NewX = x + bias * DirectionX[Direction];
				int NewY = y + bias * DirectionY[Direction];
				Shape.List[bias] = GetPlace(NewX, NewY);
			}
			if (JudgeOneShapeSuccess(Player, Shape))
			{
				return Player;
			}
		}
	}

	for (int i = 0; i < ComputerList.size(); i++)
	{
		int x = ComputerList[i].x;
		int y = ComputerList[i].y;
		for (int Direction = 0; Direction < 4; Direction++)
		{
			ChessShape Shape;
			Shape.TotalNum = 5;

			//bias位置
			//获取当前棋型
			for (int bias = 0; bias < 5; bias++)
			{
				int NewX = x + bias * DirectionX[Direction];
				int NewY = y + bias * DirectionY[Direction];
				Shape.List[bias] = GetPlace(NewX, NewY);
			}
			if (JudgeOneShapeSuccess(Computer, Shape))
			{
				return Computer;
			}
		}
	}
	if (ComputerList.size() + PlayerList.size() >= TOTAL_PLACE)
	{
		return -1;
	}
	return 0;
}
