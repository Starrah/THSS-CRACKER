#pragma once
#include<cstring>
#include<cmath>
#define cl(x) memset(x,0,sizeof(x))
typedef unsigned long long ull;

class mat22 {
private:
	ull a[2][2];
public:
	static const int mat_size = 2;
	mat22();
	mat22(int diag);
	mat22(int a11, int a12, int a21, int a22);
	ull *operator[](int x) { return a[x]; }
	const ull *operator[](int x) const { return a[x]; }
	friend mat22 operator * (const mat22 &A, const mat22 &B);
};

mat22 operator * (const mat22 &A, const mat22 &B);

template<typename T>
T quick_pow_recursive(const T &a, int b) {
	if (b == 1) return a;
	T ret = quick_pow_recursive(a, b >> 1);
	if (b & 1)
		return ret * ret*a;
	else
		return ret * ret;
}

template<typename T>
T quick_pow_nonrecursive(const T &a, int b) {
	T ret = 1, t = a;
	for (; b; b >>= 1, t = t * t)
		if (b & 1)
			ret = ret * t;
	return ret;
}