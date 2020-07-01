#include "MatrixMul.h"

void mut_brute(const mat & A, const mat & B, mat & res) {
	int n = A.size(); res.clear();
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				res[i][j] += A[i][k] * B[k][j];
}

void mut_strassen(const mat & A, const mat & B, mat & res) {
	if (A.size()==1){
	    res[0][0] = A[0][0]*B[0][0];
	    return;
	}
	int n = A.size(), m = (n + 1) >> 1;
	mat a(m), b(m, 1), c(m, 1), d(m, 1), e(m), f(m, 1), g(m, 1), h(m, 1);
	A.left_top(a); A.right_top(b); A.left_bottom(c); A.right_bottom(d);
	B.left_top(e); B.right_top(f); B.left_bottom(g), B.right_bottom(h);

	mat P1(m), P2(m), P3(m), P4(m), P5(m), P6(m), P7(m);

	mut_strassen(a, f - h, P1); mut_strassen(a + b, h, P2);
	mut_strassen(c + d, e, P3); mut_strassen(d, g - e, P4);
	mut_strassen(a + d, e + h, P5); mut_strassen(b - d, g + h, P6);
	mut_strassen(a - c, e + f, P7);

	for (int i = 0; i < m; i++) for (int j = 0; j < m; j++) res[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
	for (int i = 0; i < m; i++) for (int j = m; j < n; j++) res[i][j] = P1[i][j - m] + P2[i][j - m];
	for (int i = m; i < n; i++) for (int j = 0; j < m; j++) res[i][j] = P3[i - m][j] + P4[i - m][j];
	for (int i = m; i < n; i++) for (int j = m; j < n; j++) res[i][j] = P5[i - m][j - m] + P1[i - m][j - m] - P3[i - m][j - m] - P7[i - m][j - m];
}

void mut_strassen_plus(const mat & A, const mat & B, mat & res)
{
	if (A.size() <= STRASSEN_THRESHOLD) {
		mut_brute(A, B, res);
		return;
	}
	int n = A.size(), m = (n + 1) >> 1;
	mat a(m), b(m, 1), c(m, 1), d(m, 1), e(m), f(m, 1), g(m, 1), h(m, 1);
	A.left_top(a); A.right_top(b); A.left_bottom(c); A.right_bottom(d);
	B.left_top(e); B.right_top(f); B.left_bottom(g), B.right_bottom(h);

	mat P1(m), P2(m), P3(m), P4(m), P5(m), P6(m), P7(m);

	mut_strassen_plus(a, f - h, P1); mut_strassen_plus(a + b, h, P2);
	mut_strassen_plus(c + d, e, P3); mut_strassen_plus(d, g - e, P4);
	mut_strassen_plus(a + d, e + h, P5); mut_strassen_plus(b - d, g + h, P6);
	mut_strassen_plus(a - c, e + f, P7);

	for (int i = 0; i < m; i++) for (int j = 0; j < m; j++) res[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
	for (int i = 0; i < m; i++) for (int j = m; j < n; j++) res[i][j] = P1[i][j - m] + P2[i][j - m];
	for (int i = m; i < n; i++) for (int j = 0; j < m; j++) res[i][j] = P3[i - m][j] + P4[i - m][j];
	for (int i = m; i < n; i++) for (int j = m; j < n; j++) res[i][j] = P5[i - m][j - m] + P1[i - m][j - m] - P3[i - m][j - m] - P7[i - m][j - m];
}
