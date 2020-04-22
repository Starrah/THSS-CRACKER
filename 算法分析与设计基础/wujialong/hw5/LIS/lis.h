#pragma once
#include<algorithm>
#include<cassert>
#include<vector>

std::pair<int, int> lis_brute(int *a, int n, int *f, int *last);
std::pair<int, int> lis_nlgn(int *a, int n, int *c, int *last);

void gen_random_data(int *a, int n);
void validate_solution(int *a, int *last, std::pair<int, int> res);
void validate(int n);

std::vector<int> lis_adapter(const std::vector<int> &_a);