#include "define.h"
#include "evaluate.h"

std::vector<ScoreShape> ScoreList;

/*
描述：计算总得分
参数：被估计的棋子颜色
返回：得分
*/
int Evaluate(int Evaluated)
{
	int PlayerScore = 0;
	int ComputerScore = 0;
	ScoreList.clear();
	for (int i = 0; i < PlayerList.size(); i++)
	{
		PlayerScore += EvaluateOnePoint(PlayerList[i].x, PlayerList[i].y, Player);
	}
	/*PrintScoreList();
	cout << "当前玩家总估值：" << PlayerScore << endl;
	cout << "--------------------------------------------" << endl;*/

	ScoreList.clear();
	for (int i = 0; i < ComputerList.size(); i++)
	{
		ComputerScore += EvaluateOnePoint(ComputerList[i].x, ComputerList[i].y, Computer);
	}
	/*PrintScoreList();
	cout << "当前电脑总估值：" << ComputerScore << endl;*/
	int TotalScore = -1;
	if (Evaluated == Player)
	{
		TotalScore = PlayerScore - 0.1 * ComputerScore;
	}
	else
	{
		TotalScore = ComputerScore - 0.1 * PlayerScore;
	}
	/*cout << "当前总估值：" << TotalScore << endl;
	cout << "--------------------------------------------" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "--------------------------------------------" << endl;*/
	return TotalScore;
}

/*
描述：计算某一点的得分
参数：当前点x,y，被估计的棋子颜色
返回：得分
*/
int EvaluateOnePoint(int x, int y, int Evaluated)
{
	int TotalScore = 0;
	//方向
	for (int Direction = 0; Direction < 4; Direction++)
	{
		//如果此方向上，该点已经有得分形状，不重复计算
		if (JudgeRepeat(x, y, DirectionX[Direction], DirectionY[Direction]))
		{
			continue;
		}
		int MaxScore = 0;
		ScoreShape CurrentMax;
		//offset偏置
		//求当前方向最大值
		for (int offset = -5; offset <= 0; offset++)
		{
			ChessShape Shape5;
			ChessShape Shape6;
			ScoreShape Score5;
			ScoreShape Score6;
			Score5.DX = DirectionX[Direction];
			Score5.DY = DirectionY[Direction];
			Score6.DX = DirectionX[Direction];
			Score6.DY = DirectionY[Direction];
			Shape5.TotalNum = 5;
			Shape6.TotalNum = 6;
			Score5.TotalNum = 5;
			Score6.TotalNum = 6;

			//bias位置
			//获取当前棋型
			for (int bias = 0; bias < 6; bias++)
			{
				int NewX = x + (bias + offset) * DirectionX[Direction];
				int NewY = y + (bias + offset) * DirectionY[Direction];
				Shape5.List[bias] = GetPlace(NewX, NewY);
				Shape6.List[bias] = GetPlace(NewX, NewY);
				Score5.List[bias] = GetPlace(NewX, NewY);
				Score6.List[bias] = GetPlace(NewX, NewY);
				Point TempPoint(NewX, NewY);
				Score5.Place[bias] = TempPoint;
				Score6.Place[bias] = TempPoint;
			}
			//估计棋型分数
			int score5 = EvaluateOneShape(Evaluated, Shape5);
			Score5.Score = score5;
			int score6 = EvaluateOneShape(Evaluated, Shape6);
			Score6.Score = score6;
			if (score5 > MaxScore)
			{
				MaxScore = score5;
				CurrentMax = Score5;
			}
			if (score6 > MaxScore)
			{
				MaxScore = score6;
				CurrentMax = Score6;
			}
		}
		if (MaxScore > 0)
		{
			//计算两个形状相交， 如两个3活 相交， 得分增加 一个子的除外
			int AddScore = CalculateAddScore(x, y, CurrentMax.DX, CurrentMax.DY, MaxScore);

			ScoreList.push_back(CurrentMax);
			TotalScore += MaxScore;
			TotalScore += AddScore;
		}
	}
	return TotalScore;
}

/*
描述：判断当前点是否被重复计算
参数：当前点x,y，方向dx，dy
返回：是否
*/
bool JudgeRepeat(int x, int y, int dx, int dy)
{
	for (int i = 0; i < ScoreList.size(); i++)
	{
		ScoreShape TheOne = ScoreList[i];
		if (TheOne.DX == dx && TheOne.DY == dy)
		{
			int Length = TheOne.TotalNum;
			for (int j = 0; j < Length; j++)
			{
				if (TheOne.Place[j].x == x && TheOne.Place[j].y == y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*
描述：判断当前点是否在其他方向有得分，有的话计算额外加分
参数：当前点x,y，方向dx，dy，当前得分
返回：额外加分
*/
int CalculateAddScore(int x, int y, int dx, int dy, int score)
{
	int AddScore = 0;
	for (int i = 0; i < ScoreList.size(); i++)
	{
		ScoreShape TheOne = ScoreList[i];
		if (TheOne.DX == dx && TheOne.DY == dy)
		{
			int Length = TheOne.TotalNum;
			for (int j = 0; j < Length; j++)
			{
				if (TheOne.Place[j].x == x && TheOne.Place[j].y == y && TheOne.Score > 0)
				{
					AddScore += score;
					AddScore += TheOne.Score;
					break;
				}
			}
		}
	}
	return AddScore;
}

/*
描述：打印得分列表
参数：当前点x,y，方向dx，dy，当前得分无
返回：无
*/
void PrintScoreList()
{
	for (int i = 0; i < ScoreList.size(); i++)
	{
		cout << "第" << i << "个得分棋型" << endl;
		cout << "总共得分：" << ScoreList[i].Score << endl;
		cout << "当前方向：" << "(" << ScoreList[i].DX << "," << ScoreList[i].DY << ")" << endl;
		if (ScoreList[i].TotalNum == 5)
		{
			cout << "得分点：" <<
				"(" << ScoreList[i].Place[0].x << "," << ScoreList[i].Place[0].y << ")" << "," <<
				"(" << ScoreList[i].Place[1].x << "," << ScoreList[i].Place[1].y << ")" << "," <<
				"(" << ScoreList[i].Place[2].x << "," << ScoreList[i].Place[2].y << ")" << "," <<
				"(" << ScoreList[i].Place[3].x << "," << ScoreList[i].Place[3].y << ")" << "," <<
				"(" << ScoreList[i].Place[4].x << "," << ScoreList[i].Place[4].y << ")" << endl;
			cout << "棋型："
				<< ScoreList[i].List[0]
				<< ScoreList[i].List[1]
				<< ScoreList[i].List[2]
				<< ScoreList[i].List[3]
				<< ScoreList[i].List[4] << endl;
		}
		else
		{
			cout << "得分点：" <<
				"(" << ScoreList[i].Place[0].x << "," << ScoreList[i].Place[0].y << ")" << "," <<
				"(" << ScoreList[i].Place[1].x << "," << ScoreList[i].Place[1].y << ")" << "," <<
				"(" << ScoreList[i].Place[2].x << "," << ScoreList[i].Place[2].y << ")" << "," <<
				"(" << ScoreList[i].Place[3].x << "," << ScoreList[i].Place[3].y << ")" << "," <<
				"(" << ScoreList[i].Place[4].x << "," << ScoreList[i].Place[4].y << ")" << "," <<
				"(" << ScoreList[i].Place[5].x << "," << ScoreList[i].Place[5].y << ")" << endl;
			cout << "棋型："
				<< ScoreList[i].List[0]
				<< ScoreList[i].List[1]
				<< ScoreList[i].List[2]
				<< ScoreList[i].List[3]
				<< ScoreList[i].List[4]
				<< ScoreList[i].List[5] << endl;
		}
		cout << "--------------------------------------------------------------------------------------------" << endl;
	}
}

/*
描述：判断当前位置棋型得分
参数：被判断的棋类型, 棋型
返回：得分
*/
int EvaluateOneShape(int Evaluated, ChessShape TheShape)
{
	if (TheShape.TotalNum == 5)
	{
		//01100 50
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == blank
			&& TheShape.List[4] == blank)
		{
			return 50;
		}
		//00110 50
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == blank
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == blank)
		{
			return 50;
		}
		//11010 200
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == blank
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == blank)
		{
			return 200;
		}
		//11100 500
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == blank
			&& TheShape.List[4] == blank)
		{
			return 500;
		}
		//00111 500
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == blank
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated)
		{
			return 500;
		}
		//01110 5000
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == blank)
		{
			return 5000;
		}
		//11101 5000
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == blank
			&& TheShape.List[4] == Evaluated)
		{
			return 5000;
		}
		//11011 5000
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == blank
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated)
		{
			return 5000;
		}
		//10111 5000
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == blank
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated)
		{
			return 5000;
		}
		//11110 5000
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == blank)
		{
			return 5000;
		}
		//01111 5000
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated)
		{
			return 5000;
		}
		//11111 10000000
		if (TheShape.List[0] == Evaluated
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated)
		{
			return 10000000;
		}
	}
	if (TheShape.TotalNum == 6)
	{
		//010110 5000
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == blank
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated
			&& TheShape.List[5] == blank)
		{
			return 5000;
		}
		//011010 5000
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == blank
			&& TheShape.List[4] == Evaluated
			&& TheShape.List[5] == blank)
		{
			return 5000;
		}
		//011110 50000
		if (TheShape.List[0] == blank
			&& TheShape.List[1] == Evaluated
			&& TheShape.List[2] == Evaluated
			&& TheShape.List[3] == Evaluated
			&& TheShape.List[4] == Evaluated
			&& TheShape.List[5] == blank)
		{
			return 50000;
		}
	}
	return 0;
}

