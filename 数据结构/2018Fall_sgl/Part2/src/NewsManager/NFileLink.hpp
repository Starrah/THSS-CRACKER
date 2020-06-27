/*
filename: NFileLink.hpp
description: the link of files
version: 1.0
date: 12/11/2018
*/

#ifndef NFILELINK_HPP
#define NFILELINK_HPP


#include <iostream>

#include "NChainLink.hpp"
#include "NCharString.h"

class NFileNode
{
public:
	NCharString * m_Title{ NULL };  //title of the article
	int m_Code{ 0 };   //code of the article
	int m_Times{ 0 };  //times it contains the certain word
	int m_WordTypes{ 1 }; //type of words it contains
public:
	//construction
	NFileNode(NCharString* new_title, int new_code)
	{
		m_Title = new_title->m_Duplicate();
		m_Code = new_code;
		m_Times = 1;
	}

	//destruction
	~NFileNode()
	{
		delete(m_Title);
	}

	NFileNode* m_Duplicate()
	{
		NFileNode* new_node = new NFileNode(m_Title, m_Code);
		new_node->m_Times = m_Times;
		return new_node;
	}


	//function: compare the times of its appearance
	//variable: another filenode
	//return: if more, return 1, same:0, less:return -1
	int m_Compare(NFileNode* another_node)
	{
		if (m_Times > another_node->m_Times)
		{
			return 1;
		}
		else if (m_Times < another_node->m_Times)
		{
			return -1;
		}
		else
		{
			if (m_WordTypes > another_node->m_WordTypes)
			{
				return 1;
			}
			else if (m_WordTypes < another_node->m_WordTypes)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}

	//function: judge whether the node's code is the same as the given one
	//variable: given code
	//return 1:yes 0:no
	bool m_JudgeWhetherSame(int given_code)
	{
		if (given_code == m_Code)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

};

class NFileLink
{
public:
	NCharString * m_Word{ NULL };
	NChainLink<NFileNode> m_FileList;
public:
	//construction
	NFileLink(NCharString* word_name)
	{
		m_Word = word_name->m_Duplicate();
	}
	//destruction
	~NFileLink()
	{
		m_FileList.m_Clear();
		delete(m_Word);
	}

	//function:compare the top words of the link and another, used in splay trees
	//variable: another link
	//return: <:-1, =:0,>:1
	int m_Compare(NFileLink* another_link)
	{
		return m_Word->m_Compare(another_link->m_Word);
	}

	//add a file to the link, at the tail
	//variable: new file's code and title
	void m_InsertNewFile(NCharString* new_title, int new_code)
	{
		if (m_FileList.m_Tail == m_FileList.m_Head || m_FileList.m_Tail->m_Data->m_JudgeWhetherSame(new_code) != 1)
		{
			NChainNode<NFileNode>* new_chain_node = new NChainNode<NFileNode>();
			new_chain_node->m_Data = new NFileNode(new_title, new_code);
			m_FileList.m_InsertAtPlace(m_FileList.m_Tail, new_chain_node);
		}
		else
		{
			m_FileList.m_Tail->m_Data->m_Times++;
		}
	}

	//adjust the link's tail
	void m_AdjustTail()
	{
		NChainNode<NFileNode>* tail = m_FileList.m_Tail;
		NChainNode<NFileNode>* last = m_FileList.m_Tail->m_Previous;
		if (last == m_FileList.m_Head || tail->m_Compare(last) < 1 )
		{
			return;
		}
		else
		{
			m_FileList.m_Tail = last;
			last->m_Next = NULL;
			NChainNode<NFileNode>* current = last->m_Previous;
			m_FileList.m_Length--;
			while (current != NULL)
			{
				if (current == m_FileList.m_Head || current->m_Compare(tail) >= 0 )
				{
					m_FileList.m_InsertAtPlace(current, tail);
					break;
				}
				current = current->m_Previous;
			}
		}
	}

	//duplicate one
	//return:duplicated one
	NFileLink* m_Duplicate()
	{
		NFileLink* new_link = new NFileLink(m_Word);
		NChainNode<NFileNode>* current = m_FileList.m_Head->m_Next;
		while (current != NULL)
		{
			NChainNode<NFileNode>* new_chain_node = new NChainNode<NFileNode>();
			new_chain_node->m_Data = current->m_Data->m_Duplicate();
			new_link->m_FileList.m_InsertAtPlace(new_link->m_FileList.m_Tail, new_chain_node);
			current = current->m_Next;
		}
		return new_link;
	}


	//add one filelink to this filelink
	//variable: new filelink
	void m_MergeFileLink(NFileLink* new_link)
	{
		NChainNode<NFileNode>* current = new_link->m_FileList.m_Head->m_Next;
		while (current != NULL)
		{
			NChainNode<NFileNode>* flag = m_FileList.m_Head->m_Next;
			bool whether_successful = 0;
			while (flag != NULL)
			{
				if (current->m_Data->m_JudgeWhetherSame(flag->m_Data->m_Code) == 1)
				{
					flag->m_Data->m_Times += current->m_Data->m_Times;
					flag->m_Data->m_WordTypes += current->m_Data->m_WordTypes;
					whether_successful = 1;
					break;
				}
				flag = flag->m_Next;
			}
			if (whether_successful == 0)
			{
				NFileNode* add_file = current->m_Data->m_Duplicate();
				NChainNode<NFileNode>* add_node = new NChainNode<NFileNode>(add_file);
				m_FileList.m_InsertAtPlace(m_FileList.m_Tail, add_node);
			}
			current = current->m_Next;
		}
	}

	//edit a file
	//variable: the old node you want to change, and the new node
	void m_EditFile(NChainNode<NFileNode>* old_node, NFileNode* new_node)
	{
		m_FileList.m_DeleteAtPlace(old_node);
		m_InsertNewFile(new_node->m_Title, new_node->m_Code);
		m_AdjustTail();
	}

	//delete a node
	void m_DeleteFile(NChainNode<NFileNode>* old_node)
	{
		m_FileList.m_DeleteAtPlace(old_node);
	}

	//search a node
	//variable:given node
	//return:pointer of the first node, if no such node, return NULL
	NChainNode<NFileNode>* m_SearchFile(NChainNode<NFileNode>* want_node)
	{
		return m_FileList.m_SearchEqual(want_node);
	}

};








#endif