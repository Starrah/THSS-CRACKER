#ifndef _SEARCHMOVE_
#define _SEARCHMOVE_
//搜索核心

/*
描述：搜索AI落子位置
参数：无
返回：无
*/
extern void SearchMove();

/*
描述：minimax搜索，函数返回值取正值的时候是max，负值的时候是min
参数：当前side，深度，alpha，beta值
*/
extern int MiniMax(int Side, int Depth, int alpha, int beta);

/*
描述：打印整个一局的实验结果
参数：无
返回：无
*/
extern void PrintLog();



#endif
