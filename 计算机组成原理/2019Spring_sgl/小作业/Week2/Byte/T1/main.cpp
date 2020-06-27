#include<iostream>
using namespace std;

int g_AnyEvenOne(unsigned x)
{
	unsigned max = 0xffffffff;
	unsigned ans = x ^ max;
	return !ans;
}

int main()
{
	for (int i = 0; i < 10; i++)
	{
		unsigned x;
		cin >> x;
		cout << "The number is " << x << ",";
		int a = g_AnyEvenOne(x);
		cout << " the answer is " << a << endl;
	}

}