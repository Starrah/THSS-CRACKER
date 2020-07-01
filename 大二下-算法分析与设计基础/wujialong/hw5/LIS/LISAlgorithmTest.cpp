#include "LISAlgorithmTest.h"
#include <ctime>
#include <cmath>
#include <windows.h>

void LISAlgorithmTest::generate_data(int n)
{
	if (a == nullptr) a = new int[n + 1];
	gen_random_data(a, n);
}

double LISAlgorithmTest::test(int f)
{
	int *c = new int[n + 1];
	int *last = new int[n + 1];
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	switch (f) {
	case 0:
		QueryPerformanceCounter(&nBeginTime);
		lis_brute(a, n, c, last);
		QueryPerformanceCounter(&nEndTime);
		break;
	case 1:
		QueryPerformanceCounter(&nBeginTime);
		lis_nlgn(a, n, c, last);
		QueryPerformanceCounter(&nEndTime);
		break;
	}
	delete[] c; delete[] last;
	return (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
}

void LISAlgorithmTest::validate_algos()
{
	int cnt = 0;
	while (true) {
		validate(10000);
		cout << "OK! case # " << ++cnt << endl;
	}
}

