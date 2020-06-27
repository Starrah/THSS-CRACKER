#pragma once
/*
filename: NStack.hpp
description: stack
version: 1.0
date: 10/20/2018
*/

#ifndef NSTACK_HPP
#define NSTACK_HPP


#include <iostream>

#include "NChainLink.hpp"
#include "NCharString.h"

class NStack
{
	//data
public:
	NChainLink<NCharString> m_Tag_Stack;
	NChainLink<NCharString> m_Content_Stack;

	//function
public:
	void m_PushBack(NCharString* new_tag, NCharString* new_content)
	{
		NChainNode<NCharString>* new_tag_node = new NChainNode<NCharString>(new_tag);
		NChainNode<NCharString>* new_content_node = new NChainNode<NCharString>(new_content);
		m_Tag_Stack.m_InsertAtPlace(m_Tag_Stack.m_Tail, new_tag_node);
		m_Content_Stack.m_InsertAtPlace(m_Content_Stack.m_Tail, new_content_node);
	}

	void m_PopBack()
	{
		m_Tag_Stack.m_DeleteAtPlace(m_Tag_Stack.m_Tail);
		m_Content_Stack.m_DeleteAtPlace(m_Content_Stack.m_Tail);
	}

	//empty:1 no:0
	bool m_JudgeEmpty()
	{
		if (m_Tag_Stack.m_Length == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	//clear the stack
	void m_Clear()
	{
		for (int i = 1; i <= m_Tag_Stack.m_Length; i++)
		{
			m_PopBack();
		}
	}

	//return: the element tag at top, if empty, return NULL
	NCharString* m_GetTopTag()
	{
		if (m_JudgeEmpty())
		{
			return NULL;
		}
		return m_Tag_Stack.m_Tail->m_GetData();
	}

	//return: the element content at top, if empty, return NULL
	NCharString* m_GetTopContent()
	{
		if (m_JudgeEmpty())
		{
			return NULL;
		}
		return m_Content_Stack.m_Tail->m_GetData();
	}
};










#endif