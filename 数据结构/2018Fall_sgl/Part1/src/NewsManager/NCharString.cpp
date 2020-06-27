/*
filename: NCharSting.cpp
description: a string
version: 1.0
date: 10/20/2018
*/

#include <iostream>
#include <malloc.h>
#include <fstream>


using namespace std;

#include "NCharString.h"

NCharString::NCharString(const char c[], const int length)
{
	m_Length = length;
	m_String = (char*)malloc(length);
	m_Capacity = m_Length;
	for (int i = 0; i < length; i++)
	{
		m_String[i] = c[i];
	}
}


NCharString* NCharString::m_CutString(const int start, const int end)
{
	if (start < 0 || start > end || end >= m_Length)
	{
		return NULL;
	}
	NCharString* new_string = new NCharString();
	new_string->m_String = (char*)malloc(end - start + 1);
	new_string->m_Length = end - start + 1;
	new_string->m_Capacity = end - start + 1;
	for (int i = 0; i <= end - start; i++)
	{
		new_string->m_String[i] = m_String[start + i];
	}
	return new_string;
}

void NCharString::m_SetValue(const char c[], const int length)
{
	if (m_String != NULL)
	{
		free(m_String);
		m_String = NULL;
	}
	m_Length = length;
	m_Capacity = length;
	m_String = (char*)malloc(length);
	for (int i = 0; i < length; i++)
	{
		m_String[i] = c[i];
	}
}

void NCharString::m_SetValue(const NCharString* new_string)
{
	if (m_String != NULL)
	{
		free(m_String);
		m_String = NULL;
	}
	m_Length = new_string->m_Length;
	m_Capacity = new_string->m_Length;
	m_String = (char*)malloc(m_Length);
	for (int i = 0; i < m_Length; i++)
	{
		m_String[i] = new_string->m_String[i];
	}
}

void NCharString::m_PushBackString(const NCharString* new_string)
{
	if (new_string == NULL)
	{
		return;
	}
	if (m_String != NULL)
	{
		int old_length = m_Length;
		m_Length += new_string->m_Length;
		//m_Capacity += new_string->m_Length;
		if (m_Length > m_Capacity)
		{
			m_Capacity = m_Length;
			/*char* temp_string = (char*)malloc(old_length * sizeof(char));
			for (int i = 0; i < old_length; i++)
			{
				temp_string[i] = m_String[i];
			}*/
			m_String = (char*)realloc(m_String, m_Capacity * sizeof(char));
			/*for (int i = 0; i < old_length; i++)
			{
				m_String[i] = temp_string[i];
			}
			free(temp_string);*/
		}

		for (int i = old_length; i < m_Length; i++)
		{
			m_String[i] = new_string->m_String[i - old_length];
		}
		/*for (int i = m_Length; i < strlen(m_String); i++)
		{
			m_String[i] = 0;
		}*/

	}
	else
	{
		m_SetValue(new_string);
	}
}

NCharString* f_ConCat(const NCharString* first_string, const NCharString* second_string)
{
	NCharString* new_string = new NCharString();
	new_string->m_Length = first_string->m_Length + second_string->m_Length;
	new_string->m_Capacity = first_string->m_Length + second_string->m_Length;
	new_string->m_String = (char*)malloc(new_string->m_Length);
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

void NCharString::m_InsertBeforeString(const NCharString* new_string)
{

	if (m_String != NULL)
	{
		int old_length = m_Length;
		char* old_string = (char*)malloc(m_Length * sizeof(char));
		for (int i = 0; i < m_Length; i++)
		{
			old_string[i] = m_String[i];
		}
		free(m_String);
		m_Length += new_string->m_Length;
		m_Capacity += new_string->m_Length;
		m_String = (char*)malloc(m_Length * sizeof(char));
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

int NCharString::m_Compare(const NCharString* new_string)
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

void NCharString::m_PushBackChar(const char new_char)
{
	if (m_Length == m_Capacity)
	{
		m_Capacity += c_ReallocNumber;
		m_String = (char*)realloc(m_String, m_Capacity * sizeof(char));
	}
	m_Length++;
	m_String[m_Length - 1] = new_char;
}


void NCharString::m_FindNextFunction()
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
				NCharString* sub_front = m_CutString(0, j - 1);
				NCharString* sub_back = m_CutString(i - j, i - 1);
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

int NCharString::m_FindSubString(NCharString* sub_string, int m_StartPlace)
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

void NCharString::m_OutPut(NCharString* filename)
{
	char real_file[1000] = { 0 };
	for (int i = 0; i < filename->m_Length; i++)
	{
		real_file[i] = filename->m_String[i];
	}
	fstream file(real_file, ios::app);
	for (int i = 0; i < m_Length; i++)
	{
		file << m_String[i];
	}
	file << endl;
}

void NCharString::m_PopBackChar()
{
	m_String[m_Length - 1] = 0;
	m_Length--;
}

NCharString* NCharString::m_Duplicate()
{
	NCharString* new_string = new NCharString();
	new_string->m_String = (char*)malloc(m_Capacity * sizeof(char));
	new_string->m_Length = m_Length;
	new_string->m_Capacity = m_Capacity;
	for (int i = 0; i < m_Length; i ++)
	{
		new_string->m_String[i] = m_String[i];
	}
	return new_string;
}