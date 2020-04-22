#include "hash.h"

hash_t zobristValue[GRID_NUM][GRID_NUM][3];						//每个位置的每种着子的zobrist值
hash_t whiteFirstValue, MinFirstValue;							//当前白棋先行和玩家先行的zobrist值
hash_t zobrist;													//当前局面zobrist值，包括谁先手，先手是min还是max

std::unordered_map<hash_t, hashNode> hashMap;					//哈希表

//随机数生成器
hash_t rand64() {
	return rand() ^ ((hash_t)rand() << 15) ^ ((hash_t)rand() << 30) ^ ((hash_t)rand() << 45) ^ ((hash_t)rand() << 60);
}

//zobrist值的初始化
void initHashValue() {
	for (int i = 1; i < GRID_NUM; i++)
		for (int j = 1; j < GRID_NUM; j++) {
			zobristValue[i][j][1] = rand64();
			zobristValue[i][j][2] = rand64();
		}
	whiteFirstValue = rand64();
	MinFirstValue = rand64();
	zobrist = 0;
	hashMap.clear();
}

//清理长期未被访问的哈希节点
void hashMapClean()
{
	for (auto iter = hashMap.begin(); iter != hashMap.end();) {
		if (timeStamp - iter->second.time > HASH_CLEAN_TIME)
			iter = hashMap.erase(iter);
		else
			iter++;
	}
}
