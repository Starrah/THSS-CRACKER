
#ifndef _DEFINE_
#define _DEFINE_
//全局变量定义

#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iomanip>
using namespace std;

struct Point
{
	int x;
	int y;
	Point() {}
	Point(int a, int b)
	{
		x = a;
		y = b;
	}
};

//全局常量
#define blank 0		//空白点
#define black 1		//黑方
#define white 2		//白方
#define outbound -1 //越界
#define inf 100000000		
#define GRID_NUM 16	//棋盘规模	
#define TOTAL_PLACE (GRID_NUM - 1) * (GRID_NUM - 1)
#define MAX_DEPTH 3 //搜索深度



//和游戏有关的全局变量
extern int chessBoard[GRID_NUM][GRID_NUM]; //棋盘
extern int Player; //玩家
extern int Computer; //AI
extern int CurrentSide;//当前
extern int NewComputerX; //AI当前最优下棋X
extern int NewComputerY; //AI当前最优下棋Y
extern int NewPlayerX; //玩家当前落子位置X
extern int NewPlayerY; //玩家当前落子位置Y
extern int GameResult; //这一局游戏结果
//玩家落子情况
extern std::vector<Point> PlayerList;
//敌人落子情况
extern std::vector<Point> ComputerList;

//常用的底层函数封装
//对方棋子
extern int Opposite(int a); 
//交换当前棋子
extern void SwitchSide();
//x的4个方向
extern int DirectionX[4];
//y的4个方向
extern int DirectionY[4];
//获取棋盘某位置值，1黑2白0空-1越界
extern int GetPlace(int x, int y);
//判断棋盘某位置是否有邻居
extern bool JudgeNeighbor(int x, int y);

//实验用各种全局变量
//这一局游戏总共电脑下棋几次
extern int TotalSearchMove;
//搜索次数
extern int SearchNum;
//剪枝次数
extern int CutNum;
//这一局游戏总共搜索次数
extern int TotalSearchNum;
//这一局游戏总共剪枝次数
extern int TotalCutNum;
//这一局游戏总共消耗时间
extern int TotalCostTime;
#endif
