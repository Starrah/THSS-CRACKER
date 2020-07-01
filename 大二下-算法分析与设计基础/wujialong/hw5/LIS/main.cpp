#include "LISAlgorithmTest.h"
#include <iomanip>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	LISAlgorithmTest test;
	test.set_algos(2, "n^2 algorithm",
		"nlgn algorithm");
	//test.validate_algos();
	bool no_pause = test.get_input(argc, argv);
	double t = test.run();
	cout << "algo = " << test.getAlgo() << endl;
	cout << "n = " << test.getN() << endl;
	cout << "平均用时：" << fixed << setprecision(10) << setiosflags(ios::showpoint) << t << " s" << endl;
	if (!no_pause)
		system("pause");
	return 0;
}

