#include "gameover.h"

static constexpr int Dx[] = { 1,0,1,1 };						//连子方向
static constexpr int Dy[] = { 0,1,1,-1 };

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
