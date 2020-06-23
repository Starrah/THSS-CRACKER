#pragma once
#include "sorts.h"

void parallelMerge(value_t* tmp, int p1, int r1, int p2, int r2, value_t* a, int p3);

void parallelMergeSort(value_t* a, int l, int r, value_t* tmp, bool pMerge = false, bool cut = true, int depth = 0);

void parallelQuickSort(value_t* a, int l, int r, bool cut = 1, int depth = 0);