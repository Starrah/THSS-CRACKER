/*
filename: NNews.h
description: the class of a news file, including construction and handling
version: 1.0
date: 10/21/2018
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <Windows.h>
using namespace std;

#include "NCharString.h"
#include "NStack.hpp"
#include "NCharStringLink.hpp"
#include "NNews.h"

//return the least of a1,a2,a3,a4
int g_JudgeMin(int a1, int a2, int a3)
{
	if (a1 != -1 && a1 < a2 && a1 <= a3)
	{
		return 1;
	}
	else if (a2 != -1 && a2 <= a1 && a2 <= a3)
	{
		return 2;
	}
	else if (a3 != -1 && a3 <= a1 && a3 <= a2)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

//return the number of least of a1,a2,a3,a4, if no:return -1
int g_GetMinNumber(int a1, int a2, int a3)
{
	if (a1 != -1 && a1 < a2 && a1 <= a3)
	{
		return a1;
	}
	else if (a2 != -1 && a2 <= a1 && a2 <= a3)
	{
		return a2;
	}
	else if (a3 != -1 && a3 <= a1 && a3 <= a2)
	{
		return a3;
	}
	else
	{
		return -1;
	}
}


void NNews::m_DivideWord()
{
	int max_length = m_Dictionary->m_MaxLength;
	int i = 0;
	while (i < m_Content_Chinese->m_Length - 1)
	{
		if (m_Content_Chinese->m_String[i] >= 0 && m_Content_Chinese->m_String[i] <= 127)
		{
			i++;
			continue;
		}
		bool whether_success = 0;
		int length = max_length;
		NCharString* current_string = m_Content_Chinese->m_CutString(i, min(i + length - 1, m_Content_Chinese->m_Length - 1));
		while (length > 0)
		{
			if (i >= 218)
			{
				int frog = 1;
			}
			if (i + length - 1 < m_Content_Chinese->m_Length)
			{
				NCharString* ans_string = m_Dictionary->m_HashList->m_SearchList(current_string);
				if (ans_string != NULL)
				{
					m_WordList.m_PushBack(ans_string);
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

					}
				}
				
			}
			else
			{
				length = m_Content_Chinese->m_Length - i;
			}
		}
		delete(current_string);

		if (whether_success == 0)
		{
			i +=2;
		}
	}
}

void NNews::m_OutPut()
{
	//todo: get right filename
	//todo: output others

	char info_c[10] = "info";
	char txt_c[10] = "txt";
	char front_c[10] = "output/";
	NCharString* out_info_name = new NCharString(front_c, 7);
	NCharString* out_word_name = new NCharString(front_c, 7);
	NCharString* out_info_back = new NCharString(info_c, 4);
	NCharString* out_txt_back = new NCharString(txt_c, 3);
	NCharString* name_model = m_FileName->m_CutString(6, m_FileName->m_Length - 5);
	out_info_name->m_PushBackString(name_model);
	out_info_name->m_PushBackString(out_info_back);
	out_word_name->m_PushBackString(name_model);
	out_word_name->m_PushBackString(out_txt_back);

	//open file
	char real_file_info[1000] = { 0 };
	for (int i = 0; i < out_info_name->m_Length; i++)
	{
		real_file_info[i] = out_info_name->m_String[i];
	}
	ofstream file_1(real_file_info);
	char real_file_txt[1000] = { 0 };
	for (int i = 0; i < out_word_name->m_Length; i++)
	{
		real_file_txt[i] = out_word_name->m_String[i];
	}
	ofstream file_2(real_file_txt);




	if (m_Title != NULL)
	{
		m_Title->m_OutPut(out_info_name);
	}
	if (m_Time != NULL)
	{
		m_Time->m_OutPut(out_info_name);
	}
	if (m_Source != NULL)
	{
		m_Source->m_OutPut(out_info_name);
	}
	if (m_Content != NULL)
	{
		m_Content->m_OutPut(out_info_name);
	}

	NChainNode<NCharString>* present = m_WordList.m_Head;
	while (present != NULL)
	{
		if (present != m_WordList.m_Head)
		{
			present->m_Data->m_OutPut(out_word_name);
		}
		present = present->m_Next;
	}

}

NNews::NNews(NCharString* filename)
{
	char real_file[100] = { 0 };
	for (int i = 0; i < filename->m_Length; i++)
	{
		real_file[i] = filename->m_String[i];
	}
	ifstream file(real_file,ios::in);
	if (!file.is_open())
	{
		return;
	}
	m_FileName = filename;
	NCharString* the_str = new NCharString();
	while (!file.eof())
	{
		
		char c = file.get();
		the_str->m_PushBackChar(c);

	}
	char begin_hint_c[10] = "<";
	char end_hint_left_c[10] = "</";
	char end_tag_c[10] = ">";
	char space_c[10] = " ";
	char title_tag_c[10] = "h1";
	char news_tag_c[10] = "div";
	char content_tag_c[30] = "textarea";
	char disable_tag_1_c[10] = "script";
	char disable_tag_2_c[10] = "img";
	char disable_tag_3_c[10] = "i";
	char disable_tag_4_c[10] = "style";
	char news_source_c[30] = "class=\"post_time_source\"";
	char news_source_appendix_c[30] = "ep-time-sour";
	char news_content_c[30] = "class=\"post_text\"";
	char news_content_appendix_c[30] = "id=\"endText\"";




	NCharString* begin_hint = new NCharString(begin_hint_c, 1);
	NCharString* end_hint_left = new NCharString(end_hint_left_c, 2);
	NCharString* end_tag = new NCharString(end_tag_c, 1);
	NCharString* space = new NCharString(space_c, 1);
	NCharString* title_tag = new NCharString(title_tag_c, 2);
	NCharString* news_tag = new NCharString(news_tag_c, 3);
	NCharString* content_tag = new NCharString(content_tag_c, 8);
	NCharString* disable_tag_1 = new NCharString(disable_tag_1_c, 6);
	NCharString* disable_tag_2 = new NCharString(disable_tag_2_c, 3);
	NCharString* disable_tag_3 = new NCharString(disable_tag_3_c, 1);
	NCharString* disable_tag_4 = new NCharString(disable_tag_4_c, 5);
	NCharString* news_source = new NCharString(news_source_c, 24);
	NCharString* news_source_appendix = new NCharString(news_source_appendix_c, 12);
	NCharString* news_content = new NCharString(news_content_c, 17);
	NCharString* news_content_appendix = new NCharString(news_content_appendix_c, 12);


	int i = 0;
	bool whether_source_in_stack = 0;
	bool whether_content_in_stack = 0;
	while (i < the_str->m_Length)
	{
		//initialize
		int begin_num = the_str->m_FindSubString(begin_hint, i);
		int end_left_num = the_str->m_FindSubString(end_hint_left, i);
		int end_tag_num = the_str->m_FindSubString(end_tag, i);
		int end_handle_place = g_GetMinNumber(begin_num, end_left_num, end_tag_num);
		int operation_type = g_JudgeMin(begin_num, end_left_num, end_tag_num);

		NCharString* stack_top_tag = m_NewsStack.m_GetTopTag();
		NCharString* stack_top_content = m_NewsStack.m_GetTopContent();
		if (end_handle_place == -1)
		{
			break;
		}

		//handle content
		//judge whether is title, source/time, maintext
		if (title_tag->m_Compare(stack_top_tag) == 0)
		{
			m_Title = the_str->m_CutString(i, end_left_num - 1);
		}
		else
		{
			if (news_tag->m_Compare(stack_top_tag) == 0)
			{
				int judge_source;
				int judge_content;
				if (stack_top_content == NULL)
				{
					judge_source = -1;
					judge_content = -1;
				}
				else
				{
					judge_source = stack_top_content->m_FindSubString(news_source, 0);
					judge_content = stack_top_content->m_FindSubString(news_content, 0);
					if (judge_source == -1)
					{
						judge_source = stack_top_content->m_FindSubString(news_source_appendix, 0);
					}
					judge_content = stack_top_content->m_FindSubString(news_content, 0);
					if (judge_content == -1)
					{
						judge_content = stack_top_content->m_FindSubString(news_content_appendix, 0);
					}
				}
				if (judge_source >= 0)
				{
					whether_source_in_stack = 1;
				}

				else if (judge_content >= 0)
				{
					whether_content_in_stack = 1;
				}

			}
			if (whether_content_in_stack == 0 && content_tag->m_Compare(stack_top_tag) == 0)
			{
				whether_content_in_stack = 1;
			}
		}

		if (whether_source_in_stack == 1)
		{
			//handle source and time
			for (int j = i; j < end_handle_place; j++)
			{
				if (the_str->m_String[j] >= '0' && the_str->m_String[j] <= '9')
				{
					m_Time = the_str->m_CutString(j, j + 18);
					break;
				}
				if (the_str->m_String[j] < 0 || the_str->m_String[j] > 127)
				{
					m_Source = the_str->m_CutString(j, end_handle_place - 1);
					break;
				}
			}
		}

		if (whether_content_in_stack == 1)
		{
			if (disable_tag_1->m_Compare(stack_top_tag) != 0 && disable_tag_2->m_Compare(stack_top_tag) != 0
				&& disable_tag_3->m_Compare(stack_top_tag) != 0 && disable_tag_4->m_Compare(stack_top_tag) != 0)
			{
				//handle content
				if (m_Content == NULL)
				{
					m_Content = new NCharString();
				}
				int kk;
				NCharString* new_content = the_str->m_CutString(i, end_handle_place - 1);
				m_Content->m_PushBackString(new_content);
				if (new_content != NULL)
				{
					delete(new_content);
				}
			}
		}

		//handle hrefs
		//in stack
		if (operation_type == 1)
		{
			int space_num = the_str->m_FindSubString(space, begin_num + 1);
			if (space_num <= end_tag_num)
			{
				NCharString* new_tag = the_str->m_CutString(begin_num + 1, space_num - 1);
				NCharString* new_content = the_str->m_CutString(space_num + 1, end_tag_num - 1);
				m_NewsStack.m_PushBack(new_tag, new_content);
			}
			else
			{
				NCharString* new_tag = the_str->m_CutString(begin_num + 1, end_tag_num - 1);
				NCharString* new_content = new NCharString();
				m_NewsStack.m_PushBack(new_tag, new_content);
			}
			i = end_tag_num + 1;
		}

		//out of stack
		else if (operation_type == 2)
		{
			NCharString* new_tag = the_str->m_CutString(end_left_num + 2, end_tag_num - 1);
			while (1)
			{
				if (m_NewsStack.m_JudgeEmpty())
				{
					break;
				}
				NCharString* stack_top = m_NewsStack.m_GetTopTag();
				NCharString* content_top = m_NewsStack.m_GetTopContent();
				if (content_top != NULL && whether_source_in_stack == 1 && news_tag->m_Compare(stack_top) == 0 && content_top->m_FindSubString(news_source, 0) != -1)
				{
					whether_source_in_stack = 0;
				}
				if (content_top != NULL && whether_content_in_stack == 1 && news_tag->m_Compare(stack_top) == 0)
				{
					if (content_top->m_FindSubString(news_content, 0) != -1 || content_top->m_FindSubString(news_content_appendix, 0) != -1)
					{
						whether_content_in_stack = 0;
					}
				}
				if (content_top != NULL && whether_content_in_stack == 1 && content_tag->m_Compare(stack_top) == 0)
				{
					whether_content_in_stack = 0;
				}
				if (new_tag->m_Compare(stack_top) == 0)
				{
					m_NewsStack.m_PopBack();
					break;
				}
				else
					m_NewsStack.m_PopBack();
			}
			i = end_tag_num + 1;
			delete(new_tag);
		}
		else
		{
			i = end_tag_num + 1;
		}

	}
	delete(the_str);

	m_Content_Chinese = new NCharString();
	for (int i = 0; i < m_Content->m_Length; i++)
	{
		if (m_Content->m_String[i]<0 || m_Content->m_String[i] >127)
		{
			m_Content_Chinese->m_PushBackChar(m_Content->m_String[i]);
		}
	}
}

