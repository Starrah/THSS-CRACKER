#include "GameLoop.h"
GameLoop::GameLoop(bool ai_is_black)
{
	score_map["01100"] = 50;
	score_map["00110"] = 50;
	score_map["11010"] = 200;
	score_map["00111"] = 500;
	score_map["11100"] = 500;
	score_map["01110"] = 5000;
	score_map["010110"] = 5000;
	score_map["011010"] = 5000;
	score_map["11101"] = 5000;
	score_map["11011"] = 5000;
	score_map["10111"] = 5000;
	score_map["11110"] = 5000;
	score_map["01111"] = 5000;
	score_map["011110"] = 50000;
	score_map["11111"] = 99999999;
	if (ai_is_black)  //ai黑棋先手
	{
		user_color = white;
		ai_color = black;
		cur_turn = AI;
	}
	else  //用户黑棋先手
	{
		user_color = black;
		ai_color = white;
		cur_turn = USER;
	}

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 g2(seed1);
	uint64_t u64Random = g2();

	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = 0; j < GRID_NUM; j++) {
			zobrist[i][j] = g2();
		}
	}

	user_steps.clear();
	ai_steps.clear();

	clearBoard();

	//create_initialize();
}

void GameLoop::printMenu()
{
	cout << "*************五子棋人机对弈AI*************" << endl;
	if (user_color == black)
		cout << "*************用户执黑先手*********************" << endl;
	else
		cout << "*************电脑执黑先手*********************" << endl;
	cout << "输入: end		 结束游戏" << endl;
	cout << "输入: move x y  表示落子点" << endl;
	cout << "输入: regret    表示悔棋" << endl;
	cout << "ai棋数:   " << ai_steps.size() << endl << "user棋数: " << user_steps.size() << endl;
	//cout << "回合: " << turns << endl;
}

void GameLoop::printLog()
{
	cout << log;
}

void GameLoop::userLoop()
{
	//cin.clear();
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "user input: ";
	string cmd;
	int x = -1, y = -1;
	cin >> cmd;
	if (cmd.empty())
		throw - 1;
	if (cmd.compare("move") == 0)
	{
		cin >> x >> y;
		if (inboard(y, x) && chessBoard[y][x] == 0)
			makeMove(y, x, USER, user_color);
		else
			throw - 1;
	}
	else if (cmd.compare("regret") == 0)
	{
		if (ai_steps.empty() || user_steps.empty())
		{
			throw - 1;
		}
		else
		{
			unMakeMove();
			throw 1;
		}
	}
	else if (cmd.compare("end") == 0) {
		throw string("return");
	}
	else
	{
		throw - 1;
	}

}

void GameLoop::aiLoop()
{
	//minMaxHeauSearch(ai_color, DEPTH, -inf, +inf, *this);
	//if (user_steps.size()) {
	//	auto & last_step = user_steps
	//}
	do {
		deepSearch(ai_color, DEPTH, -inf, +inf, *this, 2);
	} while (!inboard(next_x, next_y) || chessBoard[next_x][next_y] != blank);
	makeMove(next_x, next_y, AI, ai_color);
	//random search bug: will endless loop if table  is full
	//int rx, ry;
	//do
	//{
	//	rx = rand() % GRID_NUM;
	//	ry = rand() % GRID_NUM;
	//} while (chessBoard[rx][ry] != blank);
	//chessBoard[rx][ry] = ai_color;
}

void GameLoop::run()
{
	while (true)
	{
		system("cls");
		printMenu();
		print();//输出棋盘
		//cout << "score: " << evaluate(ai_color, user_color) << endl;

		//printValueBoard(); //输出估值


		if (isWin())
		{
			cout << "you win" << endl;
			system("pause");
			return;
		}

		if (isLose())
		{
			cout << "you lose" << endl;
			system("pause");
			return;
		}

		//平局
		if (ai_steps.size() + user_steps.size() == GRID_NUM * GRID_NUM)
		{
			cout << "draw" << endl;
			system("pause");
			return;
		}

		switch (cur_turn)
		{
		case USER:
			try
			{
				userLoop();
			}
			catch (int i)
			{
				if (i == -1)
					cout << "invalid input!" << endl;
				else
					cout << "regret!" << endl;
				cur_turn = USER;
				break;
			}
			catch (string s) {
				return;
			}
			cur_turn = AI;
			break;
		case AI:
			aiLoop();
			cur_turn = USER;
			break;
		default:
			cout << "error!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			assert(1);
			break;
		}
	}

}

bool GameLoop::isWin()
{
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (gameover(make_pair(i, j), user_color))
				return true;
		}
	}
	return false;
}

bool GameLoop::isLose()
{
	for (int i = 0; i < GRID_NUM; i++)
	{
		for (int j = 0; j < GRID_NUM; j++)
		{
			if (gameover(make_pair(i, j), ai_color))
				return true;
		}
	}
	return false;
}

bool GameLoop::isGameOver(int last_x, int last_y, int last_color)
{
	for (int offset = -5; offset <= 0; offset++)
	{
		if (gameover(make_pair(last_x + offset, last_y), last_color, 0) ||
			gameover(make_pair(last_x, last_y + offset), last_color, 1) ||
			gameover(make_pair(last_x + offset, last_y + offset), last_color, 2) ||
			gameover(make_pair(last_x - offset, last_y + offset), last_color, 3))
			return true;
	}
	return false;
}

GameLoop::~GameLoop()
{
}
