#include "define.h"
#include "evaluate.h"
#include <iostream>
using namespace std;

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
				printf("%-2d",j);
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