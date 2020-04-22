#pragma once

#include "define.h"

//快速判断是否游戏结束（仅考虑 pos 这一步的影响），返回赢方或平局
//假设 pos位置确为 player，不作检查
int gameover(point pos, int player); 

//完整计算游戏结束时的状态，返回赢方或者平局
int gameover();