#pragma once

#include "Matrix.h"

void mut_brute(const mat &A, const mat &B, mat& res);

void mut_strassen(const mat &A, const mat &B, mat &res);

const int STRASSEN_THRESHOLD = 50;
void mut_strassen_plus(const mat &A, const mat &B, mat &res);