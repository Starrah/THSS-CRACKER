#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdarg>
using namespace std;

class AlgorithmTest
{
protected:
	int n, f, count;
	int algo_num;
	vector<string> desc;
public:
	AlgorithmTest() { }
	~AlgorithmTest() { }

	void set_algos(int algo_num, ...);

	bool get_input(int argc, char* argv[]);
	int get_n();
	int get_count();
	int get_f();

	int getN() const { return n; }
	string getAlgo() const { return desc[f]; }

	virtual void generate_data(int n) = 0;
	virtual double test(int f) = 0;
	virtual double run();
};

