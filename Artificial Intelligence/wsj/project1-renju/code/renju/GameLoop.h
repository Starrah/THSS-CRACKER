#pragma once
#include "define.h"
#include "searchmove.h"
#include "printchessboard.h"
#include "makemove.h"
#include "evaluate.h"
class GameLoop
{
	int user_color;//用户执棋颜色
	int ai_color;//ai执棋颜色
	int cur_turn;//当前轮为用户还是ai
	string log = "";
	void printMenu();//打印菜单
	void printLog();//打印附加信息

	void userLoop();//每次循环时用户的操作,主要是处理输入
	void aiLoop();//每次循环时ai的操作


public:
	GameLoop(bool ai_is_black);
	inline void clearBoard() {
		for (int i = 0; i < GRID_NUM; i++)
			for (int j = 0; j < GRID_NUM; j++)
				chessBoard[i][j] = blank;
	}//清空棋盘

	bool isWin(); //是否胜利,On2
	bool isLose(); //是否失败 On2
	bool isGameOver(int last_x, int last_y, int last_color); //只根据上一个点来判断是否胜利,O1
	void run();
	~GameLoop();

	map<pair<long long, long long>, int> zobrist_map;//zobrist散列表
	long long cur_zobrist[2] = { 0,0 };//白棋，黑棋的zobrist值
	long long zobrist[GRID_NUM][GRID_NUM];//zobrist地图

	void cal_zobrist(){
		cur_zobrist[0] = cur_zobrist[1] = 0;
		for (int i = 0; i < GRID_NUM; i++)
			for (int j = 0; j < GRID_NUM; j++) {
				if (chessBoard[i][j] == white) {
					cur_zobrist[0] ^= zobrist[i][j];
				}
				if (chessBoard[i][j] == black) {
					cur_zobrist[1] ^= zobrist[i][j];
				}
			}
	}

	void update_zobrist(int x, int y, int color) {
		if (color == white)
			cur_zobrist[0] ^= zobrist[x][y];
		if(color == black)
			cur_zobrist[1] ^= zobrist[x][y];
	}
};

