/*
filename: NHash.hpp
description: the class of a hash
version: 1.0
date: 11/1/2018
*/


#ifndef NHASH_H
#define NHASH_H

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "NWCharString.h"
#include "NCharString.h"
#include "NCharStringLink.hpp"

#define MAX_NUM 290057
#define MULTIPLE_NUM 131

class NHash
{
//data
public:
	NCharStringLink * m_HashList[MAX_NUM]{ NULL };

//function
	NHash(NCharStringLink* the_link)
	{
		NChainNode<NCharString>* node_this = the_link->m_Head;
		while (node_this != NULL)
		{
			if (node_this != the_link->m_Head)
			{
				int hash_num = 0;
				for (int i = 0; i < node_this->m_Data->m_Length; i++)
				{
					int c = node_this->m_Data->m_String[i];
					hash_num = (abs(hash_num) * MULTIPLE_NUM + abs(c)) % MAX_NUM;
				}
				if (m_HashList[hash_num] == NULL)
				{
					m_HashList[hash_num] = new NCharStringLink();
				}
				m_HashList[hash_num]->m_PushBack(node_this->m_Data);
			}
			node_this = node_this->m_Next;
		}
	}
	
	NCharString* m_SearchList(NCharString* wanted_char)
	{
		int hash_num = 0;
		for (int i = 0; i < wanted_char->m_Length; i++)
		{
			int c = wanted_char->m_String[i];
			hash_num = (abs(hash_num) * MULTIPLE_NUM + abs(c)) % MAX_NUM;
		}
		if (m_HashList[hash_num] == NULL)
		{
			return NULL;
		}
		else
		{
			NChainNode<NCharString>* wanted_node = new NChainNode<NCharString>(wanted_char);
			NChainNode<NCharString>* ans_node = m_HashList[hash_num]->m_SearchEqual(wanted_node);
			if (ans_node != NULL)
			{
				return ans_node->m_Data;
			}
			else
			{
				return NULL;
			}
		}
	}

};

















#endif