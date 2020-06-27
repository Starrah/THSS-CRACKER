/*
filename: NChainLink.hpp
description: a chain link with template, the basis of stack and charlist
version: 1.0
date: 10/20/2018
*/

#ifndef NCHAINLINK_HPP
#define NCHAINLINK_HPP


#include <iostream>



template <class Data>
class NChainNode
{
	//data
public:
	Data * m_Data{ NULL };

	//pointer
public:
	NChainNode<Data>* m_Next{ NULL };
	NChainNode<Data>* m_Previous{ NULL };

	//function
public:

	//constructions and destructions
	NChainNode<Data>() {}
	NChainNode<Data>(Data* d) : m_Data(d) {}
	~NChainNode<Data>() 
	{
		if (m_Data != NULL)
		{
			delete(m_Data);
		}
	}

	//set the data
	void m_SetData(const Data* new_data)
	{
		m_Data = new_data;
	}

	//function:compare the node and another
	//variable: node new
	//return: -1: this<new ,0: this==new, 1: this>new
	int m_Compare(const NChainNode<Data>* new_node)
	{
		return m_Data->m_Compare(new_node->m_Data);
	}

	Data* m_GetData()
	{
		return m_Data;
	}
	//print the node
	/*void m_Print()
	{
	m_Data.m_Print();
	}*/
};



template <class Data>
class NChainLink
{
	//pointer
public:
	NChainNode<Data> *m_Head, *m_Tail, *m_This;
	int m_Length;


	//function of searching
public:

	//function:return the pointer of the node at certain place
	//variable:place
	//return:pointer
	//warning: if place is invalid, return NULL
	NChainNode<Data>* m_SearchAtPlace(const int place)
	{
		if (place <= 0 || place > m_Length)
		{
			return NULL;
		}
		m_This = m_Head;
		for (int i = 1; i <= place; i++)
		{
			m_This = m_This->m_Next;
		}
		return m_This;
	}

	//function:search for the first node that is identical to the given one
	//variable:given node
	//return:pointer of the first node, if no such node, return NULL
	NChainNode<Data>* m_SearchEqual(const NChainNode<Data>* required_node)
	{
		m_This = m_Head;
		while (m_This != NULL)
		{
			if (m_This != m_Head)
			{
				if (m_This->m_Compare(required_node) == 0)
				{
					NChainNode<Data>* new_this = new NChainNode<Data>();
					new_this->m_Data = m_This->m_Data->m_Duplicate();
					return new_this;

				}
			}
			m_This = m_This->m_Next;
		}
		return NULL;
	}




	//function of changing
public:
	//empty construction
	NChainLink()
	{
		m_Head = new NChainNode<Data>();
		m_Head->m_Next = NULL;
		m_This = m_Head;
		m_Tail = m_Head;
		m_Head->m_Previous = NULL;
		m_Length = 0;
	}

	//empty destruction
	~NChainLink(){}
	/*{
		if (m_Head == NULL)
		{
			return;
		}
		m_This = m_Head;
		NChainNode<Data>* previous = m_This;
		while (m_This->m_Next != NULL)
		{
			previous = m_This;
			m_This = m_This->m_Next;
			delete(previous);
		}
		delete(m_This);
	}*/
	

	//clear the link
	void m_Clear()
	{
		m_This = m_Head;
		NChainNode<Data>* previous = m_This;
		while (m_This->m_Next != NULL)
		{
			previous = m_This;
			m_This = m_This->m_Next;
			delete(previous);
		}
		delete(m_This);
		m_Head = NULL;
	}


	//function: insert a node at the back of one node
	//variable: the place you want, and the pointer of the new node 
	void m_InsertAtPlace(NChainNode<Data>* wanted_node, NChainNode<Data>* new_node)
	{
		NChainNode<Data>* old_next = wanted_node->m_Next;
		wanted_node->m_Next = new_node;
		new_node->m_Previous = wanted_node;
		new_node->m_Next = old_next;
		if (old_next == NULL)
		{
			m_Tail = new_node;
		}
		else
		{
			old_next->m_Previous = new_node;
		}
		m_Length++;
	}

	//function: delete a node at a certain place
	//variable: the node being deleted
	void m_DeleteAtPlace(NChainNode<Data>* wanted_node)
	{
		if (wanted_node == m_Head)
		{
			return;
		}
		NChainNode<Data>* old_next = wanted_node->m_Next;
		NChainNode<Data>* old_previous = wanted_node->m_Previous;
		old_previous->m_Next = old_next;
		if (old_next != NULL)
		{
			old_next->m_Previous = old_previous;
		}
		else
		{
			m_Tail = old_previous;
		}
		m_Length -= 1;
	}

	//sort the chainlink from biggest to smallest
	void m_Sort()
	{
		if (m_Head == m_Tail || m_Head->m_Next == m_Tail)
		{
			return;
		}
		NChainNode<Data>* node_i = m_Head->m_Next;
		NChainNode<Data>* node_j = node_i->m_Next;
		while (node_i!= m_Tail)
		{
			while (node_j != NULL)
			{
				if (node_i->m_Compare(node_j) == -1)
				{
					Data* temp = node_i->m_Data;
					node_i->m_Data = node_j->m_Data;
					node_j->m_Data = temp;
				}
				node_j = node_j->m_Next;
			}
			node_i = node_i->m_Next;
			node_j = node_i->m_Next;
		}
	}
	
};


#endif