#include "createmoves.h"
//此处的createmoves根据空白区域的价值来排序
//

bool compS(ScoreWithP& p1, ScoreWithP& p2)
{
	return p1.score > p2.score;
}
vector<pair<int, int>> createMoves(int player) //生成全部合法走法集
{
	//为了节约时间,仅产生周围已有落子的
	vector<pair<int, int>> choices;
	vector<ScoreWithP> choicesWithS;
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (chessBoard[i][j] == blank &&	//TODO:之后将考虑延伸成线段的棋子2格
				hasNeighbor(make_pair(i, j)))
			{
				int score = BlankEvaluate::evaluatePoint(make_pair(i, j), player, getOppo(player), false) + BlankEvaluate::evaluatePoint(make_pair(i, j), getOppo(player), player, true);
				ScoreWithP sp(score, i, j);
				choicesWithS.push_back(sp);
			}
		}
	}

	sort(choicesWithS.begin(), choicesWithS.end(), compS);

	for (auto s : choicesWithS)
	{
		choices.push_back(make_pair(s.x, s.y));
	}
	if (choices.size() == 0)
	{
		int x = GRID_NUM / 2 + rand() % 3 - 1;
		int y = GRID_NUM / 2 + rand() % 3 - 1;
		choices.push_back(make_pair(x, y));
	}
	return choices;
}
//查找所有合法落子点。对于五子棋来说，棋盘上的空白点均为可行走法。

vector<pair<int, int>> createSimpleMoves(int player) //生成全部合法走法集
{
	//为了节约时间,仅产生周围已有落子的
	vector<pair<int, int>> choices;
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (chessBoard[i][j] == blank &&	//TODO:之后将考虑延伸成线段的棋子2格
				hasNeighbor(make_pair(i, j)))
			{
				choices.push_back(make_pair(i, j));
			}
		}
	}
	return choices;
}