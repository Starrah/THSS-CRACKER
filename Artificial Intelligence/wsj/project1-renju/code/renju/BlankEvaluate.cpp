#include "BlankEvaluate.h"

int BlankEvaluate::valueBoard[GRID_NUM][GRID_NUM];
int BlankEvaluate::getPointStatus(pair<int, int> node, int i, int j)
{ // node：当前点  i：方向  j：坐标相对值 
	int x = node.first;
	int y = node.second;
	switch (i)
	{
	case 0:
		break;
	case 1:
		x = x + j;
		break;
	case 2:
		x = x + j;
		y = y + j;
		break;
	case 3:
		y = y + j;
		break;
	case 4:
		x = x - j;
		y = y + j;
		break;
	case 5:
		x = x - j;
		break;
	case 6:
		x = x - j;
		y = y - j;
		break;
	case 7:
		y = y - j;
		break;
	case 8:
		x = x + j;
		y = y - j;
	}
	if (!inboard(x, y))
	{ // 越界处理
		return out;
	}
	return chessBoard[x][y];
}

int BlankEvaluate::evaluatePoint(pair<int, int> node, int cur_color, int oppo_color, bool defend = false)
{ //  target_player:当前计算的player的代号
	//defend 为1时,说明是从对方角度看的,我需要进行防御
	if (getPointStatus(node, 0, 0) != blank) //判断此位置是否已经落子
	{
		return -1000;
	}
	int value = 0;
	int numoftwo = 0;
	int target_color = cur_color;
	int other_color = oppo_color;

	for (int i = 1; i <= 8; i++)  // 8个方向
	{ 
		// 活四 01111* *代表当前空位置  0 代表其他空位置    下同 
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) == target_color && getPointStatus(node, i, -4) == target_color
			&& getPointStatus(node, i, -5) == blank)
		{
			value += 300000;
			if (defend) { value -= 500; }
			continue;
		}
		// 死四A 21111*
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) == target_color && getPointStatus(node, i, -4) == target_color
			&& (getPointStatus(node, i, -5) == other_color || getPointStatus(node, i, -5) == out))
		{
			value += 250000;
			if (defend) { value -= 500; }
			continue;
		}
		// 死四B 111*1
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) == target_color && getPointStatus(node, i, 1) == target_color)
		{
			value += 240000;
			if (defend) { value -= 500; }
			continue;
		}
		// 死四C 11*11
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, 1) == target_color && getPointStatus(node, i, 2) == target_color)
		{
			value += 230000;
			if (defend) { value -= 500; }
			continue;
		}
		// 活三 近3位置 111*0
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) == target_color)
		{
			if (getPointStatus(node, i, 1) == blank)
			{
				value += 750;
				if (getPointStatus(node, i, -4) == blank)
				{
					value += 3150;
					if (defend) { value -= 300; }
				}
			}
			else
			{
				if (getPointStatus(node, i, -4) == blank)
				{
					value += 500;
				}
			}
			continue;
		}
		// 活三 远3位置 1110*
		if (getPointStatus(node, i, -1) == blank && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) == target_color && getPointStatus(node, i, -4) == target_color)
		{
			value += 350;
			continue;
		}
		// 死三 11*1
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, 1) == target_color)
		{
			value += 600;
			if (getPointStatus(node, i, -3) == blank && getPointStatus(node, i, 2) == blank)
			{
				value += 3150;
				continue;
			}
			if ((getPointStatus(node, i, -3) == other_color || getPointStatus(node, i, -3) == out) && (getPointStatus(node, i, 2) == other_color || getPointStatus(node, i, 2) == out))
			{
				continue;
			}
			else
			{
				value += 700;
				continue;
			}
		}
		//活二的个数   
		if (getPointStatus(node, i, -1) == target_color && getPointStatus(node, i, -2) == target_color
			&& getPointStatus(node, i, -3) != other_color && getPointStatus(node, i, 1) != other_color)
		{
			numoftwo++;
		}
		//其余散棋
		int numOfplyer = 0; // 因为方向会算两次？
		for (int k = -4; k <= 0; k++)
		{ // ++++* +++*+ ++*++ +*+++ *++++
			int temp = 0;
			for (int l = 0; l <= 4; l++)
			{
				if (getPointStatus(node, i, k + l) == target_color)
				{
					temp++;
				}
				else
					if (getPointStatus(node, i, k + l) == other_color
						|| getPointStatus(node, i, k + l) == out)
					{
						temp = 0;
						break;
					}
			}
			numOfplyer += temp;
		}
		value += numOfplyer * 15;
		if (numOfplyer != 0)
		{
		}
	}
	if (numoftwo >= 2)
	{
		value += 3000;
		if (defend)
		{
			value -= 100;
		}
	}
	return value;
}

void BlankEvaluate::evaluate_all(int ai_color, int user_color)
{
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			valueBoard[i][j] = evaluatePoint(make_pair(i, j), AI, ai_color, user_color) + evaluatePoint(make_pair(i, j), USER, ai_color, user_color);
		}
	}
}


BlankEvaluate::BlankEvaluate()
{
}


BlankEvaluate::~BlankEvaluate()
{
}
