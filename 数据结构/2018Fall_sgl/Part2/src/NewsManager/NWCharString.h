/*
filename: NWCharSting.h
description: a Wchar string
version: 1.0
date: 10/20/2018
*/

#ifndef NWCHARSTRING_H
#define NWCHARSTRING_H

#include <iostream>
#include <malloc.h>

#include "NCharString.h"

class NNews;
class NCharString;
class NHash;
class NWCharString
{
	friend class NNews;
	friend class NCharString;
	friend class NHash;

	//data
protected:
	wchar_t* m_String{ NULL };
	const int c_ReallocNumber{ 20 };
	int* m_Next{ NULL };

public:
	int m_Length{ 0 };
	int m_Capacity{ 0 };
	//function 
public:
	//construction and destruction
	NWCharString() {}
	NWCharString(const wchar_t c[], const int length);
	NWCharString(NCharString* old_string);
	~NWCharString()
	{
		if (m_String != NULL)
		{
			free(m_String);

		}
		if (m_Next != NULL)
		{
			free(m_Next);
		}
	}

	//function:output string in a file
	//variable:filename
	void m_OutPut(char filename[]);

	//function: Cut String
	//variable: start and end
	//return: new string
	//warning: if start and end is invalid, return NULL
	NWCharString* m_CutString(const int start, const int end);

	//function: set value
	//variable: char of charstring you want
	void m_SetValue(const wchar_t c[], const int length);
	void m_SetValue(const NWCharString* new_string);

	//function: append a charstring after it
	//variable: the charstring you want to append
	void m_PushBackString(const NWCharString* new_string);

	//function: append a char after it
	//variable: the char you want to append
	void m_PushBackChar(const char new_char);

	//function: pop back a char after it
	void m_PopBackChar();


	//function: append a charstring before it
	//variable: the charstring you want to append
	void m_InsertBeforeString(const NWCharString* new_string);

	//function: compare two charstring
	//variable: the one compared with this
	//return :-1: this < new, 0: this = new, 1: this > new
	int m_Compare(const NWCharString* new_string);

	//find next[j], which is useful in kmp
	void m_FindNextFunction();

	//function: find the first place of a substring behind the startplace
	//variable:the substring, and the startplace
	//return: the first place, if no, return -1
	int m_FindSubString(NWCharString* sub_string, int m_StartPlace);

	//friend function
	//function: concat two strings
	//variable: two string
	//return: new string
	friend NWCharString* f_ConCat(const NWCharString* first_string, const NWCharString* second_string);
};
extern NWCharString* f_ConCat(const NWCharString* first_string, const NWCharString* second_string);

#endif
