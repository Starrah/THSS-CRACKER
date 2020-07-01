#include "SortingAlgorithmTest.h"
#include <iomanip>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	SortingAlgorithmTest test;
	test.set_algos(7, "插入排序",
		"希尔排序",
		"快速排序",
		"归并排序",
		"基数排序 with r=11",
		"基数排序 with r=16",
		"基数排序 with r=logn");
	bool no_pause = test.get_input(argc, argv);
	double t = test.run();
	cout << "algo = " << test.getAlgo() << endl;
	cout << "n = " << test.getN() << endl;
	cout << "平均用时：" << fixed << setprecision(10) << setiosflags(ios::showpoint) << t  << " s" << endl;
	if (!no_pause)
		system("pause");
	return 0;
}

