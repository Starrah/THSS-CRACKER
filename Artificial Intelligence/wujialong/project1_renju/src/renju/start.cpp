#include "define.h"
#include "searchmove.h"
#include "printchessboard.h"
#include "makemove.h"
#include "gameover.h"
#include "io.h"
#include "hash.h"

int main()
{
	//哈希初始化
	initHashValue();

	//读入记录
	if (!readRecord()) {
		agent = getTheIntitative(); // 询问先手
		if (agent == white) zobrist ^= MinFirstValue;
	}
	user = agent == black ? white : black;

	//终局判定
	if (winner = gameover())
	{
		outputWinner();
		writeRecord();
		system("pause");
		return 0;
	}
	point agentLastMove(0, 0);
	int eval = 0;

	//AI计算第一步棋
	if ((agent == black && moveRecord.size() % 2 == 0) || 
		(agent == white && moveRecord.size() % 2 == 1))
	{
		userWaiting();
		std::pair<point, int> searchResult = searchMove();
		point agentMove = searchResult.first;
		eval = searchResult.second;
		makeMove(agentMove, agent);
		agentLastMove = agentMove;
		timeStamp++;
		winner = gameover(agentMove, agent);
	}

	//AI和玩家轮流操作
	while (!winner) {
	    point userMove = getUserMove(eval, agentLastMove);
		makeMove(userMove, user);
		timeStamp++;
		if (winner = gameover(userMove, user))
			break;
		userWaiting();
		std::pair<point, int> searchResult = searchMove();
		point agentMove = searchResult.first;
		eval = searchResult.second;
		makeMove(agentMove, agent);
		agentLastMove = agentMove;
		timeStamp++;
		if (winner = gameover(agentMove, agent))
			break;
		writeRecord();
	}

	//判定终局时的游戏状态
	//并保存相关信息
	outputWinner();
	writeRecord();
	system("pause");
	return 0;
}
