#ifndef _SEARCHMOVE_
#define _SEARCHMOVE_
//搜索核心
//六个搜索算法
#include "GameLoop.h"
#include "define.h"
#include "createmoves.h"
#include "makemove.h"
#include "evaluate.h"
#include "printchessboard.h"
class GameLoop;
//int searchMove();//搜索核心
//启发式alphabeta
int minMaxHeauSearch(int now_play, int depth, int alpha, int beta, GameLoop & gl);
//minimax
int minMaxSearch(int now_play, int depth, GameLoop& gl);
//alphabeta算法
int minMaxAlphaSearch(int now_play, int depth, int alpha, int beta, GameLoop& gl);
//zobrist散列+alphabeta
int minMaxZobristSearch(int now_play, int depth, int alpha, int beta, GameLoop& gl);
//zobrist散列+启发式alphabeta
int minMaxHeauZobristSearch(int now_play, int depth, int alpha, int beta, GameLoop& gl);
//迭代加深算法
int deepSearch(int now_play, int depth, int alpha, int beta, GameLoop & gl, double time_limit);
//每次搜索完后,next_x, next_y代表最优的下棋点.last_x,last_y
extern int next_x, next_y;
extern int DEPTH;//最大搜索深度
#endif
