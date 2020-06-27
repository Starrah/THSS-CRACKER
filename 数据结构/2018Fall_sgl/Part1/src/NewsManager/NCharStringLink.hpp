/*
filename: NCharStringLink.hpp
description: a chain link of string
version: 1.0
date: 10/20/2018
*/

#ifndef NCHARSTRINGLINK_HPP
#define NCHARSTRINGLINK_HPP


#include <iostream>

#include "NChainLink.hpp"
#include "NCharString.h"
#include "NWCharString.h"

class NCharStringLink : public NChainLink<NCharString>
{
public:
	void m_PushBack(NCharString* new_string)
	{
		NChainNode<NCharString>* new_node = new NChainNode<NCharString>(new_string);
		m_InsertAtPlace(m_Tail, new_node);
	}

	//function:search for the first node whose length <= the given one
	//variable:given node
	//return:pointer of the first node, if no such node, return NULL
	NChainNode<NCharString>* m_SearchByLength(const NCharString* required_node)
	{
		m_This = m_Head;
		while (m_This != NULL)
		{
			if (m_This != m_Head)
			{
				if (m_This->m_Data->m_Length <= required_node->m_Length)
				{
					return m_This;
				}
			}
			m_This = m_This->m_Next;
		}
		return NULL;
	}


	void m_InsertByLength(NCharString* new_string)
	{
		NChainNode<NCharString>* the_node = m_SearchByLength(new_string);
		NChainNode<NCharString>* wanted_node;
		if (the_node != NULL)
		{
			wanted_node = the_node->m_Previous;
		}
		else
		{
			wanted_node = m_Tail;
		}
		NChainNode<NCharString>* new_node = new NChainNode<NCharString>(new_string);
		m_InsertAtPlace(wanted_node, new_node);
	}
};




class NWCharStringLink : public NChainLink<NWCharString>
{
public:
	void m_PushBack(NWCharString* new_string)
	{
		NChainNode<NWCharString>* new_node = new NChainNode<NWCharString>(new_string);
		m_InsertAtPlace(m_Tail, new_node);
	}

	//function:search for the first node whose length <= the given one
	//variable:given node
	//return:pointer of the first node, if no such node, return NULL
	NChainNode<NWCharString>* m_SearchByLength(const NWCharString* required_node)
	{
		m_This = m_Head;
		while (m_This != NULL)
		{
			if (m_This != m_Head)
			{
				if (m_This->m_Data->m_Length <= required_node->m_Length)
				{
					return m_This;
				}
			}
			m_This = m_This->m_Next;
		}
		return NULL;
	}


	void m_InsertByLength(NWCharString* new_string)
	{
		NChainNode<NWCharString>* the_node = m_SearchByLength(new_string);
		NChainNode<NWCharString>* wanted_node;
		if (the_node != NULL)
		{
			wanted_node = the_node->m_Previous;
		}
		else
		{
			wanted_node = m_Tail;
		}
		NChainNode<NWCharString>* new_node = new NChainNode<NWCharString>(new_string);
		m_InsertAtPlace(wanted_node, new_node);
	}
};








#endif