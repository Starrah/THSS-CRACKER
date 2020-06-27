#include "define.h"
#include "makemove.h"
#include "gameover.h"
#include "createmoves.h"
#include "searchmove.h"
//#include "printchessboard.h"

/*
描述：打印整个一局的实验结果
参数：无
返回：无
*/
void PrintLog()
{
	cout << "总共搜索次数：" << TotalSearchNum << endl;
	cout << "总共剪枝次数：" << TotalCutNum << endl;
	cout << "总共搜索耗费时间：" << TotalCostTime << "ms" << endl;

	int AverageSearchNum = TotalSearchNum / TotalSearchMove;
	int AverageCutNum = TotalCutNum / TotalSearchMove;
	int AverageCostTime = TotalCostTime / TotalSearchMove;

	cout << "平均搜索次数：" << AverageSearchNum << endl;
	cout << "平均剪枝次数：" << AverageCutNum << endl;
	cout << "平均搜索耗费时间：" << AverageCostTime << "ms" << endl;
}


/*
描述：搜索AI落子位置
参数：无
返回：无
*/
void SearchMove()
{
	CutNum = 0;
	SearchNum = 0;
	int StartTime = clock();
	MiniMax(Computer, MAX_DEPTH, -inf, inf);
	int StopTime = clock();
	int CostTime = (StopTime - StartTime) * 1000 / CLOCKS_PER_SEC;
	cout << "本次共搜索次数：" << SearchNum << endl;
	cout << "本次共剪枝次数：" << CutNum << endl;
	cout << "本次搜索耗费" << CostTime << "ms" << endl;
	TotalSearchMove++;
	TotalCostTime += CostTime;
	TotalSearchNum += SearchNum;
	TotalCutNum += CutNum;
}

/*
描述：minimax搜索，函数返回值取正值的时候是max，负值的时候是min
参数：当前side，深度，alpha，beta值
*/
int MiniMax(int Side, int Depth, int alpha, int beta)
{
	if (Depth <= 0 || JudgeWin() != 0)
	{
		return Evaluate(Side);
	}

	//获取所有可行的落子位置
	//todo:替换三个落子位置函数，进行测试
	vector<Point> MoveList = CreateMoves();


	for (int i = 0; i < MoveList.size(); i++)
	{
		//makemove阶段
		int x = MoveList[i].x;
		int y = MoveList[i].y;
		SearchNum++;
		chessBoard[x][y] = Side;
		//TODO:在更新chessboard之后用散列判断是否搜索过这个局面，是的话就直接剪枝
		Point NewMove(x, y);
		if (Side == Player)
		{
			PlayerList.push_back(NewMove);
		}
		else
		{
			ComputerList.push_back(NewMove);
		}

		//递归
		int value = -MiniMax(Opposite(Side), Depth - 1, -beta, -alpha);

		//unmakemove阶段
		chessBoard[x][y] = blank;
		if (Side == Player)
		{
			PlayerList.pop_back();
		}
		else
		{
			ComputerList.pop_back();
		}

		if (value > alpha)
		{
			//最优
			if (Depth == MAX_DEPTH)
			{
				NewComputerX = x;
				NewComputerY = y;
			}

			//剪枝
			if (value >= beta)
			{
				CutNum++;
				return beta;
			}
			alpha = value;
		}
	}
	return alpha;
}


