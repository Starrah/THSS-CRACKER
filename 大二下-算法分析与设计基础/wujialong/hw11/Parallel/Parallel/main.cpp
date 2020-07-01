#include "sorts.h"
#include "parallel_sorts.h"
#include "io.h"
#include "windows.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <cassert>
#include <iomanip>
using namespace std;

const int N = 5e7;
int n;
value_t a[N + 5], b[N + 5], tmp[N + 5];

int main(int argc, char* argv[]) {
	int n = -1, f = -1;
	int count = -1;
	bool no_pause = 0;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--n") == 0) {
			n = atoi(argv[i + 1]); no_pause = 1;
		}
		if (strcmp(argv[i], "--func") == 0) {
			f = atoi(argv[i + 1]); no_pause = 1;
		}
		if (strcmp(argv[i], "--count") == 0) {
			count = atoi(argv[i + 1]); no_pause = 1;
		}
	}
	if (n == -1) n = get_n();
	if (f == -1) f = get_f();
	if (count == -1) count = get_count();

	double total = 0, start, end;
	switch (f) {
	case 0: // serial MergeSort
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			MergeSort(a, 1, n, tmp);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 1: // parallel MergeSort（串行归并，无截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelMergeSort(a, 1, n, tmp, 0, 0);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 2: // parallel MergeSort（并行归并，无截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelMergeSort(a, 1, n, tmp, 1, 0);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 3: // parallel MergeSort（串行归并，有截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelMergeSort(a, 1, n, tmp, 0, 1);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 4: // parallel MergeSort（并行归并，有截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelMergeSort(a, 1, n, tmp, 1, 1);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 5: // serial QuickSort
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			QuickSort(a, 1, n);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 6: // parallel QuickSort（串行 partition，无截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelQuickSort(a, 1, n, 0);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	case 7: // parallel QuickSort（串行 partition，有截断）
		for (int i = 0; i < count; i++) {
			gen_data(a, n);
			start = omp_get_wtime();
			parallelQuickSort(a, 1, n, 1);
			end = omp_get_wtime();
			total += end - start;
		}
		break;
	}

	cout << "平均用时：" << total / count << " s" << endl;
	if (!no_pause)
		system("pause");
	return 0;
}
