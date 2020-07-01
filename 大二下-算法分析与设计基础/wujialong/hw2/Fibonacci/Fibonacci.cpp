#include "Fibonacci.h"

ull fibo_formula(int n) { // not correct when n>70
	double phi = (1 + sqrt(5)) / 2;
	return (ull)round(quick_pow_nonrecursive(phi, n) / sqrt(5));
}

ull fibo_brute_recursive(int n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	return fibo_brute_recursive(n - 2) + fibo_brute_recursive(n - 1);
}

ull fibo_recurrence(int n) {
	if (n == 0) return 0;
	ull f0 = 0, f1 = 1;
	while (--n) {
		ull t = f0 + f1;
		f0 = f1; f1 = t;
	}
	return f1;
}

ull fibo_mat_nonrecursive(int n) {
	mat22 ret(1, 0, 0, 1), a(1, 1, 1, 0);
	int b = n;
	for (; b; b >>= 1, a = a * a)
		if (b & 1)
			ret = ret * a;
	return ret[0][1];
}

ull fibo_mat_recursive(int n) {
	return quick_pow_recursive(mat22(1, 1, 1, 0), n)[0][1];
}
