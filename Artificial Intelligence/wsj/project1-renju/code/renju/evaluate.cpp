#include "evaluate.h"

//给定起点，偏移和方向计算目标点在棋盘上的状态
int getPointStatus(pair<int, int> node, int direction, int offset)
{ // node：当前点  i：方向  j：坐标相对值 
	int x = node.first;
	int y = node.second;
	switch (direction)
	{
	case 0:
		break;
	case 1:
		x = x + offset;
		break;
	case 2:
		x = x + offset;
		y = y + offset;
		break;
	case 3:
		y = y + offset;
		break;
	case 4:
		x = x - offset;
		y = y + offset;
	}
	if (!inboard(x, y))
	{ // 越界处理
		return out;
	}
	return chessBoard[x][y];
}

//给定某点和某方向，计算以offset为起点的五子连线
void getPoints(int x, int y, int offset, int direction, pair<int, int>* nodes)
{
	switch (direction)
	{
	case 0:
		break;
	case 1:
		for (int i = 0; i < 5; i++)
		{
			nodes[i] = make_pair(x + (offset + i), y);
		}
		break;
	case 2:
		for (int i = 0; i < 5; i++)
		{
			nodes[i] = make_pair(x + (offset + i), y + (offset + i));
		}
		break;
	case 3:
		for (int i = 0; i < 5; i++)
		{
			nodes[i] = make_pair(x, y + (offset + i));
		}
		break;
	case 4:
		for (int i = 0; i < 5; i++)
		{
			nodes[i] = make_pair(x - (offset + i), y + (offset + i));
		}
	}
}

//给定某点某方向计算对应形状分数
int calScore(pair<int, int> node, int direction, int my_player, int ai_color, int user_color, vector<map<string, int>>* score_all_arr)
{
	int my_color = 0;
	int enemy_color = 0;
	map<string, int> max_score_shape;
	max_score_shape["score"] = 0;
	max_score_shape["direction"] = direction;
	if (my_player == AI)
	{
		my_color = ai_color;
		enemy_color = user_color;
	}
	else if (my_player == USER)
	{
		my_color = user_color;
		enemy_color = ai_color;
	}

	for (auto item : *score_all_arr)
	{
		pair<int, int> arr[5];
		getPoints(item["x"], item["y"], item["offset"], item["direction"], arr);
		for (auto pt : arr)
		{
			if (pt == node && direction == item["direction"])
				return 0;
		}
	}

	for (int offset = -5; offset <= 0; offset++)
	{
		string pos[6];
		for (int i = 0; i <= 5; i++)
		{
			if (getPointStatus(node, direction, i + offset) == blank)
			{
				pos[i] = "0";
			}
			else if (getPointStatus(node, direction, i + offset) == my_color)
			{
				pos[i] = "1";
			}
			else
			{
				pos[i] = "2";
			}
		}
		string tmp_shap5 = pos[0] + pos[1] + pos[2] + pos[3] + pos[4];
		string tmp_shap6 = pos[0] + pos[1] + pos[2] + pos[3] + pos[4] + pos[5];
		for (map<string, int>::iterator it = score_map.begin(); it != score_map.end(); ++it)
		{
			if (tmp_shap5 == it->first || tmp_shap6 == it->first)
			{
				if (it->second > max_score_shape["score"])
				{
					max_score_shape["x"] = node.first;
					max_score_shape["y"] = node.second;
					max_score_shape["offset"] = offset;
					max_score_shape["score"] = it->second;
					max_score_shape["direction"] = direction;
				}
			}
		}
	}
	int add_score = 0;
	if (max_score_shape["score"] != 0)
	{
		for (auto item : *score_all_arr)
		{
			pair<int, int> arr1[5], arr2[5];
			getPoints(item["x"], item["y"], item["offset"], item["direction"], arr1);
			getPoints(max_score_shape["x"], max_score_shape["y"], max_score_shape["offset"], max_score_shape["direction"], arr2);
			for (auto pt1 : arr1)
			{
				for (auto pt2 : arr2)
				{
					if (pt1 == pt2 && item["score"] > 10 && max_score_shape["score"] > 10)
					{
						add_score += item["score"] + max_score_shape["score"];
					}
				}

			}
		}
		score_all_arr->push_back(max_score_shape);
	}
	return add_score + max_score_shape["score"];
}

//估值函数，给定当前map给定当前局势分数 
int Evaluate(int ai_color, int user_color, int gamma)
{
	int my_score = 0;
	int enemy_score = 0;
	vector<map<string, int>> score_all_arr_my;
	vector<map<string, int>> score_all_arr_enemy;
	for (int i = 1; i < GRID_NUM; i++)
	{
		for (int j = 1; j < GRID_NUM; j++)
		{
			pair<int, int> node = make_pair(i, j);
			if (getPointStatus(node, 0, 0) == ai_color) //判断此位置是否已经落子
			{
				my_score += calScore(make_pair(i, j), 1, AI, ai_color, user_color, &score_all_arr_my);
				my_score += calScore(make_pair(i, j), 2, AI, ai_color, user_color, &score_all_arr_my);
				my_score += calScore(make_pair(i, j), 3, AI, ai_color, user_color, &score_all_arr_my);
				my_score += calScore(make_pair(i, j), 4, AI, ai_color, user_color, &score_all_arr_my);
			}
			else if (getPointStatus(node, 0, 0) == user_color)
			{
				enemy_score += calScore(make_pair(i, j), 1, USER, ai_color, user_color, &score_all_arr_enemy);
				enemy_score += calScore(make_pair(i, j), 2, USER, ai_color, user_color, &score_all_arr_enemy);
				enemy_score += calScore(make_pair(i, j), 3, USER, ai_color, user_color, &score_all_arr_enemy);
				enemy_score += calScore(make_pair(i, j), 4, USER, ai_color, user_color, &score_all_arr_enemy);
			}
		}
	}
	//cout << "ai score: " << my_score << endl;
	//cout << "user score: " << enemy_score << endl;
	return my_score - 0.1 * gamma * enemy_score;
}