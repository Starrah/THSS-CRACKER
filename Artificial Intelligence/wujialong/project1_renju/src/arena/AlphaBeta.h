#pragma once

#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <unordered_map>
#include "common.h"

namespace AlphaBeta {
	using std::min;
	using std::max;
	using std::vector;
	using std::make_pair;
	using std::pair;

	// 啥用也没有，就是为了让 main 的代码格式能统一起来
	void initHashValue() {}
	int zobrist, MinFirstValue, whiteFirstValue; 

	/******************************** define.h **************************************/

	int chessBoard[GRID_NUM][GRID_NUM];						//棋盘
	int winner;												//记录游戏结束时的赢家或平局状态
	int remainBlank = 225;											//棋盘中的剩余可用空位
	std::vector<point> moveRecord;							//下棋记录
	int agent;												//AI扮演的角色(黑棋/白棋)
	int user;												//玩家扮演的角色(与AI相反)
	int timeStamp;											//时间戳（当前棋局步数）

	/******************************** evaluate.h ***********************************/


//连续单线棋型估值表
	const int cheng[6][3] = {
	{0,0,0},
	{0,0,0}, //成 1
	{30,50,100}, // 成 2：死，单活，双活
	{50,200,5000}, // 成 3
	{10,8000,500000}, // 成 4
	{10000000,10000000,10000000}
	};

	//间断单线棋型估值表
	const int chong[6][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,10,100}, // 冲 3
	{0,50,5000}, // 冲 4
	{20,200,8000} // 冲 5
	};

	//先手的连续单线棋型估值表
	const int xianShouCheng[6][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,500000}, // 先手成双活3
	{0,500000,500000}, // 先手成活4 必赢
	{10000000,10000000,10000000},
	};

	//先手的间断单线棋型估值表
	const int xianShouChong[6][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,2000,500000}, // 先手冲双活4 必赢
	{10000000,10000000,10000000}, // 先手冲 5 必赢
	};

	//落子点的自带权重分布表
	const int PosValue[GRID_NUM][GRID_NUM] =
	{
		{0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0},

		{0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0, 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0, 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
		{0, 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
		{0, 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
		{0, 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
		{0, 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
		{0, 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
		{0, 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
		{0, 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
		{0, 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
		{0, 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
		{0, 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
		{0, 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
		{0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	int evaluate(int player, int current);
	int evaluateStep(int player, int x, int y);
	int patternAnalysis(int chengCnt[6][3], int chongCnt[6][3]);

	//全局估值算法
	int Evaluate(int current) {
		int res = evaluate(agent, current) - evaluate(user, current);
		if (current == user) res -= 500;
		return res;
	}

	//对某一方进行局面整体估值
	int evaluate(int player, int current)
	{
		const int n = GRID_NUM - 1;
		int eval = 0;
		int chengCnt[6][3]{}, chongCnt[6][3]{};

		// 水平
		for (int i = 1; i <= n; i++) {
			int lastL = -1, lastR = -1;
			int lastLB = -1, lastRB = -1;
			for (int j = 1; j <= n; j++) {
				if (chessBoard[i][j] == player) {
					int l = j, r = j;
					while (r + 1 <= n && chessBoard[i][r + 1] == player) r++;
					// 成
					int leftUnblocked = !(l - 1 == 0 || chessBoard[i][l - 1] == opposite(player));
					int rightUnblocked = !(r + 1 == n + 1 || chessBoard[i][r + 1] == opposite(player));
					eval += cheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					eval += (player == current) * xianShouCheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					chengCnt[min((r - l + 1), 5)][leftUnblocked + rightUnblocked]++;
					// 冲
					if (lastR != -1 && lastR == l - 2 && chessBoard[i][l - 1] == blank) {
						eval += chong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						eval += (player == current) * xianShouChong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						chongCnt[min(r - lastL + 1, 5)][lastLB + rightUnblocked]++;
					}
					lastL = l; lastR = r; lastLB = leftUnblocked; lastRB = rightUnblocked;
					j = r;
				}
			}
		}

		// 竖直
		for (int j = 1; j <= n; j++) {
			int lastL = -1, lastR = -1;
			int lastLB = -1, lastRB = -1;
			for (int i = 1; i <= n; i++) {
				if (chessBoard[i][j] == player) {
					int l = i, r = i;
					while (r + 1 <= n && chessBoard[r + 1][j] == player) r++;
					// 成
					int leftUnblocked = !(l - 1 == 0 || chessBoard[l - 1][j] == opposite(player));
					int rightUnblocked = !(r + 1 == n + 1 || chessBoard[r + 1][j] == opposite(player));
					eval += cheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					eval += (player == current) * xianShouCheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					chengCnt[min((r - l + 1), 5)][leftUnblocked + rightUnblocked]++;
					// 冲
					if (lastR != -1 && lastR == l - 2 && chessBoard[l - 1][j] == blank) {
						eval += chong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						eval += (player == current) * xianShouChong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						chongCnt[min(r - lastL + 1, 5)][lastLB + rightUnblocked]++;
					}
					lastL = l; lastR = r; lastLB = leftUnblocked; lastRB = rightUnblocked;
					i = r;
				}
			}
		}

		// ↙
		for (int s = 1 + 1; s <= n + n; s++) {
			int lastL = -1, lastR = -1;
			int lastLB = -1, lastRB = -1;
			int minJ = max(s - n, 1), maxJ = min(s - 1, n);
			for (int j = minJ; j <= maxJ; j++) {
				if (chessBoard[s - j][j] == player) {
					int l = j, r = j;
					while (r + 1 <= maxJ && chessBoard[s - (r + 1)][r + 1] == player) r++;
					// 成
					int leftUnblocked = !(l - 1 == minJ - 1 || chessBoard[s - (l - 1)][l - 1] == opposite(player));
					int rightUnblocked = !(r + 1 == maxJ + 1 || chessBoard[s - (r + 1)][r + 1] == opposite(player));
					eval += cheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					eval += (player == current) * xianShouCheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					chengCnt[min((r - l + 1), 5)][leftUnblocked + rightUnblocked]++;
					// 冲
					if (lastR != -1 && lastR == l - 2 && chessBoard[s - (l - 1)][l - 1] == blank) {
						eval += chong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						eval += (player == current) * xianShouChong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						chongCnt[min(r - lastL + 1, 5)][lastLB + rightUnblocked]++;
					}
					lastL = l; lastR = r; lastLB = leftUnblocked; lastRB = rightUnblocked;
					j = r;
				}
			}
		}

		// ↘
		for (int s = 1 - n; s <= n - 1; s++) {
			int lastL = -1, lastR = -1;
			int lastLB = -1, lastRB = -1;
			int minJ = max(1 - s, 1), maxJ = min(n - s, n);
			for (int j = minJ; j <= maxJ; j++) {
				if (chessBoard[s + j][j] == player) {
					int l = j, r = j;
					while (r + 1 <= maxJ && chessBoard[s + (r + 1)][r + 1] == player) r++;
					// 成
					int leftUnblocked = !(l - 1 == minJ - 1 || chessBoard[s + (l - 1)][l - 1] == opposite(player));
					int rightUnblocked = !(r + 1 == maxJ + 1 || chessBoard[s + (r + 1)][r + 1] == opposite(player));
					eval += cheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					eval += (player == current) * xianShouCheng[min(r - l + 1, 5)][leftUnblocked + rightUnblocked];
					chengCnt[min((r - l + 1), 5)][leftUnblocked + rightUnblocked]++;
					// 冲
					if (lastR != -1 && lastR == l - 2 && chessBoard[s + (l - 1)][l - 1] == blank) {
						eval += chong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						eval += (player == current) * xianShouChong[min(r - lastL + 1, 5)][lastLB + rightUnblocked];
						chongCnt[min(r - lastL + 1, 5)][lastLB + rightUnblocked]++;
					}
					lastL = l; lastR = r; lastLB = leftUnblocked; lastRB = rightUnblocked;
					j = r;
				}
			}
		}
		return patternAnalysis(chengCnt, chongCnt) + eval;
	}

	constexpr int Dx[] = { 1,0,1,1 };						//搜索方向控制
	constexpr int Dy[] = { 0,1,1,-1 };

	//对某一方进行单步落子的快速估值
	int evaluateStep(int player, int x, int y)
	{
		int chengCnt[6][3]{}, chongCnt[6][3]{};
		int eval = 0;
		for (int k = 0; k < 4; k++) {
			int dx = Dx[k], dy = Dy[k];
			int lx = x, ly = y, rx = x, ry = y;
			while (inboard(lx - dx, ly - dy) && chessBoard[lx - dx][ly - dy] == player)
				lx -= dx, ly -= dy;
			while (inboard(rx + dx, ry + dy) && chessBoard[rx + dx][ry + dy] == player)
				rx += dx, ry += dy;
			int leftUnblocked = !(!inboard(lx - dx, ly - dy) || chessBoard[lx - dx][ly - dy] == opposite(player));
			int rightUnblocked = !(!inboard(rx + dx, ry + dy) || chessBoard[rx + dx][ry + dy] == opposite(player));
			// 成
			eval += cheng[min(max(abs(rx - lx + 1), abs(ry - ly + 1)), 5)][leftUnblocked + rightUnblocked];
			chengCnt[min(max(abs(rx - lx + 1), abs(ry - ly + 1)), 5)][leftUnblocked + rightUnblocked]++;

			// 左冲
			if (!leftUnblocked && inboard(lx - 2 * dx, ly - 2 * dy) && chessBoard[lx - 2 * dx][ly - 2 * dy] == player) {
				int llx = lx - 2 * dx, lly = ly - 2 * dy;
				while (inboard(llx - dx, lly - dy) && chessBoard[llx - dx][lly - dy] == player)
					llx -= dx, lly -= dy;
				int leftLeftUnblocked = !(!inboard(llx - dx, lly - dy) || chessBoard[llx - dx][lly - dy] == opposite(player));
				eval += chong[min(max(abs(rx - llx + 1), abs(ry - lly + 1)), 5)][leftLeftUnblocked + rightUnblocked];
				chongCnt[min(max(abs(rx - llx + 1), abs(ry - lly + 1)), 5)][leftLeftUnblocked + rightUnblocked]++;
			}

			// 右冲
			if (!rightUnblocked && inboard(rx + 2 * dx, ry + 2 * dy) && chessBoard[rx + 2 * dx][ry + 2 * dy] == player) {
				int rrx = rx + 2 * dx, rry = ry + 2 * dy;
				while (inboard(rrx + dx, rry + dy) && chessBoard[rrx + dx][rry + dy] == player)
					rrx += dx, rry += dy;
				int rightRightUnblocked = !(!inboard(rrx + dx, rry + dy) || chessBoard[rrx + dx][rry + dy] == opposite(player));
				eval += chong[min(max(abs(rrx - lx + 1), abs(rry - ly + 1)), 5)][leftUnblocked + rightRightUnblocked];
				chongCnt[min(max(abs(rrx - lx + 1), abs(rry - ly + 1)), 5)][leftUnblocked + rightRightUnblocked];
			}
		}
		return patternAnalysis(chengCnt, chongCnt) + eval + PosValue[x][y];
	}

	//组合棋型估值
	int patternAnalysis(int chengCnt[6][3], int chongCnt[6][3])
	{
		//必胜棋型
		int ret = 0, winMove = 0, goodTry = 0;
		winMove += chengCnt[4][2] + chengCnt[5][0] + chengCnt[5][1] + chengCnt[5][2];
		winMove += (chengCnt[3][2] > 1); //双活三型
		winMove += ((chongCnt[5][0] + chongCnt[5][1] + chongCnt[5][2]) > 1); //双冲五型
		winMove += (chengCnt[3][2] && chengCnt[4][1]); //活三+半活四型
		winMove += (chengCnt[4][1] && chongCnt[5][2]); //半活四+冲5型，且这两个棋形不在一起，不会被同时堵死，则必胜
		ret += (!!winMove) * (winValue / 2);

		//准必赢、潜力棋型
		goodTry += 50 * (chengCnt[3][2] && chongCnt[5][2]); //活三+冲5型,有一定概率被堵死（.OOO.O.）,但可以尝试
		goodTry += 30 * (chengCnt[3][2] && (chongCnt[5][1] || chongCnt[5][0])); //活三+半冲5型
		goodTry += 25 * (chengCnt[4][1] && (chongCnt[5][1] || chongCnt[5][0])); //半活四+冲5型，但可能被堵死

		//对方必应
		goodTry += 100 * (chengCnt[3][2] + chongCnt[4][2]);
		goodTry += 100 * (chongCnt[5][0] + chongCnt[5][1]);
		ret += goodTry * 100;
		return ret;
	}

	/******************************* createmoves.h *********************************/

	const auto MOVE_NUM = 8U;											//搜索树的最大分支数
	const auto DEFEND_NUM = 3U;											//保证用于防守策略的分支数

	//生成最佳的扩展集（柱搜索）
	//基本思想：
	//对每个合法落子点进行估价，
	//选取估价最高的落子点进行搜索。
	std::vector<point> createMoves(int player)
	{
		int n = GRID_NUM - 1;
		int eval[GRID_NUM][GRID_NUM]{}, defend_eval[GRID_NUM][GRID_NUM]{};
		vector<point> moves, defends;

		//防守落子点
		//如果不在这些位置下棋，
		//就会很快陷入不利局面。
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				if (chessBoard[i][j] == blank) {
					defends.push_back(point(i, j));
					defend_eval[i][j] = evaluateStep(opposite(player), i, j);
				}
		sort(defends.begin(), defends.end(), [&](const point & A, const point & B) {
			return defend_eval[A.x][A.y] > defend_eval[B.x][B.y];
			});
		while (defends.size() > DEFEND_NUM) defends.pop_back();

		//进攻落子点
		//搜索有利于自己胜利的落子点
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				if (chessBoard[i][j] == blank &&
					find(defends.begin(), defends.end(), point(i, j)) == defends.end()) {
					moves.push_back(point(i, j));
					eval[i][j] = evaluateStep(player, i, j);
				}
		sort(moves.begin(), moves.end(), [&](const point & A, const point & B) {
			return eval[A.x][A.y] > eval[B.x][B.y];
			});
		while (moves.size() > (MOVE_NUM - DEFEND_NUM)) moves.pop_back();

		//合并进攻和防守
		for (auto it : defends) {
			moves.push_back(it);
			eval[it.x][it.y] = defend_eval[it.x][it.y];
		}
		sort(moves.begin(), moves.end(), [&](const point & A, const point & B) {
			return eval[A.x][A.y] > eval[B.x][B.y];
			});
		return moves;
	}

	/******************************** gameover.h ***********************************/

	//快速判断是否游戏结束（仅考虑 pos 这一步的影响），返回赢方或平局
	//假设 pos位置确为 player，不作检查
	int gameover(point pos, int player)
	{
		//计算 player 方是否由于pos这一子胜利
		for (int k = 0; k < 4; k++) {
			int dx = Dx[k], dy = Dy[k];
			for (int start = -4; start <= 0; start++) {
				bool same = 1;
				for (int v = start; v < start + 5; v++)
					same &= (inboard(pos.x + dx * v, pos.y + dy * v) && chessBoard[pos.x + dx * v][pos.y + dy * v] == chessBoard[pos.x][pos.y]);
				if (same)
					return player;
			}
		}
		// 计算是否平局
		if (!remainBlank) { return draw; }
		return blank;
	}

	//完整计算游戏结束时的状态
	int gameover()
	{
		if (!remainBlank) return draw; //平局
		for (int i = 1; i < GRID_NUM; i++) {
			for (int j = 1; j < GRID_NUM; j++) {
				if (chessBoard[i][j] == blank)
					continue;
				for (int k = 0; k < 4; k++) {
					bool same = 1; int dx = Dx[k], dy = Dy[k];
					for (int v = -2; v <= 2; v++)
						same &= (inboard(i + dx * v, j + dy * v) && chessBoard[i + dx * v][j + dy * v] == chessBoard[i][j]);
					if (same)
						return chessBoard[i][j]; //某一方胜利的计算
				}
			}
		}
		return blank;
	}

	/******************************** makemove.h ***********************************/

	bool makeMove(point pos, int player);

	//执行走法
	bool makeMove(int x, int y, int player)
	{
		return makeMove(point(x, y), player);
	}

	//执行走法
	bool makeMove(point pos, int player)
	{
		if (chessBoard[pos.x][pos.y] != blank) return false;
		chessBoard[pos.x][pos.y] = player;
		moveRecord.push_back(pos);
		remainBlank--;
		return true;
	}

	//撤销走法
	bool unMakeMove(int player)
	{
		point pos = moveRecord.back();
		moveRecord.pop_back();
		chessBoard[pos.x][pos.y] = blank;
		remainBlank++;
		return true;
	}

	/******************************* searchmove.h **********************************/

	const auto SEARCH_DEPTH = (9);										//参考的最大搜索深度

	std::pair<point, int> MiniMax(int current, int depth, int alpha, int beta);

	int searchCount = 0;

	double branchFact[20]; // （平均）有效分支因子b：1+b+b^2+...b^d = N（搜索节点数）
	int branchFactCount[20];

	//搜索的入口函数
	std::pair<point, int> searchMove(int depth = SEARCH_DEPTH)
	{
		printf("搜索深度：%d\n", depth);
		searchCount = 0;
		auto result = MiniMax(agent, depth, -inf, inf);
		double b = solveBranchFact(depth, searchCount);
		branchFact[depth] = (branchFact[depth] * branchFactCount[depth] + b) / (branchFactCount[depth] + 1);
		branchFactCount[depth]++;
		return result;
	}

	//带alpha-beta剪枝的Minimax搜索算法
	//current : 当前 player
	std::pair<point, int> MiniMax(int current, int depth, int alpha, int beta) {
		searchCount++;

		//搜索深度达到上限时，评估局面并返回
		if (depth == 0) {
			int v = Evaluate(current);
			return make_pair(point(), v);
		}

		// 生成待搜索的扩展集
		vector<point> moveList = createMoves(current);
		int exploreLen = moveList.size();
		int tmpWinner;

		if (current == agent) { // 极大搜索
			point optMove;
			int v = -inf, newv;
			for (int i = 0; exploreLen && i < (int)moveList.size(); i++) { // i==-1 优先搜索 priorMove
				point currentMove = moveList[i];
				if (currentMove == point())
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
					if (v >= beta) {
						unMakeMove(current);
						return make_pair(optMove, v);
					}
					alpha = max(alpha, v);
					unMakeMove(current);
				}
			}

			return make_pair(optMove, v);
		}
		else { // 极小搜索
			point optMove;
			int v = inf, newv;
			for (int i = 0; exploreLen && i < (int)moveList.size(); i++) { // i==-1 优先搜索 priorMove
				point currentMove = moveList[i];
				if (currentMove == point())
					continue;
				exploreLen--;

				if (makeMove(currentMove, current)) {
					if (tmpWinner = gameover(currentMove, current))
						// 考虑玩家是非理性人，算力有限
						newv = tmpWinner == current ? (-(int)(winValue * pow(0.95, (SEARCH_DEPTH - depth) >> 1))) : 0;
					else
						newv = MiniMax(opposite(current), depth - 1, alpha, beta).second;
					if (newv < v)
					{
						v = newv, optMove = currentMove;
					}
					if (v <= alpha) {
						unMakeMove(current);
						return make_pair(optMove, v);
					}
					beta = min(beta, v);
					unMakeMove(current);
				}
			}

			return make_pair(optMove, v);
		}
	}

	/********************************* clearall ************************************/

	void clearAll() {
#define cl(x) memset(x,0,sizeof(x))

		cl(chessBoard);
		winner = 0;											//记录游戏结束时的赢家或平局状态
		remainBlank = 225;											//棋盘中的剩余可用空位
		moveRecord.clear();							//下棋记录
		agent = 0;												//AI扮演的角色(黑棋/白棋)
		user = 0;												//玩家扮演的角色(与AI相反)
		timeStamp = 0;											//时间戳（当前棋局步数）

		cl(branchFact);
		cl(branchFactCount);

#undef cl
	}
}