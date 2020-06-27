#include "define.h"

//和游戏有关的全局变量
int chessBoard[GRID_NUM][GRID_NUM];//定义棋盘
int Player = -1; //玩家
int Computer = -1; //AI
int CurrentSide = -1;//当前
int NewComputerX = -1; //AI当前最优下棋X
int NewComputerY = -1; //AI当前最优下棋Y
int NewPlayerX = -1; //玩家当前落子位置X
int NewPlayerY = -1; //玩家当前落子位置Y
int GameResult = -1; //这一局游戏结果
//玩家落子情况
std::vector<Point> PlayerList;
//敌人落子情况
std::vector<Point> ComputerList;


//常用的底层函数封装

//对方棋子
int Opposite(int a)
{
	return black + white - a;
}

//交换当前棋子
extern void SwitchSide()
{
	CurrentSide = black + white - CurrentSide;
}

//x的4个方向
int DirectionX[4] = { 0, 1, 1, -1 };

//y的4个方向
int DirectionY[4] = { 1, 0, 1, 1 };

//获取棋盘某位置值，1黑2白0空-1越界
int GetPlace(int x, int y)
{
	if (x <= 0 || y <= 0 || x >= GRID_NUM || y >= GRID_NUM)
	{
		return outbound;
	}
	return chessBoard[x][y];
}

//判断棋盘某位置是否有邻居
extern bool JudgeNeighbor(int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		int newx = x + DirectionX[i];
		int newy = y + DirectionY[i];
		if (GetPlace(newx, newy) == black || GetPlace(newx, newy) == white)
		{
			return true;
		}
		newx = x - DirectionX[i];
		newy = y - DirectionY[i];
		if (GetPlace(newx, newy) == black || GetPlace(newx, newy) == white)
		{
			return true;
		}
	}
	return false;
}



//实验用各种全局变量
//这一局游戏总共电脑下棋几次
int TotalSearchMove = 0;
//搜索次数
int SearchNum = 0;
//剪枝次数
int CutNum = 0;
//这一局游戏总共搜索次数
int TotalSearchNum = 0;
//这一局游戏总共剪枝次数
int TotalCutNum = 0;
//这一局游戏总共消耗时间
int TotalCostTime = 0;