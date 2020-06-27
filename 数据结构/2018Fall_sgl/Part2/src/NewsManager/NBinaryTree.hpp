
/*
filename: NBinaryTree.hpp
description: a binary tree
date: 12/2/2018
*/


#ifndef NBINARYTREE_HPP
#define NBINARYTREE_HPP

#include <iostream>



template<class Data>
class NBinaryNode
{
	//data
protected:
	//Data m_Data;    //used for int, char, string, etc
	Data* m_DataPointer{ NULL }; //used for self-designed things, such as char
public:
	NBinaryNode<Data>* m_LeftSon{ NULL };
	NBinaryNode<Data>* m_RightSon{ NULL };
	NBinaryNode<Data>* m_Father{ NULL };
	bool m_Mode{ 0 };//0:left, 1:right
	int m_Height{ 1 };

	//function
public:
	//construction
	//NBinaryNode(Data d) :m_Data(d) {}
	NBinaryNode(Data* d) 
	{
		m_DataPointer = d;
	}
	~NBinaryNode()
	{
		delete(m_DataPointer);
	}

	//function: compare two nodes, in real data
	//variable: new node
	//return: -1:this<new 0:this=new 1:this>new
	int m_Compare(NBinaryNode<Data>* new_node)
	{
		if (m_DataPointer != NULL)
		{
			return m_DataPointer -> m_Compare(new_node -> m_DataPointer);
		}
		/*else
		{
			if (m_Data < new_node->m_Data)
			{
				return -1;
			}
			if (m_Data == new_node->m_Data)
			{
				return 0;
			}
			if (m_Data > new_node->m_Data)
			{
				return 1;
			}
		}*/
	}

	//return the data
	/*Data m_GetData()
	{
		return m_Data;
	}*/
	Data* m_GetData(int a)
	{
		return m_DataPointer;
	}
	//copy the data from another node
	void m_CopyData(NBinaryNode<Data>* new_node)
	{
		if (m_DataPointer != NULL)
		{
			m_DataPointer = new_node->m_DataPointer;
		}
		/*else
		{
			m_Data = new_node->m_Data;
		}*/
	}

	//output the data
	void m_OutPut()
	{
		if (m_DataPointer != NULL)
		{
			// m_DataPointer -> m_OutPut();
		}
		/*else
		{
			std::cout << m_Data;
		}*/
	}

	//function:renew the height of itself
	void m_RenewHeight()
	{
		int left_height = 0, right_height = 0;
		if (m_LeftSon != NULL)
		{
			left_height = m_LeftSon->m_Height;
		}
		if (m_RightSon != NULL)
		{
			right_height = m_RightSon->m_Height;
		}
		m_Height = fmax(left_height + 1, right_height + 1);
	}
};


template <class Data>
class NBinaryTree
{
	//pointer
public:
	NBinaryNode<Data>* m_Root{ NULL };
	int m_Height{ 0 };

	//function of changing
public:
	//empty construction
	NBinaryTree(){}



	//recursing destruction
	void m_DeleteTree(NBinaryNode<Data>* current_place)
	{
		if (current_place == NULL)
		{
			return;
		}
		if (current_place->m_LeftSon != NULL)
		{
			m_DeleteTree(current_place->m_LeftSon);
		}
		if (current_place->m_RightSon != NULL)
		{
			m_DeleteTree(current_place->m_RightSon);
		}
		delete(current_place);
	}
	~NBinaryTree()
	{
		if (m_Root != NULL)
		{
			m_DeleteTree(m_Root);
		}
	}



	//function:add one new node to the tree
	//variable:the new node, the place you want to insert, mode(0:left, 1:right)
	void m_AddNode(NBinaryNode<Data>* new_son, NBinaryNode<Data>* add_place, bool mode)
	{
		if (mode == 0)
		{
			new_son->m_Father = add_place;
			add_place->m_LeftSon = new_son;
			new_son->m_Mode = 0;
		}
		else
		{
			new_son->m_Father = add_place;
			add_place->m_RightSon = new_son;
			new_son->m_Mode = 1;

		}
	}

	//function: find the Left_down place of the subtree
	NBinaryNode<Data>* m_FindLeftEnd(NBinaryNode<Data>* start_place)
	{
		NBinaryNode<Data>* current = start_place;
		while (current->m_LeftSon != NULL)
		{
			current = current->m_LeftSon;
		}
		return current;
	}

	//function: print the link
	void m_OutPut(NBinaryNode<Data>* current)
	{
		if (current == NULL)
		{
			return;
		}
		else
		{
			current->m_OutPut();
			if (current->m_LeftSon == NULL && current->m_RightSon == NULL)
			{
				return;
			}
			else
			{
				std::cout << "(";
				if (current->m_LeftSon == NULL)
				{
					std::cout << "-";
				}
				else
				{
					m_OutPut(current->m_LeftSon);
				}
				std::cout << ",";
				if (current->m_RightSon == NULL)
				{
					std::cout << "-";
				}
				else
				{
					m_OutPut(current->m_RightSon);
				}
				std::cout << ")";
			}
		}
	}

	//recursion function,used in getting height
	void m_GetHeight(NBinaryNode<Data>* current_node)
	{
		int left_height = 0, right_height = 0;
		if (current_node->m_LeftSon != NULL)
		{
			m_GetHeight(current_node->m_LeftSon);
			left_height = current_node->m_LeftSon->m_Height;
		}
		if (current_node->m_RightSon != NULL)
		{
			m_GetHeight(current_node->m_RightSon);
			right_height = current_node->m_RightSon->m_Height;
		}
		current_node->m_Height = max(left_height + 1, right_height + 1);
	}
};




#endif