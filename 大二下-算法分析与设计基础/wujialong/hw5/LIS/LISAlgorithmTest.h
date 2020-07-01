#pragma once
#include "AlgorithmTest.h"
#include "lis.h"


class LISAlgorithmTest :
	public AlgorithmTest
{
	int *a;
public:
	LISAlgorithmTest() { a = nullptr; }
	~LISAlgorithmTest() { if (a) delete[] a; }

	void generate_data(int n) override;
	double test(int f) override;
	void validate_algos();
};

