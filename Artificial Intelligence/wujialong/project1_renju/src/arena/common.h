#pragma once

#define blank (0)												//空白点
#define black (1)												//黑方
#define white (2)												//白方
#define draw  (3)												//平局
#define opposite(x) (3-(x))										//对手：black->white, white->black
#define inf (1<<30)												//常数正无穷
#define winValue (100000000)									//必胜局面的赋值
#define inboard(a,b) ((a)>0 && (a)<=15 && (b)>0 && (b)<=15)		//用于检验招法是否在棋盘上
#define GRID_NUM 16												//棋盘规模	

//棋盘坐标的点对
struct point {
	int x, y;
	point(int x = 0, int y = 0) :x(x), y(y) {}
};
bool operator ==(const point& A, const point& B)
{
	return A.x == B.x && A.y == B.y;
}

double solveBranchFact(int d, int N) {
	double L = 1, R = 100;
	while (R - L > 1e-4) {
		double mid = (L + R) / 2;
		double t = (pow(mid, d + 1) - 1) / (mid - 1);
		if (t < N) L = mid; else R = mid;
	}
	return (L + R) / 2;
}