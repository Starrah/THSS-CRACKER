#pragma once
/*
filename: NReversedWordList.hpp
description: reversed word list
version: 1.0
date: 12/11/2018
*/


#ifndef NREVERSEDWORDLIST_HPP
#define NREVERSEDWORDLIST_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <Windows.h>

#include "GGlobalFunctions.h"
#include "NFileLink.hpp"
#include "NWordLink.hpp"
#include "NSplayTree.hpp"
#include "NDictionary.hpp"



class NReversedWordList:public NSplayTree<NFileLink>
{
public:
	//construction
	NReversedWordList(NDictionary* the_dictionary)
	{
		NChainNode<NCharString>* current = the_dictionary->m_Dictionary.m_Head->m_Next;
		while (current != NULL)
		{
			
			NFileLink* word_link = new NFileLink(current->m_Data);
			NBinaryNode<NFileLink>* word_node = new NBinaryNode<NFileLink>(word_link);
			this->m_InsertNewNode(word_node);
			current = current->m_Next;
		}
	}
	


	//handle a file
	//variable: the out and the wordlist filename of the file
	//return: the right wordlink node of this file
	NWordLink* m_AddOneFile(NCharString* file_out_name, NCharString* file_wordlist)
	{
		//cin the two files and get the information you want
		char real_file_out[100] = { 0 };
		for (int i = 0; i < file_out_name->m_Length; i++)
		{
			real_file_out[i] = file_out_name->m_String[i];
		}
		std::ifstream file_out(real_file_out, std::ios::in);
		if (!file_out.is_open())
		{
			return NULL;
		}
		char real_file_word[100] = { 0 };
		for (int i = 0; i < file_wordlist->m_Length; i++)
		{
			real_file_word[i] = file_wordlist->m_String[i];
		}
		std::ifstream file_word(real_file_word, std::ios::in);
		if (!file_word.is_open())
		{
			return NULL;
		}
		char title[1100] = { 0 };
		file_out.getline(title,1000);
		NCharString* title_article = new NCharString(title, strlen(title));
		int title_num = g_SwitchCharToInt(&real_file_out[7], file_out_name->m_Length - 12);

		NFileLink* current_used_files[5000] = {NULL};
		int usenum = 0;

		NWordLink* current_word_link = new NWordLink(title_article, title_num);
		while (1)
		{
			char word[40] = { 0 };
			file_word >> word;
			if (word[0] == 0)
			{
				break;
			}
			//find insert place
			
			NCharString* word_string = new NCharString(word, strlen(word));
			current_word_link->m_InsertNewWord(word_string);
			NFileLink* new_word_link = new NFileLink(word_string);
			NBinaryNode<NFileLink>* new_word_node = new NBinaryNode<NFileLink>(new_word_link);
			NBinaryNode<NFileLink>* find_link = this->m_FindNode(new_word_node);
			find_link->m_GetData(1)->m_InsertNewFile(title_article, title_num);
			NFileLink* real_link = find_link->m_GetData(1);
			usenum++;
			current_used_files[usenum] = real_link;
			delete(new_word_node);
		}
		for (int i = 1; i <= usenum; i++)
		{
			current_used_files[i]->m_AdjustTail();
		}
		return current_word_link;
	}

	//handle word search	

	//function: handle search by words
	void m_HandleWordSearch(NDictionary* the_dictionary)
	{
		//open files
		std::ifstream file_in("query1.txt", std::ios::in);
		if (!file_in.is_open())
		{
			return;
		}
		ofstream file_out("result1.txt");

		NCharStringLink* the_link = NULL;
		while (1)
		{
			the_link = new NCharStringLink();
			char line[3000];
			if (!file_in.getline(line, 3000))
			{
				break;
			}
			NCharString* the_line = new NCharString(line, strlen(line));
				int max_length = the_dictionary->m_MaxLength;
				int i = 0;
				while (i < the_line ->m_Length - 1)
				{
					if (the_line->m_String[i] >= 0 && the_line->m_String[i] <= 127)
					{
						i++;
						continue;
					}
					bool whether_success = 0;
					int length = max_length;
					NCharString* current_string = the_line->m_CutString(i, min(i + length - 1, the_line->m_Length - 1));
					while (length > 0)
					{

						if (i + length - 1 < the_line->m_Length)
						{
							NCharString* ans_string = the_dictionary->m_HashList->m_SearchList(current_string);
							if (ans_string != NULL)
							{
								the_link->m_PushBack(ans_string);
								i += length;
								whether_success = 1;
								break;
							}
							else
							{
								if (current_string->m_String[length - 1] >= 0 && current_string->m_String[length - 1] <= 127)
								{
									length -= 1;
									current_string->m_PopBackChar();

								}
								else
								{
									length -= 2;
									current_string->m_PopBackChar();
									current_string->m_PopBackChar();
									if (current_string->m_Length <= 0)
									{
										current_string->m_Length = 0;
									}

								}
							}

						}
						else
						{
							length = the_line->m_Length - i;
						}
					}
					delete(current_string);

					if (whether_success == 0)
					{
						i += 2;
					}
				}
				delete(the_line);
			m_HandleWordSearchByLine(the_link);
			delete(the_link);
			the_link = NULL;
		}
	}

	//handle word search by line
	//variable: the wordlist
	void m_HandleWordSearchByLine(NCharStringLink* searched_link)
	{
		fstream file("result1.txt", ios::app);
		NFileLink* the_filelink = NULL;
		NChainNode<NCharString>* present = searched_link->m_Head->m_Next;
		while (present != NULL)
		{
			NFileLink* new_file = new NFileLink(present->m_Data);
			NBinaryNode<NFileLink>* new_node = new NBinaryNode<NFileLink>(new_file);
			NBinaryNode<NFileLink>* find_place = this->m_FindNode(new_node);
			if (find_place->m_Compare(new_node) == 0)
			{
				if (the_filelink == NULL)
				{
					the_filelink = find_place->m_GetData(1)->m_Duplicate();
				}
				else
				{
					the_filelink->m_MergeFileLink(find_place->m_GetData(1));
				}
			}
			delete(new_node);
			present = present->m_Next;
		}
		if (the_filelink == NULL || the_filelink->m_FileList.m_Length == 0)
		{
			file << "no news can be found" <<endl;
			return;
		}
		the_filelink->m_FileList.m_Sort();
		NChainNode<NFileNode>* out_current = the_filelink->m_FileList.m_Head->m_Next;
		while (out_current != NULL)
		{
			file << "(" << out_current->m_Data->m_Code << "," << out_current->m_Data->m_Times << ") ";
			out_current = out_current->m_Next;
		}
		file << "\n";
		delete(the_filelink);
	}



};
#endif