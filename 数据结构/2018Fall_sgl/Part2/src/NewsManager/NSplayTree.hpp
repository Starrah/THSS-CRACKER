
/*
filename: NSplayTree.hpp
description: a splay tree
date: 12/2/2018
*/


#ifndef NSPLAYTREE_HPP
#define NSPLAYTREE_HPP

#include <iostream>
#include <cmath>

#include "NBinaryTree.hpp"
#include "NBinarySortTree.hpp"

template <class Data>
class NSplayTree : public NBinarySortTree<Data>
{
public:
	//empty construction and destruction
	NSplayTree(){}

	//function: adjust a binary tree
	//recursion function, variable: the tree
	NSplayTree(NBinaryTree<Data>* tree)
	{
		m_Adjust(tree->m_Root);
	}
	~NSplayTree(){}

	//construct from a binary sort tree


	//used in adding a new node
	//function:inserting a new node
	//variable: new node
	void m_InsertNewNode(NBinaryNode<Data>* new_node)
	{
		NBinaryNode<Data>* insert_place = this->m_FindNode(new_node);
		int m = 0;
		if (insert_place == NULL)
		{
			this->m_Root = new_node;
			return;
		}
		else
		{ 
			m = new_node->m_Compare(insert_place);
		}
		if (m == 0)
		{
			//no need to insert
			return;
		}
		else if(insert_place->m_LeftSon!=NULL||insert_place->m_RightSon!=NULL)
		{
			//directly insert
			if (m == -1)
			{
				this->m_AddNode(new_node, insert_place, 0);
			}
			else
			{
				this->m_AddNode(new_node, insert_place, 1);
			}
		}
		else
		{
			bool mode_insert;
			bool mode_subtree;
			if (m == -1)
			{
				mode_insert = 0;
			}
			else
			{
				mode_insert = 1;
			}
			//search for minimum bad tree
			NBinaryNode<Data>* minimum_bad = NULL;
			m_SearchMinimumBad(insert_place, mode_insert, minimum_bad, mode_subtree);
			if (minimum_bad == NULL)
			{
				//not bad
				this->m_AddNode(new_node, insert_place, mode_insert);
			}
			//bad
			else
			{
				bool mode_subsub = 0;
				NBinaryNode<Data>* son_node;
				if (mode_subtree == 1)
				{
					son_node = minimum_bad->m_RightSon;
				}
				else
				{
					son_node = minimum_bad->m_LeftSon;
				}
				if (new_node->m_Compare(son_node) == 1)
				{
					mode_subsub = 1;
				}
				else
				{
					mode_subsub = 0;
				}

				this->m_AddNode(new_node, insert_place, mode_insert);
				//left-left
				if (mode_subtree == 0 && mode_subsub == 0)
				{
					this->m_RotateRight(minimum_bad);
				}
				//left-right
				if (mode_subtree == 0 && mode_subsub == 1)
				{
					this->m_RotateLeft(minimum_bad->m_LeftSon);
					this->m_RotateRight(minimum_bad);
				}

				//right-left
				if (mode_subtree == 1 && mode_subsub == 0)
				{
					this->m_RotateRight(minimum_bad->m_RightSon);
					this->m_RotateLeft(minimum_bad);
				}

				//right-right
				if (mode_subtree == 1 && mode_subsub == 1)
				{
					this->m_RotateLeft(minimum_bad);
				}
			}
		}
	}
	
	//function:search the minimum bad node, also renew height
	//variable: insert place, minimum bad, mode(0:left, 1:right)
	void m_SearchMinimumBad(NBinaryNode<Data>* insert_place, bool mode_insert, NBinaryNode<Data>*& minimum_bad, bool& mode_subtree)
	{
		NBinaryNode<Data>* current = insert_place->m_Father;
		bool mode = insert_place->m_Mode;
		insert_place->m_Height = 2;
		while (current != NULL)
		{
			int height_left = 0, height_right = 0;
			if (current->m_LeftSon != NULL)
			{
				height_left = current->m_LeftSon->m_Height;
			}
			if (current->m_RightSon != NULL)
			{
				height_right = current->m_RightSon->m_Height;
			}
			current->m_Height = fmax(height_left + 1, height_right + 1);
			if (abs(height_left - height_right) >= 2)
			{
				//bad
				minimum_bad = current;
				mode_subtree = mode;
				return;
			}
			else
			{
				//no bad
				mode = current->m_Mode;
				current = current->m_Father;
			}
		}
	}

	//2 functions of rotating
	//variable:the father of the tree
	void m_RotateRight(NBinaryNode<Data>* father)
	{
		NBinaryNode<Data>* leftson = father->m_LeftSon;
		if (father->m_Father != NULL)
		{
			if (father->m_Mode == 0)
			{
				father->m_Father->m_LeftSon = leftson;
				leftson->m_Father = father->m_Father;
				leftson->m_Mode = 0;
			}
			else
			{
				father->m_Father->m_RightSon = leftson;
				leftson->m_Father = father->m_Father;
				leftson->m_Mode = 1;
			}
		}
		else
		{
			this->m_Root = leftson;
			leftson->m_Father = NULL;
		}
		father->m_LeftSon = leftson->m_RightSon;
		if (leftson->m_RightSon != NULL)
		{
			leftson->m_RightSon->m_Father = father;
			leftson->m_RightSon->m_Mode = 0;
		}
		father->m_Father = leftson;
		father->m_Mode = 1;
		leftson->m_RightSon = father;
		father->m_RenewHeight();
		leftson->m_RenewHeight();
	}

	void m_RotateLeft(NBinaryNode<Data>* father)
	{
		NBinaryNode<Data>* rightson = father->m_RightSon;
		if (father->m_Father != NULL)
		{
			if (father->m_Mode == 0)
			{
				father->m_Father->m_LeftSon = rightson;
				rightson->m_Father = father->m_Father;
				rightson->m_Mode = 0;
			}
			else
			{
				father->m_Father->m_RightSon = rightson;
				rightson->m_Father = father->m_Father;
				rightson->m_Mode = 1;
			}
		}
		else
		{
			this->m_Root = rightson;
			rightson->m_Father = NULL;
		}
		father->m_RightSon = rightson->m_LeftSon;
		if (rightson->m_LeftSon != NULL)
		{
			rightson->m_LeftSon->m_Father = father;
			rightson->m_LeftSon->m_Mode = 1;
		}
		father->m_Father = rightson;
		father->m_Mode = 0;
		rightson->m_LeftSon = father;
		father->m_RenewHeight();
		rightson->m_RenewHeight();
	}

	//function: used in adjusting a tree
	//variable: current node
	//recursion function
	void m_Adjust(NBinaryNode<Data>* current_node)
	{
		if (current_node != NULL)
		{
			NBinaryNode<Data>* new_data = new NBinaryNode<Data>(current_node->m_Data->m_Duplicate());
			m_InsertNewNode(new_data);
			if (current_node->m_LeftSon != NULL)
			{
				m_Adjust(current_node->m_LeftSon);
			}
			if (current_node->m_RightSon != NULL)
			{
				m_Adjust(current_node->m_RightSon);
			}
		}
	}


};

#endif