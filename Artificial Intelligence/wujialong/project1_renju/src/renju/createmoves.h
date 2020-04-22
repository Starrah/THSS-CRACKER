#ifndef _CREATEMOVES_
#define _CREATEMOVES_
//走法生成器

#include "define.h"
#include "hash.h"
#include <vector>
#include <unordered_map>
#include <vector>

#define MOVE_NUM (8U)											//搜索树的最大分支数
#define DEFEND_NUM (3U)											//保证用于防守策略的分支数

//已经执行过createMoves的局面，记录结果，不再重复
extern std::unordered_map<hash_t, std::vector<point>> createdMoves;

//生成最佳的扩展集（柱搜索）
std::vector<point> createMoves(int player);

#endif
