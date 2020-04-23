#ifndef _EVALUATE_
#define _EVALUATE_
//估值生成器
#include "define.h"

//给定起点，偏移和方向计算目标点
int getPointStatus(pair<int, int> node, int direction, int offset);

//给定某点和某方向，计算以offset为起点的五子连线
void getPoints(int x, int y, int offset, int direction, pair<int, int>* nodes);

//给定某点某方向计算对应形状分数
int calScore(pair<int, int> node, int direction, int my_player, int ai_color, int user_color, vector<map<string, int>>* score_all_arr);

//估值函数，给定当前map给定当前局势分数
int Evaluate(int ai_color, int user_color, int gamma = 1);  //gamma代表进攻与防守的权重 gamm大偏向防守
#endif