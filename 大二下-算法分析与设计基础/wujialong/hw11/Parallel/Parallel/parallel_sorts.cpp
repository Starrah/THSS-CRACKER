#include "parallel_sorts.h"
#include <algorithm>
using namespace std;

void parallelMerge(value_t* tmp, int p1, int r1, int p2, int r2, value_t* a, int p3)
{
	int n1 = r1 - p1 + 1, n2 = r2 - p2 + 1;
	if (n1 < n2) {
		swap(p1, p2); swap(n1, n2); swap(r1, r2);
	}
	if (n1 == 0) return;
	int q1 = (p1 + r1) >> 1;
	int q2 = lower_bound(tmp + p2, tmp + r2 + 1, tmp[q1]) - tmp;
	int q3 = p3 + q1 - p1 + q2 - p2;
	a[q3] = tmp[q1];

#pragma omp parallel sections
	{
#pragma omp section
		parallelMerge(tmp, p1, q1 - 1, p2, q2 - 1, a, p3);
#pragma omp section
		parallelMerge(tmp, q1 + 1, r1, q2, r2, a, q3 + 1);
	}
}

void parallelMergeSort(value_t* a, int l, int r, value_t *tmp, bool pMerge, bool cut, int depth)
{
	if (l == r) return;
	int mid = (l + r) >> 1;

	if (cut && depth >= 3) {
		MergeSort(a, l, mid, tmp);
		MergeSort(a, mid + 1, r, tmp);
	}
	else {
#pragma omp parallel sections
		{
#pragma omp section
			parallelMergeSort(a, l, mid, tmp, pMerge, cut, depth + 1);
#pragma omp section
			parallelMergeSort(a, mid + 1, r, tmp, pMerge, cut, depth + 1);
		}
	}

	if (!pMerge) {
		int p = l - 1, p1 = l, p2 = mid + 1;
		while (p1 <= mid && p2 <= r) {
			if (a[p1] < a[p2]) tmp[++p] = a[p1++];
			else tmp[++p] = a[p2++];
		}
		while (p1 <= mid) tmp[++p] = a[p1++];
		while (p2 <= r) tmp[++p] = a[p2++];
		for (int i = l; i <= r; i++) a[i] = tmp[i];
	}
	else {
//#pragma omp parallel for
		for (int i = l; i <= r; i++) tmp[i] = a[i];
		parallelMerge(tmp, l, mid, mid + 1, r, a, l);
	}
}

void parallelQuickSort(value_t* a, int l, int r, bool cut, int depth) {
	if (l < r) {
		int mid = Partition(a, l, r);
		if (cut && depth >= 3) {
			QuickSort(a, l, mid - 1);
			QuickSort(a, mid + 1, r);
		}
		else {
#pragma omp parallel sections 
			{
#pragma omp section
				parallelQuickSort(a, l, mid - 1, cut, depth + 1);
#pragma omp section
				parallelQuickSort(a, mid + 1, r, cut, depth + 1);
			}
		}
	}
}
