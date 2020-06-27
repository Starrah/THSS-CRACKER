#include <iostream>
#include <limits>
using namespace std;

int g_SatuatingAdd(int x, int y)
{
	int ans = x + y;
	int num_x = x & 0x7fffffff;
	int num_y = y & 0x7fffffff;

	int sign_x = x >> 31;
	int sign_y = y >> 31;
	int sign_ans = ans >> 31;
	
	int whether_is_different_sign = (sign_x ^ sign_y); 
	//if this == 1, cannot satuate

	int whether_up_satuate = (!sign_x) && (sign_ans) && (!whether_is_different_sign); 
	//if this == 1, up satuate

	int whether_down_satuate = (sign_x) && (!sign_ans) && (!whether_is_different_sign);
	//if this == 1, down satuate

	int whether_change = whether_up_satuate | whether_down_satuate;

	int step = (whether_change << 31) - whether_change;
	//not satuate: step all 0
	//up satuate: step maxint
	//down satuate: step maxint

	ans -= (whether_up_satuate << 31);
	ans = ans | step;
	ans += whether_down_satuate;

	return ans;
}

int main()
{
	while (1);
	for (int i = 0; i <= 10; i++)
	{
		int x, y;
		cin >> x >> y;
		int ans = g_SatuatingAdd(x, y);
		cout << x << " + " << y << " = " << ans << endl;
	}
}