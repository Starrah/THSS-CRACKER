/*
filename: NDictionary.hpp
description: dictionary of words
version: 1.0
date: 11/1/2018
*/


#ifndef NDICTIONARY_HPP
#define NDICTIONARY_HPP

#include <iostream>

#include "NChainLink.hpp"
#include "NCharString.h"
#include "NWCharString.h"
#include "NCharStringLink.hpp"
#include "NHash.hpp"

class NDictionary
{
//data
public:
	NCharStringLink m_Dictionary;
	NHash* m_HashList{ NULL };
	int m_MaxLength{ 0 };

//function
	NDictionary(char filename[])
	{
		ifstream file(filename, ios::in);
		if (!file.is_open())
		{
			return;
		}
		NCharString* the_str = new NCharString();
		int i = -1;
		int flag = -1;
		while (!file.eof())
		{
			char c = file.get();
			if (c >= 0 && c <= 127)
			{
				if (flag != -1)
				{

					NCharString* new_word = the_str->m_CutString(flag, i);
					if (i - flag + 1 > m_MaxLength)
					{
						m_MaxLength = i - flag + 1;
					}
					m_Dictionary.m_PushBack(new_word);
					flag = -1;
				}
			}
			else
			{
				if (flag == -1)
				{
					flag = i + 1;
				}
			}
			the_str->m_PushBackChar(c);
			i++;

		}
		m_HashList = new NHash(&m_Dictionary);
	}
};






#endif