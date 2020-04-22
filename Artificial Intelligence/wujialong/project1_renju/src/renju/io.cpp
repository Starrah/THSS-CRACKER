#include "io.h"
#include "hash.h"
#include "printchessboard.h"
#include "evaluate.h"
#include "makemove.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//字符串处理
void Trim(string & str)
{
	string blanks("\f\v\r\t\n ");
	str.erase(0, str.find_first_not_of(blanks));
	str.erase(str.find_last_not_of(blanks) + 1);
}

//询问先手
int getTheIntitative() {
	system("cls");
	cout << "*************五子棋人机对弈AI*************" << endl;
	cout << endl; print(); cout << endl;
	cout << "输入: newblack  电脑先手" << endl;
	cout << "输入: newwhite  电脑后手" << endl << endl;

	while (true) {
		string input;
		getline(cin, input); Trim(input);
		if (input == "newblack") return black;
		else if (input == "newwhite") return white;
		else {
			cout << endl << "请重新输入！" << endl;
			cout << "输入: newblack  电脑先手" << endl;
			cout << "输入: newwhite  电脑后手" << endl << endl;
		}
	}
}

enum stateSet {NORMAL, INVALID, REGRET_SUCCESS, REGRET_FAILED}; //输入的合法性验证

const string stateIndicator[4] = {								//提示信息
	"请输入指令：",
	"指令有误，请重新输入：",
	"悔棋成功，请继续下棋：",
	"您不需要悔棋，请重新输入指令："
};

//等待用户输入
point getUserMove(int eval, point agentLastMove)
{
	static int state = NORMAL;
	point lastMove = agentLastMove;
	while (true) {
		system("cls");
		cout << "*************五子棋人机对弈AI*************" << endl;
		cout << endl; print(); cout << endl;
		if (!(lastMove == point()))
			cout << "电脑上步落子：(" << lastMove.x << "," << lastMove.y << ")" << endl;
		if (lastMove == agentLastMove)
			cout << (agent == black ? "黑" : "白") << "棋局面评估: " << ((eval > 0) ? "+" : "") << eval << endl << endl;
		else
			cout << "局面评估当前不可用，需等到电脑再次落子" << endl << endl;
		cout << "输入: move r c  表示落子点（r,c 分别表示行列）" << endl;
		cout << "输入: regret 来悔棋" << endl << endl;
		cout << stateIndicator[state];
		string input;
		getline(cin, input);
		istringstream sin(input);
		bool fail = false;
		string order; sin >> order;
		if (order == "regret")
		{
			regret(state, lastMove);
			continue;
		}
		if (order != "move") fail = true;
		if (!fail) {
			int x=-1, y=-1;
			sin >> x >> y;
			if (!inboard(x, y) || chessBoard[x][y]!=blank)
				fail = true;
			else
				return point(x, y);
		}
		state = INVALID;
	}
}

//输出赢家
void outputWinner()
{
	system("cls");
	cout << "*************五子棋人机对弈AI*************" << endl;
	cout << endl; print(); cout << endl;
	if (winner == draw) { cout << "平局！" << endl; return; }
	if (winner == agent) cout << "电脑"; else cout << "用户";
	if (winner == black) cout << "执黑"; else cout << "执白";
	cout << "获胜。" << endl;
}

//AI计算时的等待界面
void userWaiting()
{
	system("cls");
	cout << "*************五子棋人机对弈AI*************" << endl;
	cout << endl; print(); cout << endl;
	cout << "电脑思考中..." << endl;
}

//读入记录以进行复盘
bool readRecord()
{
	ifstream fin(".\\record.txt");
	fin >> agent;
	if (agent != black && agent != white) return false;
	
	cout << "检测到棋谱，是否读入？[Y/N]";
	string input;
	getline(cin, input); Trim(input);
	char ch = 'N';
	if (input.length() > 0) ch = input[0];
	if (!(ch == 'Y' || ch == 'y')) return false;

	int x, y, cur = black;
	if (agent == white) zobrist ^= MinFirstValue;
	while (!fin.eof() && fin.good())
	{
		fin >> x >> y;
		if (makeMove(point(x, y), cur))
		{
			cur = opposite(cur);
			timeStamp++;
		}
	}

	return true;
}

//保存记录
void writeRecord()
{
	ofstream fout(".\\record.txt");
	if (!fout.is_open()) { return; }
	fout << agent << endl;
	for (auto it : moveRecord)
	{
		fout << it.x << " " << it.y << endl;
	}
}

//悔棋
void regret(int& state, point& lastMove)
{
	if (moveRecord.size() >= 2U)
	{
		unMakeMove(agent);
		unMakeMove(user);
		lastMove = (moveRecord.empty() ? point() : moveRecord.back());
		state = REGRET_SUCCESS;
	}
	else
	{
		state = REGRET_FAILED;
	}
}
