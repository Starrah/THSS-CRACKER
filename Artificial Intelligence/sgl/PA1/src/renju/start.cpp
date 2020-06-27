//Ö÷º¯Êý


#include "define.h"
#include "searchmove.h"
#include "printchessboard.h"
#include "makemove.h"
#include "gameover.h"

int main()
{

	InitializeGame();
	print();
	
	while (1)
	{
		if (CurrentSide == Computer)
		{
			SearchMove();
			makeMove();
			if (GameOver())
			{
				break;
			}
			SwitchSide();
			print();
		}
		else if (CurrentSide == Player)
		{
			int order = PlayerMove();
			if (order == -1)
			{
				break;
			}
			if (order == 1)
			{
				makeMove();
				if (GameOver())
				{
					break;
				}
				SwitchSide();
				print();
			}
			if (order == 2)
			{
				if (unMakeMove())
				{
					print();
				}
			}
		}
	}
	print();
	Replay();
	PrintLog();
	while (1) {

	}
	return 0;
}
