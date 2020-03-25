#include "MyMath.h"

mat22::mat22() {
	cl(a);
}

mat22::mat22(int diag) {
	a[0][0] = a[1][1] = diag; a[0][1] = a[1][0] = 0;
}

mat22::mat22(int a11, int a12, int a21, int a22) {
	a[0][0] = a11; a[0][1] = a12; a[1][0] = a21; a[1][1] = a22;
}

mat22 operator * (const mat22 &A, const mat22 &B) {
	mat22 ret;
	for (int k = 0; k < mat22::mat_size; k++)
		for (int i = 0; i < mat22::mat_size; i++)
			for (int j = 0; j < mat22::mat_size; j++)
				ret[i][j] += A[i][k] * B[k][j];
	return ret;
}