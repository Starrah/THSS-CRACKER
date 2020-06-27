#ifndef _CREATEMOVES_
#define _CREATEMOVES_

//获取能走的步数集合，并且进行排序剪枝

/*
描述：获取能移动的位置集合，没有优先搜索和排除不太可行位置的规则
参数：无
返回：元素集合
*/
vector<Point> CreateMovesNaive();

/*
描述：获取能移动的位置集合，没有优先搜索，但是排除不太可行位置
参数：无
返回：元素集合
*/
vector<Point> CreateMovesWithNeighbor();

/*
描述：判断当前位置是否在优先集合里搜索过了
参数：x,y，优先集合
返回：是否
*/
extern bool JudgeInPriorityList(int x, int y, std::vector<Point> PriorityList);

/*
描述：获取能移动的位置集合，优先搜索上次落子周围的位置，排除周围没有任何棋子的位置
参数：无
返回：元素集合
*/
extern vector<Point> CreateMoves();
#endif

