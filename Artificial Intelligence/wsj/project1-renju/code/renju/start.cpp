#include "define.h"
#include "searchmove.h"
#include "printchessboard.h"
#include "makemove.h"
#include "GameLoop.h"
#include <conio.h>
#include "test.h"

void printMenu()
{
	cout << "*************五子棋人机对弈AI*************" << endl;
	cout << "输入: newai     电脑执黑先手" << endl;
	cout << "输入: newuser   电脑执白后手" << endl;
	cout << "输入: end       结束游戏" << endl;
	cout << "输入: move x y  表示落子点" << endl;
	cout << "输入: regret    表示悔棋" << endl;
}



int main()
{
#ifdef _TEST_
	test();
	return 0;
#endif // _TEST_

	srand(time(NULL));

	while (true)
	{
		system("cls");
		printMenu();
		cout << "指定先后手: ";
		string s;
		cin >> s;
		if (s.compare("newai") == 0)
		{
			GameLoop game = GameLoop(true);
			game.run();
		}
		else if (s.compare("newuser") == 0)
		{
			GameLoop game = GameLoop(false);
			game.run();
		}
		else if (s.compare("end") == 0)
		{
			return 0;
		}
		else
		{
			cout << "您输入的参数无效,请重新输入,按任意键确认";
			system("pause");
		}
	}

	return 0;

}
