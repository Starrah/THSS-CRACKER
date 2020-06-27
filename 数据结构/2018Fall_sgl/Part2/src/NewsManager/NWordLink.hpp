/*
filename: NWordLink.hpp
description: the link of words
version: 1.0
date: 12/15/2018
*/

#ifndef NWORDLINK_HPP
#define NWORDLINK_HPP


#include <iostream>
#include <cmath>


#include "NChainLink.hpp"
#include "NCharString.h"


class NWordNode
{
public:
	NCharString * m_Word{ NULL };  //title of the article
	int m_Times{ 0 };  //times it contains the certain word
public:
	//construction
	NWordNode(NCharString* new_word)
	{
		m_Word = new_word->m_Duplicate();
		m_Times = 1;
	}

	//destruction
	~NWordNode()
	{
		delete(m_Word);
	}

	NWordNode* m_Duplicate()
	{
		NWordNode* new_node = new NWordNode(m_Word);
		new_node->m_Times = m_Times;
		return new_node;
	}


	//function: compare the times of its appearance
	//variable: another filenode
	//return: if more, return 1, same:0, less:return -1
	int m_Compare(NWordNode* another_node)
	{
		return m_Word->m_Compare(another_node->m_Word);
	}

};

class NWordLink
{
public:
	NCharString * m_Title{ NULL };
	int m_Code{ 0 };
	NChainLink<NWordNode> m_WordList;
public:
	//construction
	NWordLink(NCharString* title_name, int title_code)
	{
		m_Title = title_name->m_Duplicate();
		m_Code = title_code;
	}

	//destruction
	~NWordLink()
	{
		m_WordList.m_Clear();
		delete(m_Title);
	}


	//add a file to the link, at the tail
	//variable: new file's code and title
	void m_InsertNewWord(NCharString* new_word)
	{
		NChainNode<NWordNode>* current = m_WordList.m_Head->m_Next;
		bool whether_have = 0;
		while (current != NULL)
		{
			int hint = current->m_Data->m_Word->m_Compare(new_word);
			if (hint == 0)
			{
				whether_have = 1;
				current->m_Data->m_Times++;
				break;
			}
			else if (hint == 1)
			{
				break;
			}
			current = current->m_Next;
		}
		if (whether_have == 0)
		{
			NWordNode* new_wordnode = new NWordNode(new_word);
			NChainNode<NWordNode>* new_node = new NChainNode<NWordNode>(new_wordnode);
			NChainNode<NWordNode>* insert_place;
			if (current == NULL)
			{
				insert_place = m_WordList.m_Tail;
			}
			else
			{
				insert_place = current->m_Previous;
			}
			m_WordList.m_InsertAtPlace(insert_place, new_node);
		}
	}

	

	//duplicate one
	//return:duplicated one
	NWordLink* m_Duplicate()
	{
		NWordLink* new_link = new NWordLink(m_Title,m_Code);
		NChainNode<NWordNode>* current = m_WordList.m_Head->m_Next;
		while (current != NULL)
		{
			NChainNode<NWordNode>* new_chain_node = new NChainNode<NWordNode>();
			new_chain_node->m_Data = current->m_Data->m_Duplicate();
			new_link->m_WordList.m_InsertAtPlace(new_link->m_WordList.m_Tail, new_chain_node);
			current = current->m_Next;
		}
		return new_link;
	}


	//get the jeccard value and cosine value of this wordlink and another
	//variable: another wordlink, jeccard value, cosine value
	void m_GetRelevance(NWordLink* new_link, double& jeccard, double& cosine)
	{
		int same_number = 0, total_number = 0;  //used to calculate jeccard number
		int same_calculation = 0, total_word_x = 0, total_word_y = 0;//used to calculate cosine number

		NChainNode<NWordNode>* current_x = m_WordList.m_Head->m_Next;
		NChainNode<NWordNode>* current_y = new_link->m_WordList.m_Head->m_Next;
		while (current_x != NULL || current_y != NULL)
		{
			if (current_x == NULL)
			{
				total_word_y += (current_y->m_Data->m_Times)*(current_y->m_Data->m_Times);
				current_y = current_y->m_Next;
			}
			else if (current_y == NULL)
			{
				total_word_x += (current_x->m_Data->m_Times)*(current_x->m_Data->m_Times);
				current_x = current_x->m_Next;
			}
			else
			{
				int hint = current_x->m_Compare(current_y);
				if (hint == 0)
				{
					//same
					same_number++;
					same_calculation += current_x->m_Data->m_Times*current_y->m_Data->m_Times;
					total_word_y += (current_y->m_Data->m_Times)*(current_y->m_Data->m_Times);
					total_word_x += (current_x->m_Data->m_Times)*(current_x->m_Data->m_Times);
					current_x = current_x->m_Next;
					current_y = current_y->m_Next;
				}
				else if (hint == 1)
				{
					//x > y, y ++
					total_word_y += (current_y->m_Data->m_Times)*(current_y->m_Data->m_Times);
					total_word_x += (current_x->m_Data->m_Times)*(current_x->m_Data->m_Times);
					current_y = current_y->m_Next;
				}
				else if (hint == -1)
				{
					//x < y, x ++
					total_word_y += (current_y->m_Data->m_Times)*(current_y->m_Data->m_Times);
					total_word_x += (current_x->m_Data->m_Times)*(current_x->m_Data->m_Times);
					current_x = current_x->m_Next;
				}
			}
			
		}
		total_number = m_WordList.m_Length+new_link->m_WordList.m_Length - same_number;
		jeccard = double(same_number) / double(total_number);
		cosine = double(same_calculation) / sqrt(double(total_word_x)) / sqrt(double(total_word_y));
	}
};








#endif