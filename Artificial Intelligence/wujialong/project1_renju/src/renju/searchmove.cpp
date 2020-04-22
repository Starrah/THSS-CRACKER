#include "define.h"
#include "createmoves.h"
#include "makemove.h"
#include "evaluate.h"
#include "printchessboard.h"
#include "searchmove.h"
#include <algorithm>
#include <cassert>
#include <cmath>
using namespace std;

clock_t start_clock;											//开始搜索的时间
int searchDepth;											// 记录当前搜索的最大深度

//搜索的入口函数
std::pair<point, int> searchMove()
{
//	对于必应棋型进行快速防守
//	auto res = fastDefend();
//	if (!(res.first == point())) return res;

	//迭代加深搜索
	return idSearch();
}


//带alpha-beta剪枝的Minimax搜索算法
//current : 当前 player
std::pair<point, int> MiniMax(int current, int depth, int alpha, int beta) {

	//查找当前局面是否在哈希表中，以加速搜索
	std::pair<point, int> hashResult = findHashMap(current, depth, alpha, beta);
	if (hashResult.second != hashUnknowValue) {
		return hashResult;
	}

	//搜索深度达到上限时，评估局面并返回
	if (depth == 0) {
		int v = Evaluate(current);
		recordHashMap(depth, Exact, v, point());
		return make_pair(point(), v);
	}

	//上一次搜索时保存在哈希表中的行动优先搜索
	point priorMove = hashResult.first;
	// 生成待搜索的扩展集
	vector<point> moveList = createMoves(current);
	int exploreLen = moveList.size();
	int tmpWinner;

	if (current == agent) { // 极大搜索
		point optMove;
		int v = -inf, newv;
		for (int i = -1; exploreLen && i < (int)moveList.size(); i++) { // i==-1 优先搜索 priorMove
			point currentMove = i == -1 ? priorMove : moveList[i];
			if (currentMove == point() || (i >= 0 && currentMove == priorMove))
				continue;
			exploreLen--;

			if (makeMove(currentMove, current)) {
				if (tmpWinner = gameover(currentMove, current))
					newv = tmpWinner == current ? winValue : 0;
				else
					newv = MiniMax(opposite(current), depth - 1, alpha, beta).second;
				if (newv > v)
				{
					v = newv, optMove = currentMove;
				}
				if (v >= beta || clock() - start_clock > (clock_t)(TIME_ALLOWED * CLOCKS_PER_SEC)) {
					unMakeMove(current);
					recordHashMap(depth, Upper, v, optMove);
					return make_pair(optMove, v);
				}
				alpha = max(alpha, v);
				unMakeMove(current);
			}
		}

		recordHashMap(depth, Exact, v, optMove);
		return make_pair(optMove, v);
	}
	else { // 极小搜索
		point optMove;
		int v = inf, newv;
		for (int i = -1; exploreLen && i < (int)moveList.size(); i++) { // i==-1 优先搜索 priorMove
			point currentMove = i == -1 ? priorMove : moveList[i];
			if (currentMove == point() || (i >= 0 && currentMove == priorMove))
				continue;
			exploreLen--;

			if (makeMove(currentMove, current)) {
				if (tmpWinner = gameover(currentMove, current))
					// 考虑玩家是非理性人，算力有限
					newv = tmpWinner == current ? (-(int)(winValue * pow(0.95, (searchDepth - depth) >> 1))) : 0;
				else
					newv = MiniMax(opposite(current), depth - 1, alpha, beta).second;
				if (newv < v)
				{
					v = newv, optMove = currentMove;
				}
				if (v <= alpha || clock() - start_clock > (clock_t)(TIME_ALLOWED * CLOCKS_PER_SEC)) {
					unMakeMove(current);
					recordHashMap(depth, Lower, v, optMove);
					return make_pair(optMove, v);
				}
				beta = min(beta, v);
				unMakeMove(current);
			}
		}

		recordHashMap(depth, Exact, v, optMove);
		return make_pair(optMove, v);
	}
}


//迭代加深搜索
std::pair<point, int> idSearch()
{
	start_clock = clock();
	std::pair<point, int> res;
	long long startDepth = 1;

	//一个可能的优化：
	//从该局面以前达到过的最大深度开始搜索
	if (hashMap.find(zobrist) != hashMap.end()) {
		startDepth = (long long)hashMap[zobrist].depth + 1;
		hashMap[zobrist].time = timeStamp;
	}

	hashMapClean();
	//迭代加深搜索，直到耗尽时间
	for (long long i = startDepth; ; i++)
	{
		searchDepth = i;
		res = MiniMax(agent, i, -inf, inf);
		if (res.second >= winValue) return res;
		if (clock() - start_clock >= (clock_t)(TIME_ALLOWED * CLOCKS_PER_SEC)) break;
	}
	return res;
}

//快速防守
std::pair<point, int> fastDefend()
{
	if (evaluate(agent, agent) >= 500000) return make_pair(point(), 0);
	int eval = evaluate(user, user);

	//出现了必应棋型
	//则采取最小化对手评分策略
	if (eval >= 500000)
	{
		int minDamage = eval, curDamage = 0, agentEval = 0;
		point defendMove = point();
		for(int i = 1; i < GRID_NUM; i++)
			for (int j = 1; j < GRID_NUM; j++)
			{
				if (chessBoard[i][j] != blank) continue;
				makeMove(point(i, j), agent);
				curDamage = evaluate(user, user);
				if (curDamage < minDamage)
				{
					minDamage = curDamage;
					agentEval = evaluate(agent, user);
					defendMove = point(i, j);
				}
				else if (curDamage == minDamage)
				{
					auto curAgentEval = evaluate(agent, user);
					if ((defendMove == point()) ||
						(curAgentEval > agentEval))
					{
						minDamage = curDamage;
						agentEval = curAgentEval;
						defendMove = point(i, j);
					}
				}
				unMakeMove(agent);
	//			cout << "( " << i << ", " << j << " ) : " << curDamage << endl;
			}
		makeMove(defendMove, agent);
		int ret = Evaluate(agent);
		unMakeMove(agent);
		return make_pair(defendMove, ret);
	}

	//未出现必应棋型
	return make_pair(point(), 0);
}


//查找当前局面
std::pair<point, int> findHashMap(int current, int depth, int alpha, int beta) {
	if (hashMap.find(zobrist) == hashMap.end()) return std::make_pair(point(), hashUnknowValue);
	hashNode & node = hashMap[zobrist];
	node.time = timeStamp;
	if (node.depth >= depth) {
		if (node.flag == Exact)
			return make_pair(node.move, node.value);
		if (current == agent) { // MAX
			if (node.flag == Upper && node.value >= beta)
				return make_pair(node.move, node.value);
		}
		else { // MIN
			if (node.flag == Lower && node.value <= alpha)
				return make_pair(node.move, node.value);
		}
	}
	return std::make_pair(node.move, hashUnknowValue);
}

//记录当前局面
void recordHashMap(int depth, hashFlag flag, int value, point move) {
	if (hashMap.find(zobrist) == hashMap.end() || hashMap[zobrist].depth < depth ||
		(hashMap[zobrist].depth == depth && flag == Exact)) {
		hashMap[zobrist] = hashNode(zobrist, depth, flag, value, move, timeStamp);
	}
}