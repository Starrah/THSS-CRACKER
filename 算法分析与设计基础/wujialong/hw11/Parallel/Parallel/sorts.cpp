#include "sorts.h"
#include<cstdio>
#include<cstdlib>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
using namespace std;

#define rand15 rand

value_t rand32() {
	return (((value_t)rand15()) << 17) | (((value_t)rand15()) << 2) | (((value_t)rand15()) & 3);
}

void gen_data(value_t *a, int n) {
	for (int i = 1; i <= n; i++) a[i] = rand32();
}

void print_data(value_t *a, int n) {
	for (int i = 1; i <= n; i++) cout << a[i] << " "; cout << endl;
}

void InsertionSort(value_t * a, int n) {
	for (int i = 2; i <= n; i++) {
		a[0] = a[i]; int j;
		for (j = i - 1; a[0] < a[j]; j--)
			a[j + 1] = a[j];
		a[j + 1] = a[0];
	}
}

void ShellInsert(value_t * a, int n, int d) {
	for (int i = 1 + d; i <= n; i++) {
		a[0] = a[i]; int j;
		for (j = i - d; j > 0 && a[0] < a[j]; j -= d)
			a[j + d] = a[j];
		a[j + d] = a[0];
	}
}

void ShellSort(value_t * a, int n) {
	for (int d = n >> 1; d; d >>= 1) {
		d |= 1;
		ShellInsert(a, n, d);
	}
}

int Partition(value_t * a, int l, int r) {
	int pivot = a[r];
	while (l < r) {
		while (l < r && a[l] <= pivot) ++l;
		a[r] = a[l];
		while (l < r && a[r] >= pivot) --r;
		a[l] = a[r];
	}
	a[l] = pivot; return l;
}

void QuickSort(value_t * a, int l, int r) {
	if (l < r) {
		int mid = Partition(a, l, r);
		QuickSort(a, l, mid - 1);
		QuickSort(a, mid + 1, r);
	}
}

void MergeSort(value_t * a, int l, int r, value_t * tmp) {
	if (l == r) return;
	int mid = (l + r) >> 1;
	MergeSort(a, l, mid, tmp);
	MergeSort(a, mid + 1, r, tmp);
	int p = l - 1, p1 = l, p2 = mid + 1;
	while (p1 <= mid && p2 <= r) {
		if (a[p1] < a[p2]) tmp[++p] = a[p1++];
		else tmp[++p] = a[p2++];
	}
	while (p1 <= mid) tmp[++p] = a[p1++];
	while (p2 <= r) tmp[++p] = a[p2++];
	for (int i = l; i <= r; i++) a[i] = tmp[i];
}

void RadixSort(value_t * A, int n, int w, int r, value_t * tmp, int * c) {
	value_t *a = A, *b = tmp, x = 0;
	int round = (w + r - 1) / r, base = (1 << r) - 1;
	for (int j = 0; j < round; j++) {
		for (int i = 0; i <= base; i++) c[i] = 0;
		for (int i = 1; i <= n; i++) ++c[a[i] >> x & base];
		for (int i = 1; i <= base; i++) c[i] += c[i - 1];
		for (int i = n; i; i--) b[c[a[i] >> x & base]--] = a[i];
		value_t *tmp = a; a = b; b = tmp;
		x += r;
	}
	if (a != A) for (int i = 1; i <= n; i++) A[i] = a[i];
}

void validate(int n) {
	value_t* a = new value_t[n + 1]; gen_data(a, n);
	value_t* a_insertion = new value_t[n + 1]; memcpy(a_insertion, a, sizeof(int)*(n + 1));
	value_t* a_shell = new value_t[n + 1]; memcpy(a_shell, a, sizeof(int)*(n + 1));
	value_t* a_quick = new value_t[n + 1]; memcpy(a_quick, a, sizeof(int)*(n + 1));
	value_t* a_merge = new value_t[n + 1]; memcpy(a_merge, a, sizeof(int)*(n + 1));
	value_t* a_radix = new value_t[n + 1]; memcpy(a_radix, a, sizeof(int)*(n + 1));
	value_t* tmp = new value_t[n + 1];

	InsertionSort(a_insertion, n);
	ShellSort(a_shell, n);
	QuickSort(a_quick, 1, n);
	MergeSort(a_merge, 1, n, tmp);
	int r = 11;
	int *c = new int[1 << r];
	RadixSort(a_radix, n, 32, r, tmp, c);

	assert(!memcmp(a_insertion + 1, a_shell + 1, sizeof(int)*n));
	assert(!memcmp(a_shell + 1, a_quick + 1, sizeof(int)*n));
	assert(!memcmp(a_quick + 1, a_merge + 1, sizeof(int)*n));
	assert(!memcmp(a_merge + 1, a_radix + 1, sizeof(int)*n));
	
	delete[] a; delete[] a_insertion; delete[] a_shell; delete[] a_quick;
	delete[] a_merge; delete[] a_radix;
	delete[] tmp; delete[] c;
}
