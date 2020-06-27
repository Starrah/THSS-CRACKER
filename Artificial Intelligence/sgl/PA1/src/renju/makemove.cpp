#include "define.h"
#include "evaluate.h"
#include "gameover.h"
#include "printchessboard.h"

/*
描述：初始化游戏
参数：无
返回：无
*/
void InitializeGame()
{
	while (1)
	{
		std::cout << "新游戏" << endl;
		std::cout << "输入 newblack 选择黑子先手" << endl;
		std::cout << "输入 newwhite 选择白子后手" << endl;

		std::string TheString;
		std::cin >> TheString;
		if (TheString == "newblack")
		{
			Player = black;
			Computer = white;
			break;
		}
		else if (TheString == "newwhite")
		{
			Player = white;
			Computer = black;
			break;
		}
		cout << "非法命令，请重新输入！" << endl;
	}
	//黑子先手
	CurrentSide = black;

	//如果AI先手，初始化到棋盘中央
	if (Player == white)
	{
		int newx = GRID_NUM / 2;
		int newy = GRID_NUM / 2;
		Point newpoint(newx, newy);
		ComputerList.push_back(newpoint);
		chessBoard[newx][newy] = Computer;
		SwitchSide();
	}
}


/*
描述：处理玩家下棋
参数：无
返回：正常下棋返回1（并且更新下棋位置），正常悔棋返回2，异常返回0，直接退出-1
*/
int PlayerMove()
{
	std::cout << "输入 move x y 落子在(x,y)" << endl;
	std::cout << "输入 unmove 悔棋" << endl;
	std::cout << "输入 quit 退出游戏" << endl;


	std::string TheString;
	int x = -1;
	int y = -1;
	std::cin >> TheString;

	if (TheString == "move")
	{
		std::cin >> x >> y;
		if (x <= 0 || x >= GRID_NUM || y <= 0 || y >= GRID_NUM)
		{
			cout << "非法位置，越界！" << endl;
			return 0;
		}
		if (chessBoard[x][y] != blank)
		{
			cout << "非法位置，此处非空！" << endl;
			return 0;
		}
		NewPlayerX = x;
		NewPlayerY = y;
		return 1;
	}
	else if (TheString == "unmove")
	{
		return 2;
	}
	else if (TheString == "quit")
	{
		return -1;
	}
	else
	{
		cout << "Invalid order!" << endl;
		return 0;
	}
}

/*
描述：执行下棋操作--更新棋盘和下棋栈
参数：无
返回：无
*/
void makeMove()
{
	if (CurrentSide == Player)
	{
		chessBoard[NewPlayerX][NewPlayerY] = CurrentSide;
		Point NewMove(NewPlayerX, NewPlayerY);
		PlayerList.push_back(NewMove);
	}
	else if (CurrentSide == Computer)
	{
		chessBoard[NewComputerX][NewComputerY] = CurrentSide;
		Point NewMove(NewComputerX, NewComputerY);
		ComputerList.push_back(NewMove);
	}
}


/*
描述：执行悔棋操作--更新棋盘和下棋栈
参数：无
返回：正常悔棋返回true，否则返回false
*/
bool unMakeMove()
{
	int size1 = PlayerList.size();
	int size2 = ComputerList.size();
	if (size1 == 0)
	{
		cout << "无法再悔棋！" << endl;
		return false;
	}
	Point PreviousMove1 = PlayerList[size1 - 1];
	chessBoard[PreviousMove1.x][PreviousMove1.y] = blank;
	PlayerList.pop_back();
	if (size2 > 0)
	{
		Point PreviousMove2 = ComputerList[size2 - 1];
		chessBoard[PreviousMove2.x][PreviousMove2.y] = blank;
		ComputerList.pop_back();
	}
	return true;
}

/*
描述：判断游戏是否结束，结束了输出对应信息，否则继续
参数：无
返回：结束返回true，否则返回false
*/
bool GameOver()
{
	int result = JudgeWin();
	if (result == Player)
	{
		cout << "玩家胜利！" << endl;
		GameResult = Player;
	}
	else if (result == Computer)
	{
		cout << "电脑胜利！" << endl;
		GameResult = Computer;
	}
	else if (result == -1)
	{
		cout << "和棋！" << endl;
		GameResult = blank;
	}
	else
	{
		return false;
	}
	return true;
}

/*
描述：复盘
参数：无
返回：无
*/
void Replay()
{
	while (1)
	{
		std::cout << "是否要复盘？(yes/no)" << endl;
		std::string TheString;
		int x = -1;
		int y = -1;
		std::cin >> TheString;

		if (TheString == "yes")
		{
			break;
		}
		else if (TheString == "no")
		{
			return;
		}
		else
		{
			std::cout << "非法输入，请重试！" << endl;
		}
	}

	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			chessBoard[i][j] = blank;
		}
	}
	print();

	int PlayerFlag = 0;
	int ComputerFlag = 0;
	int CurrentPrint = black;
	for (int i = 0; i < PlayerList.size() + ComputerList.size(); i++)
	{
		if (CurrentPrint == Player)
		{
			int x = PlayerList[PlayerFlag].x;
			int y = PlayerList[PlayerFlag].y;
			chessBoard[x][y] = Player;
			print();
			cout << "第 " << i + 1 << "步，玩家落子于(" << x << "," << y << ")位置" << endl;
			PlayerFlag++;
			CurrentPrint = Opposite(CurrentPrint);
		}
		else if (CurrentPrint == Computer)
		{
			int x = ComputerList[ComputerFlag].x;
			int y = ComputerList[ComputerFlag].y;
			chessBoard[x][y] = Computer;
			print();
			cout << "第 " << i + 1 << "步，电脑落子于(" << x << "," << y << ")位置" << endl;
			ComputerFlag++;			
			CurrentPrint = Opposite(CurrentPrint);
		}
	}
	if (GameResult == Player)
	{
		cout << "最终玩家获胜" << endl;
	}
	else if (GameResult == Computer)
	{
		cout << "最终电脑获胜" << endl;
	}
	else if (GameResult == blank)
	{
		cout << "最终和棋" << endl;
	}
}