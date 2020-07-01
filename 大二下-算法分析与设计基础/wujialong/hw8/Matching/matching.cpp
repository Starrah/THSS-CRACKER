#include "matching.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

const int charCnt = 42;
const char charSet[] = "qwertyuiopasdfghjklzxcvbnm0123456789,.?!; ";

void gen_random_data(int m, char* P, int n, char* T) {
	for (int i = 1; i <= m; i++) P[i] = charSet[rand() % charCnt];
	for (int i = 1; i <= n; i++) T[i] = charSet[rand() % charCnt];
}

void gen_elaborate_data(int m, char* P, int n, char* T, double r) {
	gen_random_data(m, P, n, T);
	for (int i = 1; i <= n / m * r; i++) {
		int x = rand() % n + 1;
		for (int j = 1; x + j - 1 <= n && j <= m; j++)
			T[x + j - 1] = P[j];
	}
}

void gen_general_data(int m, char* P, int n, char* T)
{
	vector<string> words;
	for (int i = 1; i <= m; i++) P[i] = charSet[rand() % charCnt]; P[m + 1] = 0;
	words.push_back(string(P+1));
	for (int i = 1; i <= int(n / m * 0.01); i++) {
		string tmp; int len = rand() % (2 * m) + 1;
		for (int j = 1; j <= len; j++) tmp.push_back(charSet[rand() % charCnt]);
		words.push_back(tmp);
	}
	int p = 1;
	while (p <= n) {
		int x = rand() % words.size();
		for (int j = 0; j < words[x].length() && p <= n; j++)
			T[p++] = words[x][j];
	}
	T[p] = 0;
}

void gen_extreme_data(int m, char* P, int n, char* T)
{
	char c = charSet[rand() % charCnt];
	for (int i = 1; i <= m; i++) P[i] = c;
	for (int i = 1; i <= n; i++) T[i] = c;
}

void BruteMatcher(int n, char* T, int m, char* P, std::vector<int>& shifts) {
	for (int i = 1; i <= n - m + 1; i++) {
		int j = 0;
		while (j < m && T[i + j] == P[j + 1]) j++;
		if (j == m) {
			// printf("Pattern occurs with shift %d\n", i-m);
			shifts.push_back(i - 1);
		}
	}
}

void computePrefixFunction(int m, char* P, int* pi) {
	int k = 0; pi[1] = 0;
	for (int i = 2; i <= m; i++) {
		while (k && P[i] != P[k + 1]) k = pi[k];
		if (P[i] == P[k + 1]) k++;
		pi[i] = k;
	}
}

void KmpMatcher(int n, char* T, int m, char* P, int* pi, std::vector<int>& shifts) {
	computePrefixFunction(m, P, pi);
	int k = 0;
	for (int i = 1; i <= n; i++) {
		while (k && T[i] != P[k + 1]) k = pi[k];
		if (T[i] == P[k + 1]) k++;
		if (k == m) {
			// printf("Pattern occurs with shift %d\n", i-m);
			shifts.push_back(i - m);
			k = pi[k];
		}
	}
}

void computeBMBC(int m, char* P, int* bmBc) {
	for (int i = 0; i < charCnt; i++) bmBc[charSet[i]] = m;
	for (int i = 1; i < m; i++) bmBc[P[i]] = m - i;
}

void computeOsuff(int m, char* P, int* Osuff) {
	Osuff[m] = m;
	int g = m, f = 0;
	/* Ref:[Boyer- Moore算法_百度百科](https://baike.baidu.com/item/Boyer-%20Moore%E7%AE%97%E6%B3%95/16548374?fr=aladdin) */
	/* g 只会下降，复杂度为 O(m) */
	for (int i = m - 1; i; i--) {
		if (i > g && Osuff[m - (f - i)] < i - g)
			Osuff[i] = Osuff[m - (f - i)];
		else {
			if (i < g) g = i;
			f = i;
			while (g && P[g] == P[m - (i - g)]) g--;
			Osuff[i] = i - g;
		}
	}
}

void computeBMGS(int m, char* P, int* bmGs, int* Osuff) {
	computeOsuff(m, P, Osuff);
	for (int i = 1; i <= m; i++) bmGs[i] = m;
	int j = 1;
	for (int i = m - 1; i; i--)
		if (Osuff[i] == i) {
			while (j <= m - i) {
				if (bmGs[j] == m) bmGs[j] = m - i;
				j++;
			}
		}
	for (int i = 1; i < m; i++)
		bmGs[m - Osuff[i]] = m - i;
}

void BmMatcher(int n, char* T, int m, char* P, int* bmBc, int* bmGs, int* Osuff, std::vector<int>& shifts) {
	computeBMBC(m, P, bmBc);
	computeBMGS(m, P, bmGs, Osuff);
	int s = 0;
	while (s <= n - m) {
		int i = m;
		while (P[i] == T[s + i]) {
			if (i == 1) {
				//printf("Pattern occurs with shift %d\n", s);
				shifts.push_back(s); break;
			}
			else {
				i = i - 1;
			}
		}
		s = s + max(bmGs[i], bmBc[T[s + i]] - m + i);
	}
}
