#include "evaluate.h"
#include <algorithm>
#include <cmath>
using namespace std;

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

//全局估值算法
int Evaluate(int current) {
	int res = evaluate(agent, current) - evaluate(user, current);
	if (current == user) res -= 500;
	return res;
}

//对某一方进行局面整体估值
int evaluate(int player, int current)
{
	const int n = GRID_NUM-1;
	int eval = 0;
	int chengCnt[6][3]{}, chongCnt[6][3]{};
	
	// 水平
	for (int i = 1; i <= n; i++) {
		int lastL = -1, lastR = -1;
		int lastLB = -1, lastRB = -1;
		for (int j = 1; j <= n;j++) {
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

static constexpr int Dx[] = { 1,0,1,1 };						//搜索方向控制
static constexpr int Dy[] = { 0,1,1,-1 };

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