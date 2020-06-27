/*
filename: NCharSting.h
description: a string
version: 1.0
date: 10/20/2018
*/

#ifndef NCHARSTRING_H
#define NCHARSTRING_H

#include <iostream>
#include <malloc.h>

class NCharString
{

	//data
public:
	char* m_String{ NULL };
	const int c_ReallocNumber{ 20 };
	int* m_Next{ NULL };

public:
	int m_Length{ 0 };
	int m_Capacity{ 0 };
	//function 
public:
	//construction and destruction
	NCharString() {}
	NCharString(const char c[], const int length);
	~NCharString()
	{
		if (m_String != NULL)
		{
			char* old_string = m_String;
			m_String = NULL;
			free(old_string);
		}
		if (m_Next != NULL)
		{
			int* old_string = m_Next;
			m_Next = NULL;
			free(old_string);
		}
	}

	//function:output string in a file
	//variable:filename
	void m_OutPut(NCharString* filename);

	//function: Cut String
	//variable: start and end
	//return: new string
	//warning: if start and end is invalid, return NULL
	NCharString* m_CutString(const int start, const int end);

	//function: set value
	//variable: char of charstring you want
	void m_SetValue(const char c[], const int length);
	void m_SetValue(const NCharString* new_string);

	//function: append a charstring after it
	//variable: the charstring you want to append
	void m_PushBackString(const NCharString* new_string);

	//function: pop back a char after it
	void m_PopBackChar();

	//function: Duplicate the char
	//return: new char
	NCharString* m_Duplicate();

	//function: append a char after it
	//variable: the char you want to append
	void m_PushBackChar(const char new_char);

	//function: append a charstring before it
	//variable: the charstring you want to append
	void m_InsertBeforeString(const NCharString* new_string);

	//function: compare two charstring
	//variable: the one compared with this
	//return :-1: this < new, 0: this = new, 1: this > new
	int m_Compare(const NCharString* new_string);

	//find next[j], which is useful in kmp
	void m_FindNextFunction();

	//function: find the first place of a substring behind the startplace
	//variable:the substring, and the startplace
	//return: the first place, if no, return -1
	int m_FindSubString(NCharString* sub_string, int m_StartPlace);

	//friend function
	//function: concat two strings
	//variable: two string
	//return: new string
	friend NCharString* f_ConCat(const NCharString* first_string, const NCharString* second_string);
};
extern NCharString* f_ConCat(const NCharString* first_string, const NCharString* second_string);

#endif
