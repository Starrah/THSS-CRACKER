#pragma once

#include<algorithm>
#include<iostream>
#include<cstring>

class mat {
private:
	int n, *a;
	int row;	// 矩阵每行起始位置之间的步长
				// 应避免为大的2的幂，造成缓存冲突
public:
	mat(int n, bool clear = false);
	mat(const mat &A);
	~mat();
	void clear() { memset(a, 0, sizeof(int)*n*row); }
	int *operator [](int x) { return a + x * row; }
	const int *operator [](int x) const { return a + x * row; }
	int size() const { return n; }
	
	// 拷贝 1/4 个矩阵到 res
	void left_top(mat &res) const;
	void right_top(mat &res) const;
	void left_bottom(mat &res) const;
	void right_bottom(mat &res) const;
};

mat operator + (const mat &A, const mat& B);
mat operator - (const mat &A, const mat& B);
bool operator == (const mat &A, const mat& B);
std::ostream & operator << (std::ostream &os, const mat &A);