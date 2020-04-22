#include "AlphaBeta.h"
#include "HashMap.h"
#include "IDSearch.h"
#include <cstring>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cassert>
#include <windows.h>
using namespace std;
#define cl(x) memset(x,0,sizeof(x));

int chessBoard[GRID_NUM][GRID_NUM];
int remainBlank;

//打印棋盘
void print();
int gameover();
void Print(int round, int blackWin, int whiteWin, string blackName, string whiteName, int current);



// 验证 Hash 的优化效果：等效分支因子更小
void evaluateHashEffiency();
// 验证 IDS 的优化效果：等效分支因子更小
void evaluateIDSearchEfficiency();

#define AgentBlack IDSearch
string blackName("IDSearch");
#define AgentWhite AlphaBeta
string whiteName("AlphaBeta");
const int Round = 10;
// AgentBlack 与 AgentWhite 进行 Round 局对局
// 请保证 AgentBlack 与 AgentWhite 不同
void PK();

int main() {
	//evaluateHashEffiency();
	//evaluateIDSearchEfficiency();
	PK();
	system("pause");
	return 0;
}

void PK() {
	int blackWin = 0, whiteWin = 0; int winner;
	vector<int> winnerList, timeList;
	for (int i = 1; i <= Round; i++) {
		cl(chessBoard); remainBlank = 225;
		AgentBlack::clearAll(); AgentBlack::initHashValue();
		AgentWhite::clearAll(); AgentWhite::initHashValue();
		AgentBlack::agent = AgentWhite::user = black;
		AgentWhite::agent = AgentBlack::user = white;
		AgentWhite::zobrist ^= AgentWhite::MinFirstValue;

		int timeStamp = 0;
		while (true) {
			Print(i, blackWin, whiteWin, blackName, whiteName, black);
			point blackPos = AgentBlack::searchMove().first;
			if (chessBoard[blackPos.x][blackPos.y] != blank) {
				cout << "黑方落子非法" << endl; return;
			}
			chessBoard[blackPos.x][blackPos.y] = black; remainBlank--; timeStamp++;
			AgentBlack::makeMove(blackPos, AgentBlack::agent);
			AgentWhite::makeMove(blackPos, AgentWhite::user);
			if (winner = gameover()) {
				if (winner == black) blackWin++;
				Print(i, blackWin, whiteWin, blackName, whiteName, blank);
				printf("黑胜\n");
				printf("请等待 10 秒\n");
				Sleep(10000);
				winnerList.push_back(winner);
				timeList.push_back(timeStamp);
				break;
			}

			Print(i, blackWin, whiteWin, blackName, whiteName, white);
			point whitePos = AgentWhite::searchMove().first;
			if (chessBoard[whitePos.x][whitePos.y] != blank) {
				cout << "白方落子非法" << endl; return;
			}
			chessBoard[whitePos.x][whitePos.y] = white; remainBlank--; timeStamp++;
			AgentWhite::makeMove(whitePos, AgentWhite::agent);
			AgentBlack::makeMove(whitePos, AgentBlack::user);
			if (winner = gameover()) {
				if (winner == white) whiteWin++;
				Print(i, blackWin, whiteWin, blackName, whiteName, blank);
				printf("白胜\n");
				printf("请等待 10 秒\n");
				Sleep(10000);
				winnerList.push_back(winner);
				timeList.push_back(timeStamp);
				break;
			}
		}
	}
	printf("对局完毕，黑方已胜:白方已胜:平局 = %d:%d:%d\n", blackWin, whiteWin, Round - blackWin - whiteWin);
	for (int i = 0; i < Round; i++) {
		printf("第 %d 局，winner = %s, 双方总步数 = %d\n", i + 1, winnerList[i] == black ? "black" : "white", timeList[i]);
	}
}

void evaluateHashEffiency() {
#define HereAgentBlack AlphaBeta
	string blackName("AlphaBeta");
#define HereAgentWhite HashMap
	string whiteName("HashMap");

	const int MAX_DEPTH = 9;
	int blackWin = 0, whiteWin = 0; int winner;
	cl(chessBoard); remainBlank = 225;
	HereAgentBlack::clearAll(); HereAgentBlack::initHashValue();
	HereAgentWhite::clearAll(); HereAgentWhite::initHashValue();
	HereAgentBlack::agent = HereAgentWhite::user = black;
	HereAgentWhite::agent = HereAgentBlack::user = white;
	HereAgentWhite::zobrist ^= HereAgentWhite::MinFirstValue;

	int timeStamp = 0;
	while (true) {
		Print(1, blackWin, whiteWin, blackName, whiteName, black);
		point blackPos;
		for (int i = 1; i <= MAX_DEPTH; i++)
			blackPos = HereAgentBlack::searchMove(i).first;
		chessBoard[blackPos.x][blackPos.y] = black; remainBlank--; timeStamp++;
		HereAgentBlack::makeMove(blackPos, HereAgentBlack::agent);
		HereAgentWhite::makeMove(blackPos, HereAgentWhite::user);
		if (winner = gameover()) {
			if (winner == black) blackWin++;
			Print(1, blackWin, whiteWin, blackName, whiteName, blank);
			printf("黑胜\n");
			break;
		}

		Print(1, blackWin, whiteWin, blackName, whiteName, white);
		point whitePos;
		for (int i = 1; i <= MAX_DEPTH; i++)
			whitePos = HereAgentWhite::searchMove(i).first;
		chessBoard[whitePos.x][whitePos.y] = white; remainBlank--; timeStamp++;
		HereAgentWhite::makeMove(whitePos, HereAgentWhite::agent);
		HereAgentBlack::makeMove(whitePos, HereAgentBlack::user);
		if (winner = gameover()) {
			if (winner == white) whiteWin++;
			Print(1, blackWin, whiteWin, blackName, whiteName, blank);
			printf("白胜\n");
			break;
		}
	}

	for (int i = 1; i <= MAX_DEPTH; i++) {
		printf("depth = %d，AlphaBeta 平均等效分支因子 = %.3lf，HashMap 平均等效分支因子 = %.3lf\n",
			i, AlphaBeta::branchFact[i], HashMap::branchFact[i]);
	}

#undef HereAgentBlack
#undef HereAgentWhite
}

void evaluateIDSearchEfficiency() {
#define HereAgentBlack HashMap
	string blackName("HashMap");
#define HereAgentWhite IDSearch
	string whiteName("IDSearch");

	int blackWin = 0, whiteWin = 0; int winner;
	cl(chessBoard); remainBlank = 225;
	HereAgentBlack::clearAll(); HereAgentBlack::initHashValue();
	HereAgentWhite::clearAll(); HereAgentWhite::initHashValue();
	HereAgentBlack::agent = HereAgentWhite::user = black;
	HereAgentWhite::agent = HereAgentBlack::user = white;
	HereAgentWhite::zobrist ^= HereAgentWhite::MinFirstValue;

	int timeStamp = 0;
	while (true) {
		Print(1, blackWin, whiteWin, blackName, whiteName, black);
		//point blackPos = HereAgentBlack::searchMove().first;
		point blackPos;
		for (int i = 1; i <= 9; i++)
			blackPos = HereAgentBlack::searchMove(i).first;
		chessBoard[blackPos.x][blackPos.y] = black; remainBlank--; timeStamp++;
		HereAgentBlack::makeMove(blackPos, HereAgentBlack::agent);
		HereAgentWhite::makeMove(blackPos, HereAgentWhite::user);
		if (winner = gameover()) {
			if (winner == black) blackWin++;
			Print(1, blackWin, whiteWin, blackName, whiteName, blank);
			printf("黑胜\n");
			break;
		}

		Print(1, blackWin, whiteWin, blackName, whiteName, white);
		point whitePos = HereAgentWhite::searchMove().first;
		/*point whitePos;
		for (int i = 1; i <= 9; i++)
			whitePos = HereAgentWhite::searchMove(i).first;*/
		chessBoard[whitePos.x][whitePos.y] = white; remainBlank--; timeStamp++;
		HereAgentWhite::makeMove(whitePos, HereAgentWhite::agent);
		HereAgentBlack::makeMove(whitePos, HereAgentBlack::user);
		if (winner = gameover()) {
			if (winner == white) whiteWin++;
			Print(1, blackWin, whiteWin, blackName, whiteName, blank);
			printf("白胜\n");
			break;
		}
	}

	for (int i = 1; i <= 15; i++) {
		printf("depth = %d，HashMap 平均等效分支因子 = %.3lf，IDSearch 平均等效分支因子 = %.3lf\n",
			i, HashMap::branchFact[i], IDSearch::branchFact[i]);
	}

#undef HereAgentBlack
#undef HereAgentWhite
}

void Print(int round, int blackWin, int whiteWin, string blackName, string whiteName, int current) {
	system("cls");
	cout << "黑方：" << blackName << "；白方：" << whiteName << endl;
	printf("第 %d/%d 局\n黑方已胜:白方已胜:平局 = %d:%d:%d\n", round, Round, blackWin, whiteWin, round - (!!current) - blackWin - whiteWin);
	print();
	if (current)
		cout << (current == black ? "黑棋" : "白棋") << "思考中..." << endl;
}

//生成棋盘形状
void judge(int i, int j)
{
	if (i == 1 && j == 1)
		printf("╔ ");
	else if (i == 1 && j == 15)
		printf("╗");
	else if (i == 15 && j == 1)
		printf("╚ ");
	else if (i == 15 && j == 15)
		printf("╝");
	else if (i == 1 && j != 15 && j != 1)
		printf("┯ ");
	else if (i == 15 && j != 15 && j != 1)
		printf("┷ ");
	else if (j == 1 && i != 15 && i != 1)
		printf("┠ ");
	else if (j == 15 && i != 15 && i != 1)
		printf("┨");
	else
		printf("┼ ");
}

//打印棋盘
void print()
{
	for (int i = 0; i < 16; i++)
	{
		if (i == 0)
		{
			printf("  ");
			for (int j = 1; j < 16; j++)
				printf("%-2d", j);
			printf("\n");
			continue;
		}
		for (int j = 0; j < 16; j++)
		{
			if (j == 0)
			{
				printf("%2d", i);
				continue;
			}

			if (chessBoard[i][j] == 1) //此处为黑子
				printf("○");
			else if (chessBoard[i][j] == 2) //此处为白子
				printf("●");
			else
			{
				judge(i, j);
			}
		}
		printf("\n");
	}
}

static constexpr int Dx[] = { 1,0,1,1 };						//连子方向
static constexpr int Dy[] = { 0,1,1,-1 };

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
