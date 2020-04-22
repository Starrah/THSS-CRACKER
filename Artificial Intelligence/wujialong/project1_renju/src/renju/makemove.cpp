#include "define.h"
#include "printchessboard.h"
#include "makemove.h"
#include "hash.h"

//执行走法
bool makeMove(int x, int y, int player)
{
	return makeMove(point(x, y), player);
}

//执行走法
bool makeMove(point pos, int player)
{
	if (chessBoard[pos.x][pos.y] != blank) return false;
	chessBoard[pos.x][pos.y] = player;
	zobrist ^= zobristValue[pos.x][pos.y][player];
	zobrist ^= whiteFirstValue;
	zobrist ^= MinFirstValue;
	moveRecord.push_back(pos);
	remainBlank--;
	return true;
}

//撤销走法
bool unMakeMove(int player)
{
	point pos = moveRecord.back();
	moveRecord.pop_back();
	zobrist ^= zobristValue[pos.x][pos.y][player];
	zobrist ^= whiteFirstValue;
	zobrist ^= MinFirstValue;
	chessBoard[pos.x][pos.y] = blank;
	remainBlank++;
	return true;
}