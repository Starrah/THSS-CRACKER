#pragma once
#include "define.h"
//”√”⁄heuristic create moves
//≤Œøºhttps://www.cnblogs.com/maxuewei2/p/4825520.html#daima
class BlankEvaluate
{
public:
	static int valueBoard[GRID_NUM][GRID_NUM]; //∆Â≈Ã
	static int evaluatePoint(pair<int, int> node, int ai_color, int user_color, bool defend);
	static void evaluate_all(int ai_color, int user_color);
	static int getPointStatus(pair<int, int> node, int i, int j);

	BlankEvaluate();
	~BlankEvaluate();
};
