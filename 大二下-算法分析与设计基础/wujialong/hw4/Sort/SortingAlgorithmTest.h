#pragma once
#include "AlgorithmTest.h"
#include "sorts.h"
#include <ctime>
#include <cmath>
#include <windows.h>

class SortingAlgorithmTest :
	public AlgorithmTest
{
	value_t* a;
public:
	SortingAlgorithmTest() { a = nullptr; }
	~SortingAlgorithmTest() { if (a) delete[] a; }

	void generate_data(int n) override;
	double test(int f) override;
	void validate_algos();
};

