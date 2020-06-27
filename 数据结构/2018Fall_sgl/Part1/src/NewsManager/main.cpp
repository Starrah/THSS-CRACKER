#include <iostream>
#include <fstream>
#include <cstring>



using namespace std;

#include "NCharString.h"
#include "NChainLink.hpp"
#include "NCharStringLink.hpp"
#include "NNews.h"
#include "NDictionary.hpp"
#include "NFindFiles.hpp"

NDictionary* g_InitDictionary()
{
	char dictionary_name[100] = "WordList/WordList.txt";
	NDictionary* the_dictionary = new NDictionary(dictionary_name);
	return the_dictionary;
}
int main()
{

	NDictionary* the_dictionary = g_InitDictionary();

	//find all files;
	NCharStringLink* all_files = g_FindAllFiles();
	if (all_files == NULL)
	{
		cout << "No Valid Files!";
		return 0;
	}
	
	
	/*char test[20] = "input/4.html";
	NNews* the_news = new NNews(test);
	the_news->m_Dictionary = the_dictionary;
	the_news->m_DivideWord();
	the_news->m_OutPut();*/

	//handle all news
	NChainNode<NCharString>* present = all_files->m_Head;
	while (present != NULL)
	{
		if (present != all_files->m_Head)
		{
			NNews* the_news = new NNews(present->m_Data);	//等于提取信息接口，内容保存在news类里
			the_news->m_Dictionary = the_dictionary;
			the_news->m_DivideWord();	//等于分词接口，内容保存在news类里
			the_news->m_OutPut();



			//todo
			delete(the_news);
		}
		present = present->m_Next;
	}
	return 0;
}

