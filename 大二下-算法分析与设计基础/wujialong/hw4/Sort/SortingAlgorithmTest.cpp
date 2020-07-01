#include "SortingAlgorithmTest.h"

void SortingAlgorithmTest::generate_data(int n) {
	if (a == nullptr) a = new value_t[n + 1];
	gen_data(a, n);
}

double SortingAlgorithmTest::test(int f) {
	value_t *tmp = new value_t[n + 1];
	int r;
	int *c;

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	switch (f) {
	case 0:
		QueryPerformanceCounter(&nBeginTime);
		InsertionSort(a, n);
		QueryPerformanceCounter(&nEndTime);
		break;
	case 1:
		QueryPerformanceCounter(&nBeginTime);
		ShellSort(a, n);
		QueryPerformanceCounter(&nEndTime);
		break;
	case 2:
		QueryPerformanceCounter(&nBeginTime);
		QuickSort(a, 1, n);
		QueryPerformanceCounter(&nEndTime);
		break;
	case 3:
		QueryPerformanceCounter(&nBeginTime);
		MergeSort(a, 1, n, tmp);
		QueryPerformanceCounter(&nEndTime);
		break;
	case 4:
		r = 11;
		c = new int[1 << r];
		QueryPerformanceCounter(&nBeginTime);
		RadixSort(a, n, 32, r, tmp, c);
		QueryPerformanceCounter(&nEndTime);
		delete[] c;
		break;
	case 5:
		r = 16;
		c = new int[1 << r];
		QueryPerformanceCounter(&nBeginTime);
		RadixSort(a, n, 32, r, tmp, c);
		QueryPerformanceCounter(&nEndTime);
		delete[] c;
		break;
	case 6:
		r = (int)ceil(log(n) / log(2));
		c = new int[1 << r];
		QueryPerformanceCounter(&nBeginTime);
		RadixSort(a, n, 32, r, tmp, c);
		QueryPerformanceCounter(&nEndTime);
		delete[] c;
		break;
	}
	delete[] tmp;
	return (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
}

void SortingAlgorithmTest::validate_algos() {
	int cnt = 0;
	while (true) {
		validate(10000);
		cout << "OK! case # " << ++cnt << endl;
	}
}
