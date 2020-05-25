#pragma once

#include <vector>

extern const int charCnt;
extern const char charSet[];

/* 随机产生 P, T 结果大概率是 0 */
void gen_random_data(int m, char* P, int n, char* T);

/* 随机产生 P, T 并在 T 中若干位置用 P 覆盖，保证 P 出现 */
void gen_elaborate_data(int m, char* P, int n, char* T, double r);

void gen_general_data(int m, char* P, int n, char* T);

void gen_extreme_data(int m, char* P, int n, char* T);

void BruteMatcher(int n, char* T, int m, char* P, std::vector<int>& shifts);

void computePrefixFunction(int m, char* P, int* pi);

void KmpMatcher(int n, char* T, int m, char* P, int* pi, std::vector<int>& shifts);

void computeBMBC(int m, char* P, int* bmBc);

void computeOsuff(int m, char* P, int* Osuff);

void computeBMGS(int m, char* P, int* bmGs, int* Osuff);

void BmMatcher(int n, char* T, int m, char* P, int* bmBc, int* bmGs, int* Osuff, std::vector<int>& shifts);