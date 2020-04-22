#ifndef _SEARCHMOVE_
#define _SEARCHMOVE_

//搜索核心

#include "evaluate.h"
#include "gameover.h"
#include "define.h"
#include "hash.h"
#include <algorithm>

extern int searchDepth;											// 记录当前搜索的最大深度
#define TIME_ALLOWED (5.0)										//迭代加深搜索的最长时间限制，单位：秒

extern clock_t start_clock;										//开始搜索的时间

//搜索的入口函数
std::pair<point, int> searchMove(); 

//带alpha-beta剪枝的Minimax搜索算法
//current : 当前 player
std::pair<point, int> MiniMax(int current, int depth, int alpha, int beta);

//迭代加深搜索
std::pair<point, int> idSearch();

//快速防守
std::pair<point, int> fastDefend();

//查找当前局面
std::pair<point, int> findHashMap(int current, int depth, int alpha, int beta);

//记录当前局面
void recordHashMap(int depth, hashFlag flag, int value, point move);

#endif
