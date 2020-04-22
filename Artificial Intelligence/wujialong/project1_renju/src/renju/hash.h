#pragma once

#include "define.h"
#include <unordered_map>

typedef unsigned long long hash_t;

#define hashUnknowValue (inf+1)									//用于表示当前局面是否在哈希表中

extern hash_t zobristValue[GRID_NUM][GRID_NUM][3];				//每个位置的每种着子的zobrist值
extern hash_t whiteFirstValue, MinFirstValue;					//当前白棋先行和玩家先行的zobrist值
extern hash_t zobrist;											//当前局面zobrist值，包括谁先手，先手是min还是max

//随机数生成器
hash_t rand64();

//zobrist值的初始化
void initHashValue();

//hash节点中的估值标签的类型
enum hashFlag { Exact, Lower, Upper };

//哈希节点
struct hashNode {
	hash_t zobrist;
	int depth;
	hashFlag flag;
	int value;
	point move;
	int time;
	hashNode() { }
	hashNode(hash_t zobrist, int depth, hashFlag flag, int value, point move, int time) :
		zobrist(zobrist), depth(depth), flag(flag), value(value), move(move), time(time) { }
};
extern std::unordered_map<hash_t, hashNode> hashMap;			//哈希表

#define HASH_CLEAN_TIME 2										//哈希节点过期的时间
//清理长期未被访问的哈希节点
void hashMapClean();