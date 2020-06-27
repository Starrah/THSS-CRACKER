#ifndef _GAMEOVER_
#define _GAMEOVER_
#include "evaluate.h"

/*
描述：判断当前位置棋型是否胜利
参数：被判断的棋类型, 棋型
返回：是否
*/
extern bool JudgeOneShapeSuccess(int Evaluated, ChessShape TheShape);

/*
描述：判断游戏结束
参数：无
返回：0：没赢 -1：和棋 1：黑胜利 2：白胜利
*/
extern int JudgeWin();

#endif