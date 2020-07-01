#include "Matrix.h"

mat::mat(int n, bool clear) :n(n) {
	row = n;
	if (~row & 1) row++; // ±ÜÃâ´óµÄ2µÄÃÝ
	a = new int[n*row];
	if (clear) memset(a, 0, sizeof(int)*n*row);
}

mat::mat(const mat & A) {
	n = A.n; row = A.row;
	a = new int[n*row];
	memcpy(a, A.a, sizeof(int)*n*row);
}

mat::~mat() {
	if (a) delete[] a;
}

void mat::left_top(mat & res) const {
	int m = (n + 1) >> 1;
	for (int i = 0; i < m; i++) memcpy(res[i], a + i * row, sizeof(int)*m);
}

void mat::right_top(mat & res) const {
	int m = (n + 1) >> 1;
	for (int i = 0; i < m; i++) memcpy(res[i], a + i * row + m, sizeof(int)*(n - m));
}

void mat::left_bottom(mat & res) const {
	int m = (n + 1) >> 1;
	for (int i = m; i < n; i++) memcpy(res[i - m], a + i * row, sizeof(int)*m);
}

void mat::right_bottom(mat & res) const {
	int m = (n + 1) >> 1;
	for (int i = m; i < n; i++) memcpy(res[i - m], a + i * row + m, sizeof(int)*(n - m));
}

mat operator + (const mat &A, const mat& B) {
	//assert(A.size() == B.size());
	int n = A.size(); mat res(n);
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i][j] = A[i][j] + B[i][j];
	return res;
}

mat operator - (const mat &A, const mat& B) {
	//assert(A.size() == B.size());
	int n = A.size(); mat res(n);
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i][j] = A[i][j] - B[i][j];
	return res;
}

bool operator == (const mat &A, const mat& B) {
	if (A.size() != B.size()) return false;
	int n = A.size();
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (A[i][j] != B[i][j]) return false;
	return true;
}

std::ostream & operator << (std::ostream &os, const mat &A) {
	int n = A.size();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			os << A[i][j] << ' ';
		os << std::endl;
	}
	return os;
}