#pragma once

#include "define.h"
#include <algorithm>

//询问先手
int getTheIntitative();

//等待用户输入
point getUserMove(int eval, point agentLastMove);

//输出赢家
void outputWinner();

//AI计算时的等待界面
void userWaiting();

//读入记录以进行复盘
bool readRecord();

//保存记录
void writeRecord();

//悔棋
void regret(int& state, point& lastMove);