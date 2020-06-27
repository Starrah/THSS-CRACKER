#ifndef _MAKEMOVE_
#define _MAKEMOVE_
//走法执行器

#include "define.h"

/*
描述：初始化游戏
参数：无
返回：无
*/
void InitializeGame();

/*
描述：处理玩家下棋
参数：无
返回：正常下棋返回1（并且更新下棋位置），正常悔棋返回2，异常返回0，直接退出-1
*/
extern int PlayerMove();

/*
描述：执行下棋操作--更新棋盘和下棋栈
参数：无
返回：无
*/
extern void makeMove();

/*
描述：执行悔棋操作--更新棋盘和下棋栈
参数：无
返回：正常悔棋返回true，否则返回false
*/
extern bool unMakeMove();

/*
描述：判断游戏是否结束，结束了输出对应信息，否则继续
参数：无
返回：结束返回true，否则返回false
*/
bool GameOver();

/*
描述：复盘
参数：无
返回：无
*/
void Replay();

#endif