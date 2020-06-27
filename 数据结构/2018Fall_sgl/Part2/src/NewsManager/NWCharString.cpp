#define _CRT_SECURE_NO_WARNINGS
/*
filename: NWCharSting.cpp
description: a Wchar string
version: 1.0
date: 10/20/2018
*/

#include <iostream>
#include <malloc.h>
#include <fstream>
#include <Windows.h>


using namespace std;

#include "NCharString.h"
#include "NWCharString.h"

NWCharString::NWCharString(const wchar_t c[], const int length)
{
	m_Length = length;
	m_String = (wchar_t*)malloc(sizeof(wchar_t)*length);
	m_Capacity = m_Length;
	for (int i = 0; i < length; i++)
	{
		m_String[i] = c[i];
	}
}


NWCharString* NWCharString::m_CutString(const int start, const int end)
{
	if (start < 0 || start > end || end >= m_Length)
	{
		return NULL;
	}
	NWCharString* new_string = new NWCharString();
	new_string->m_String = (wchar_t*)malloc(sizeof(wchar_t)*(end - start + 1));
	new_string->m_Length = end - start + 1;
	new_string->m_Capacity = end - start + 1;
	for (int i = 0; i <= end - start; i++)
	{
		new_string->m_String[i] = m_String[start + i];
	}
	return new_string;
}

void NWCharString::m_SetValue(const wchar_t c[], const int length)
{
	if (m_String != NULL)
	{
		free(m_String);
		m_String = NULL;
	}
	m_Length = length;
	m_Capacity = length;
	m_String = (wchar_t*)malloc(sizeof(wchar_t)*length);
	for (int i = 0; i < length; i++)
	{
		m_String[i] = c[i];
	}
}

void NWCharString::m_SetValue(const NWCharString* new_string)
{
	if (m_String != NULL)
	{
		free(m_String);
		m_String = NULL;
	}
	m_Length = new_string->m_Length;
	m_Capacity = new_string->m_Length;
	m_String = (wchar_t*)malloc(sizeof(wchar_t)*m_Length);
	for (int i = 0; i < m_Length; i++)
	{
		m_String[i] = new_string->m_String[i];
	}
}

void NWCharString::m_PushBackString(const NWCharString* new_string)
{
	if (new_string == NULL)
	{
		return;
	}
	if (m_String != NULL)
	{
		int old_length = m_Length;
		m_Length += new_string->m_Length;
		m_Capacity += new_string->m_Length;
		m_String = (wchar_t*)realloc(m_String, sizeof(wchar_t) * m_Length);
		for (int i = old_length; i < m_Length; i++)
		{
			m_String[i] = new_string->m_String[i - old_length];
		}
	}
	else
	{
		m_SetValue(new_string);
	}
}

NWCharString* f_ConCat(const NWCharString* first_string, const NWCharString* second_string)
{
	NWCharString* new_string = new NWCharString();
	new_string->m_Length = first_string->m_Length + second_string->m_Length;
	new_string->m_Capacity = first_string->m_Length + second_string->m_Length;
	new_string->m_String = (wchar_t*)malloc(sizeof(wchar_t)*(new_string->m_Length));
	for (int i = 0; i < first_string->m_Length; i++)
	{
		new_string->m_String[i] = first_string->m_String[i];
	}
	for (int i = first_string->m_Length; i < new_string->m_Length; i++)
	{
		new_string->m_String[i] = second_string->m_String[i - first_string->m_Length];
	}
	return new_string;
}

void NWCharString::m_InsertBeforeString(const NWCharString* new_string)
{

	if (m_String != NULL)
	{
		int old_length = m_Length;
		wchar_t* old_string = (wchar_t*)malloc(m_Length * sizeof(wchar_t));
		for (int i = 0; i < m_Length; i++)
		{
			old_string[i] = m_String[i];
		}
		free(m_String);
		m_Length += new_string->m_Length;
		m_Capacity += new_string->m_Length;
		m_String = (wchar_t*)malloc(m_Length * sizeof(wchar_t));
		for (int i = 0; i < new_string->m_Length; i++)
		{
			m_String[i] = new_string->m_String[i];
		}
		for (int i = new_string->m_Length; i < m_Length; i++)
		{
			m_String[i] = old_string[new_string->m_Length];
		}
	}
	else
	{
		m_SetValue(new_string);
	}

}

int NWCharString::m_Compare(const NWCharString* new_string)
{
	if (new_string == NULL)
	{
		return 1;
	}
	int min_length;
	if (m_Length <= new_string->m_Length)
	{
		min_length = m_Length;
	}
	else
	{
		min_length = new_string->m_Length;
	}
	for (int i = 0; i < min_length; i++)
	{
		if (m_String[i] < new_string->m_String[i])
		{
			return -1;
		}
		if (m_String[i] > new_string->m_String[i])
		{
			return 1;
		}
	}
	if (m_Length < new_string->m_Length)
	{
		return -1;
	}
	if (m_Length > new_string->m_Length)
	{
		return 1;
	}
	return 0;
}

void NWCharString::m_PushBackChar(const char new_char)
{
	if (m_Length == m_Capacity)
	{
		m_Capacity += c_ReallocNumber;
		m_String = (wchar_t*)realloc(m_String, m_Capacity * sizeof(wchar_t));
	}
	m_Length++;
	m_String[m_Length - 1] = new_char;
}


void NWCharString::m_FindNextFunction()
{
	if (m_Next != NULL)
	{
		free(m_Next);
	}
	m_Next = (int*)malloc(m_Length * sizeof(int));
	for (int i = 0; i < m_Length; i++)
	{
		if (i == 0)
		{
			m_Next[i] = -1;
		}
		else if (i == 1)
		{
			m_Next[i] = 0;
		}
		else
		{
			int next_i = 0;
			for (int j = i - 1; j >= 1; j--)
			{
				NWCharString* sub_front = m_CutString(0, j - 1);
				NWCharString* sub_back = m_CutString(i - j, i - 1);
				if (sub_front->m_Compare(sub_back) == 0)
				{
					next_i = j;
					break;
				}
				delete(sub_front);
				delete(sub_back);
			}
			m_Next[i] = next_i;
		}
	}
}

int NWCharString::m_FindSubString(NWCharString* sub_string, int m_StartPlace)
{
	sub_string->m_FindNextFunction();
	int i = m_StartPlace, j = 0;
	while (1)
	{
		//quit
		if (j >= sub_string->m_Length)
		{
			return i - sub_string->m_Length;
		}
		else if (i >= m_Length)
		{
			return -1;
		}
		else
		{
			if (m_String[i] == sub_string->m_String[j])
			{
				i++;
				j++;
			}
			else
			{
				if (sub_string->m_Next[j] == -1)
				{
					j = 0;
					i++;
				}
				else
				{
					j = sub_string->m_Next[j];
				}
			}
		}

	}
}

void NWCharString::m_OutPut(char filename[])
{

	wfstream file(filename, ios::app);
	file.imbue(std::locale("chs"));
	for (int i = 0; i < m_Length; i++)
	{
		file << m_String[i];
	}
	file << "\n";
}

NWCharString::NWCharString(NCharString* old_string)
{
	m_Length = old_string->m_Length;
	m_Capacity = old_string->m_Capacity;
	m_String = (wchar_t*)malloc(m_Capacity * sizeof(wchar_t));
	mbstowcs(m_String, old_string->m_String, m_Length);
}

void NWCharString::m_PopBackChar()
{
	m_String[m_Length - 1] = 0;
	m_Length--;
}