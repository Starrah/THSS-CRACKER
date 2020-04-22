
#ifndef _DEFINE_
#define _DEFINE_
//全局变量定义

#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

//棋盘坐标的点对
struct point {
	int x, y;
	point(int x=0, int y=0):x(x),y(y) {} 
};
bool operator ==(const point &A, const point &B);

#define blank (0)												//空白点
#define black (1)												//黑方
#define white (2)												//白方
#define draw  (3)												//平局
#define opposite(x) (3-(x))										//对手：black->white, white->black
#define inf (1<<30)												//常数正无穷
#define winValue (100000000)									//必胜局面的赋值
#define inboard(a,b) ((a)>0 && (a)<=15 && (b)>0 && (b)<=15)		//用于检验招法是否在棋盘上
#define GRID_NUM 16												//棋盘规模	
	
extern int chessBoard[GRID_NUM][GRID_NUM];						//棋盘
extern int winner;												//记录游戏结束时的赢家或平局状态
extern int remainBlank;											//棋盘中的剩余可用空位
extern std::vector<point> moveRecord;							//下棋记录
extern int agent;												//AI扮演的角色(黑棋/白棋)
extern int user;												//玩家扮演的角色(与AI相反)
extern int timeStamp;											//时间戳（当前棋局步数）

#endif


