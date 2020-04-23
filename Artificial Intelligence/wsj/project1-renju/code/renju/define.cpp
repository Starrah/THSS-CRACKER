#include "define.h"

int chessBoard[GRID_NUM][GRID_NUM];//定义棋盘
int valueBoard[GRID_NUM][GRID_NUM];//棋盘估值
vector<pair<int, int>> ai_steps; //记录ai
vector<pair<int, int>> user_steps; //记录用户
map<string, int> score_map;  //记录不同棋局的得分
int turns = 0;

//存储距离1之内的邻点偏移量
int neighbor[8][2] = {
	-1, -1,
	-1, 0,
	-1, 1,
	0, -1,
	0, 1,
	1, -1,
	1, 0,
	1, 1
};

//存储距离2之内的邻点偏移量
int lNeighbor[24][2] = {
	-2, -2,
	-2, -1,
	-2, 0,
	-2, 1,
	-2, 2,
	-1, -2,
	-1, -1,
	-1, 0,
	-1, 1,
	-1, 2,
	0, -2,
	0, -1,
	0, 1,
	0, 2,
	1, -2,
	1, -1,
	1, 0,
	1, 1,
	1, 2,
	2, -2,
	2, -1,
	2, 0,
	2, 1, 
	2, 2
};

void printValueBoard()
{
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			cout << valueBoard[i][j] << " ";
		}
		cout << endl;
	}
}

//是否在距离1以内有相邻的非空点
bool hasNeighbor(pair<int, int>& point) {
	for (int i = 0; i < 8; i++) {
		int dx = neighbor[i][0];
		int dy = neighbor[i][1];
		if (inboard(point.first + dx, point.second + dy)
			&& chessBoard[point.first + dx][point.second + dy] != blank)
			return true;
	}

	return false;
}

//是否在距离2以内有相邻的非空点
bool hasLNeighbor(pair<int, int>& point) {
	for (int i = 0; i < 24; i++) {
		int dx = lNeighbor[i][0];
		int dy = lNeighbor[i][1];
		if (inboard(point.first + dx, point.second + dy)
			&& chessBoard[point.first + dx][point.second + dy] != blank)
			return true;
	}

	return false;
}