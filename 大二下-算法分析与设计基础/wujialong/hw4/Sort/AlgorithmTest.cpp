#include "AlgorithmTest.h"

void AlgorithmTest::set_algos(int algo_num, ...) {
	this->algo_num = algo_num;
	va_list arg_ptr;
	va_start(arg_ptr, algo_num);
	for (int i = 0; i < algo_num; i++)
	{
		desc.push_back(string(va_arg(arg_ptr, char*)));
	}
	va_end(arg_ptr);
}

bool AlgorithmTest::get_input(int argc, char * argv[]) {
	n = -1;
	f = -1;
	count = -1;
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
	if (n == -1)
		n = get_n();
	if (f == -1)
		f = get_f();
	if (count == -1)
		count = get_count();
	return no_pause;
}

int AlgorithmTest::get_n() {
	int n;
	while (true) {
		cout << "输入 n：" << endl;
		if (!(cin >> n) || n < 0) {
			cout << "输入有误！" << endl;
			cin.clear(istream::goodbit);
			cin.ignore();
		}
		else {
			break;
		}
	}
	return n;
}

int AlgorithmTest::get_count() {
	int count;
	while (true) {
		cout << "输入计算次数 count：（多次计算平均用时）" << endl;
		if (!(cin >> count) || count <= 0) {
			cout << "输入有误！" << endl;
			cin.clear(istream::goodbit);
			cin.ignore();
		}
		else {
			break;
		}
	}
	return count;
}

int AlgorithmTest::get_f() {
	int idx;
	while (true) {
		cout << "输入所用函数序号：" << endl;
		for (int i = 0; i < algo_num; i++) {
			cout << "\t" << i << "：" << desc[i] << endl;
		}
		cin >> idx;
		if (!cin.good() || !(idx >= 0 && idx < algo_num)) {
			cout << "输入有误！" << endl;
			cin.clear(istream::goodbit);
			cin.ignore();
		}
		else {
			break;
		}
	}
	return idx;
}

double AlgorithmTest::run() {
	double sum = 0;
	for (int i = 0; i < count; i++) {
		generate_data(n);
		sum += test(f);
	}
	return sum / count;
}
