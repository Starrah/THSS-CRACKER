#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>


using namespace std;

#include "GGlobalFunctions.h"
#include "NCharString.h"
#include "NChainLink.hpp"
#include "NCharStringLink.hpp"
#include "NNews.h"
#include "NDictionary.hpp"
#include "NFindFiles.hpp"
#include "NReversedWordList.hpp"


const double minimum_relevance = 0;
const double minimum_same = 0.99;
double relevance_size[1000][1000] = { 0 };
NWordLink* word_links[1000] = { NULL };
NWordLink* title_links[1000] = { NULL };

const int file_num = 780;

//construction a title link through dividing a the title itself
void g_BuildTitleLink(NWordLink* word_link, NDictionary* dictionary)
{
	NCharString* title_name = word_link->m_Title;
	int max_length = dictionary->m_MaxLength;
	int i = 0;
	while (i < title_name->m_Length - 1)
	{
		if (title_name->m_String[i] >= 0 && title_name->m_String[i] <= 127)
		{
			i++;
			continue;
		}
		bool whether_success = 0;
		int length = max_length;
		NCharString* current_string = title_name->m_CutString(i, min(i + length - 1, title_name->m_Length - 1));
		while (length > 0)
		{

			if (i + length - 1 < title_name->m_Length)
			{
				NCharString* ans_string = dictionary->m_HashList->m_SearchList(current_string);
				if (ans_string != NULL)
				{
					word_link->m_InsertNewWord(ans_string);
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
				length = title_name->m_Length - i;
			}
		}
		delete(current_string);

		if (whether_success == 0)
		{
			i += 2;
		}
	}
}

//cout the five recommended news list
//if no,cout "no recommended"

void g_Recommend(int wanted)
{
	ofstream file("result2.txt", ios::app);
	int max_list[10] = { 0,-1,-1,-1,-1,-1 };
	double num_list[10] = { 0,minimum_relevance,minimum_relevance,minimum_relevance,minimum_relevance,minimum_relevance };
	for (int i = 0; i <= file_num; i++)
	{
		if (i == wanted)
		{
			continue;
		}
		for (int j = 1; j <= 5; j++)
		{
			if (relevance_size[wanted][i] >= num_list[j])
			{
				g_MoveRightAList(&num_list[j], 5 - j + 1, relevance_size[wanted][i]);
				g_MoveRightAList(&max_list[j], 5 - j + 1, i);
				break;
			}
		}
	}
	for (int i = 1; i <= 5; i++)
	{
		if (max_list[i] != -1)
		{
			if (i != 1)
			{
				file << ",";
			}
			file << "(" << word_links[max_list[i]]->m_Code << ","<< word_links[max_list[i]]->m_Title->m_String<<")";
		}
	}
	file << "\n";
	file.close();
}

//used in generating the recommendation file, used in QT
void g_Recommend_QT(int wanted)
{
	ofstream file("auxiliary.txt", ios::app);
	int max_list[10] = { 0,-1,-1,-1,-1,-1 };
	double num_list[10] = { 0,minimum_relevance,minimum_relevance,minimum_relevance,minimum_relevance,minimum_relevance };
	for (int i = 0; i <= file_num; i++)
	{
		if (i == wanted)
		{
			continue;
		}
		for (int j = 1; j <= 5; j++)
		{
			if (relevance_size[wanted][i] >= num_list[j])
			{
				g_MoveRightAList(&num_list[j], 5 - j + 1, relevance_size[wanted][i]);
				g_MoveRightAList(&max_list[j], 5 - j + 1, i);
				break;
			}
		}
	}
	for (int i = 1; i <= 5; i++)
	{
		if (max_list[i] != -1)
		{
			file<<word_links[max_list[i]]->m_Code << " ";
		}
	}
	file << "\n";
	file.close();
}

void g_HandleRecommendQuery(NDictionary* the_dictionary)
{
	//open files
	std::ifstream file_in("query2.txt", std::ios::in);
	if (!file_in.is_open())
	{
		return;
	}
	ofstream file_out("result2.txt", ios::trunc);
	while(1)
	{
		NCharString* the_title = NULL;
		char line[3000];
		if (!file_in.getline(line, 3000))
		{
			break;
		}
		the_title = new NCharString(line, strlen(line));
		NWordLink* the_wordlink = new NWordLink(the_title, 0);
		g_BuildTitleLink(the_wordlink, the_dictionary);
		bool whether_success = 0;
		for (int i = 0; i <= file_num; i++)
		{
			if (title_links[i] != NULL)
			{
				double jecard = 0, cosine = 0;
				the_wordlink->m_GetRelevance(title_links[i], jecard, cosine);
				if (jecard + cosine > 2 * minimum_same)
				{
					g_Recommend(i);
					whether_success = 1;
					break;
				}
			}
		}
		if (whether_success == 0)
		{
			fstream file_out("result2.txt", ios::app);
			file_out << "The news does not exist" << endl;
			file_out.close();
		}
		delete(the_wordlink);
		delete(the_title);
	}
}

NDictionary* g_InitDictionary()
{
	char dictionary_name[100] = "WordList/WordList.txt";
	NDictionary* the_dictionary = new NDictionary(dictionary_name);
	return the_dictionary;
}

int main()
{

	NDictionary* the_dictionary = g_InitDictionary();

	NReversedWordList* the_wordlist = new NReversedWordList(the_dictionary);
	
	
	//find all files;
	NCharStringLink* all_files_input = g_FindAllFilesInput();
	if (all_files_input == NULL)
	{
		cout << "No Valid Files!";
		return 0;
	}
	
	
	//handle all news
	/*NChainNode<NCharString>* present = all_files_input->m_Head;
	while (present != NULL)
	{
		if (present != all_files_input->m_Head)
		{
			NNews* the_news = new NNews(present->m_Data);	//等于提取信息接口，内容保存在news类里
			the_news->m_Dictionary = the_dictionary;
			if (the_news->m_Content != NULL)
			{
				the_news->m_DivideWord();	//等于分词接口，内容保存在news类里
				the_news->m_OutPut();
				delete(the_news);
			}

		}
		present = present->m_Next;
	}
	*/

	//find all out and info files
	NCharStringLink* all_files_output = g_FindAllFilesOutput();
	if (all_files_output == NULL)
	{
		cout << "No Valid Files!";
		return 0;
	}
	NCharStringLink* all_files_word = g_FindAllFilesWord();
	if (all_files_word == NULL)
	{
		cout << "No Valid Files!";
		return 0;
	}
	NChainNode<NCharString>* present_out = all_files_output->m_Head;
	NChainNode<NCharString>* present_word = all_files_word->m_Head;
	while (present_out != NULL)
	{
		if (present_out != all_files_output->m_Head)
		{
			 NWordLink* a_file = the_wordlist->m_AddOneFile(present_out->m_Data, present_word->m_Data);
			 word_links[a_file->m_Code] = a_file;
			 title_links[a_file->m_Code] = new NWordLink(a_file->m_Title, a_file->m_Code);
			 g_BuildTitleLink(title_links[a_file->m_Code], the_dictionary);
		}
		present_out = present_out->m_Next;
		present_word = present_word->m_Next;
	}
	

	the_wordlist->m_HandleWordSearch(the_dictionary);


	for (int i = 0; i < file_num; i++)
	{
		for (int j = i + 1; j <= file_num; j++)
		{
			if (word_links[i] != NULL && word_links[j] != NULL)
			{
				double jeccard = 0, cosine = 0;
				word_links[i]->m_GetRelevance(word_links[j], jeccard, cosine);
				relevance_size[i][j] = (jeccard + cosine) / 2;
				relevance_size[j][i] = (jeccard + cosine) / 2;
			}
		}
	}

	
	g_HandleRecommendQuery(the_dictionary);


	/*for (int i = 0; i <= file_num; i++)
	{
		g_Recommend_QT(i);
	}*/


	return 0;
}

