#include "lis.h"
#include <cstdio>
#include <iostream>
using namespace std;

pair<int, int> lis_brute(int * a, int n, int * f, int * last) {
	for (int i = 1; i <= n; i++) {
		f[i] = 1; last[i] = 0;
		for (int j = 1; j < i; j++)
			if (a[j]<a[i] && f[j] + 1>f[i]) {
				f[i] = f[j] + 1;
				last[i] = j;
			}
	}
	pair<int, int> res(0, 0);
	for (int i = 1; i <= n; i++)
		if (f[i] > res.first)
			res = pair<int, int>(f[i], i);
	return res;
}

pair<int, int> lis_nlgn(int * a, int n, int * c, int * last) {
	int maxLen = 1;
	c[0] = 0; c[1] = 1; last[1] = 0;
	for (int i = 2; i <= n; i++)
		if (a[i] > a[c[maxLen]]) {
			c[++maxLen] = i; last[i] = c[maxLen - 1];
		}
		else {
			int p = lower_bound(c + 1, c + maxLen + 1, i, [&](int x, int y) { return a[x] < a[y]; }) - c;
			c[p] = i; last[i] = c[p - 1];
		}
	return pair<int, int>(maxLen, c[maxLen]);
}

void gen_random_data(int * a, int n)
{
	for (int i = 1; i <= n; i++) a[i] = ((rand() << 15) | rand()) % n + 1;
}

void validate_solution(int * a, int * last, pair<int, int> res) {
	int len = res.first; int tail = res.second; int cnt = 0;
	while (tail) {
		++cnt;
		if (last[tail])
			assert(last[tail] < tail && a[last[tail]] < a[tail]);
		tail = last[tail];
	}
	assert(cnt == len);
}

void validate(int n)
{
	int *a = new int[n + 1];
	int *c = new int[n + 1];
	int *last = new int[n + 1];
	gen_random_data(a, n);
	pair<int, int> res_brute = lis_brute(a, n, c, last);
	validate_solution(a, last, res_brute);
	pair<int, int> res_nlgn = lis_nlgn(a, n, c, last);
	validate_solution(a, last, res_nlgn);
	assert(res_brute.first == res_nlgn.first);
	cout << "Length of LIS = " << res_brute.first << endl;
	delete[] a; delete[] c; delete[] last;
}

std::vector<int> lis_adapter(const vector<int>& _a)
{
	int n = _a.size();
	int *a = new int[n + 1];
	int *c = new int[n + 1];
	int *last = new int[n + 1];
	for (int i = 1; i <= n; i++) a[i] = _a[i - 1];
	pair<int, int> res = lis_nlgn(a, n, c, last);
	vector<int> lis_idx;
	int tail = res.second;
	while (tail) {
		lis_idx.push_back(tail);
		tail = last[tail];
	}
	reverse(lis_idx.begin(), lis_idx.end());
	delete[] a; delete[] c; delete[] last;
	return lis_idx;
}
