#include "define.h"
//只用检测一半的出发点
int directions[4][2] = {
	1, 0,
	0, 1,
	1, 1,
	-1, 1
};

bool gameover(pair<int, int> & node, int player, int dir) {//判断是否游戏结束{
	int x = node.first;
	int y = node.second;
	for (int j = 0; j < 5; j++)
	{
		int nx = x + directions[dir][0] * j;
		int ny = y + directions[dir][1] * j;
		if (!inboard(nx, ny) || chessBoard[nx][ny] != player)
		{
			return false;
		}
	}
	return true;
}

//判断从node出发,是否能连出5子，具体判断当局游戏是否结束时，只用检测最后的落子，复杂度为O1，详见GameLoop
bool gameover(pair<int, int>& node, int player)
{
	int x = node.first;
	int y = node.second;

	for (int i = 0; i < 4; i++)
	{
		if (gameover(node, player, i))
			return true;
	}

	return false;
}
