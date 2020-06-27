/*
filename: GGlobalFunctions.cpp
description: global mathematical functions
version: 1.0
date: 10/21/2018
*/
#include "NCharString.h"

int g_JudgeMin(int a1, int a2, int a3)
{
	if (a1 != -1 && a1 < a2 && a1 <= a3)
	{
		return 1;
	}
	else if (a2 != -1 && a2 <= a1 && a2 <= a3)
	{
		return 2;
	}
	else if (a3 != -1 && a3 <= a1 && a3 <= a2)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

int g_GetMinNumber(int a1, int a2, int a3)
{
	if (a1 != -1 && a1 < a2 && a1 <= a3)
	{
		return a1;
	}
	else if (a2 != -1 && a2 <= a1 && a2 <= a3)
	{
		return a2;
	}
	else if (a3 != -1 && a3 <= a1 && a3 <= a2)
	{
		return a3;
	}
	else
	{
		return -1;
	}
}

int g_SwitchCharToInt(char c[], int end)
{
	int multiply = 1;
	int ans = 0;
	for (int i = end - 1; i >= 0; i--)
	{
		ans += multiply * (c[i] - '0');
		multiply *= 10;
	}
	return ans;
}

void g_MoveRightAList(int c[], int length, int new_num)
{
	for (int i = length - 1; i >= 1; i--)
	{
		int temp = c[i];
		c[i] = c[i - 1];
		c[i - 1] = temp;
	}
	c[0] = new_num;
}

void g_MoveRightAList(double c[], int length, double new_num)
{
	for (int i = length - 1; i >= 1; i--)
	{
		double temp = c[i];
		c[i] = c[i - 1];
		c[i - 1] = temp;
	}
	c[0] = new_num;
}

NCharString* g_SwitchIntToChar(int to_be_done)
{
    char c[10]={0};
    int num = 0;
    if(to_be_done <= 9)
    {
        num = 1;
        c[0] = to_be_done + '0';
    }
    else if(to_be_done >= 10 && to_be_done <= 99)
    {
        num = 2;
        c[1] = (to_be_done % 10) + '0';
        c[0] = (to_be_done / 10) + '0';
    }
    else
    {
        num = 3;
        c[2] = (to_be_done % 10) + '0';
        c[1] = (to_be_done - (to_be_done / 100) * 100) / 10 + '0';
        c[0] = (to_be_done / 100) + '0';
    }
    NCharString* new_char = new NCharString(c, num);
    return new_char;
}

NCharString* g_GetFileName(int file_num)
{
    NCharString* file_middle = g_SwitchIntToChar(file_num);
    char head[10] = "output/";
    char tail[10] = ".info";
    NCharString* file_name = new NCharString (head, 7);
    NCharString* file_back = new NCharString (tail, 5);
    file_name ->m_PushBackString(file_middle);
    file_name ->m_PushBackString(file_back);
    delete(file_middle);
    delete(file_back);
    return file_name;
}
