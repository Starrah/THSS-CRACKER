/*
filename: NFindFiles.hpp
description: find all files in the place
version: 1.0
date: 11/3/2018
*/

#ifndef NFINDFILES_HPP
#define NFINDFILES_HPP

#include <iostream>
#include <windows.h>

#include "NCharString.h"
#include "NChainLink.hpp"
#include "NCharStringLink.hpp"




NCharStringLink* g_FindAllFilesInput()
{
	HANDLE hFile;
	LPCTSTR lpFileName = ".\\input\\*.html"; //指定搜索目录和文件类型，如搜索d盘的音频文件可以是"D:\\*.mp3"
	WIN32_FIND_DATA pNextInfo;  //搜索得到的文件信息将储存在pNextInfo中;


	NCharStringLink* all_files = new NCharStringLink();
	char c[30] = "input/";
	NCharString* file_model = new NCharString(c, 6);
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	{
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
	}
	//printf("%s\n", pNextInfo.cFileName);
	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')//过滤.和..
		{
			continue;
		}
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
		//printf("%s\n", pNextInfo.cFileName);
	}

	return all_files;
}

NCharStringLink* g_FindAllFilesOutput()
{
	HANDLE hFile;
	LPCTSTR lpFileName = ".\\output\\*.info"; //指定搜索目录和文件类型，如搜索d盘的音频文件可以是"D:\\*.mp3"
	WIN32_FIND_DATA pNextInfo;  //搜索得到的文件信息将储存在pNextInfo中;


	NCharStringLink* all_files = new NCharStringLink();
	char c[30] = "output/";
	NCharString* file_model = new NCharString(c, 7);
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	{
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
	}
	//printf("%s\n", pNextInfo.cFileName);
	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')//过滤.和..
		{
			continue;
		}
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
		//printf("%s\n", pNextInfo.cFileName);
	}

	return all_files;
}

NCharStringLink* g_FindAllFilesWord()
{
	HANDLE hFile;
	LPCTSTR lpFileName = ".\\output\\*.txt"; //指定搜索目录和文件类型，如搜索d盘的音频文件可以是"D:\\*.mp3"
	WIN32_FIND_DATA pNextInfo;  //搜索得到的文件信息将储存在pNextInfo中;


	NCharStringLink* all_files = new NCharStringLink();
	char c[30] = "output/";
	NCharString* file_model = new NCharString(c, 7);
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	{
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
	}
	//printf("%s\n", pNextInfo.cFileName);
	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == '.')//过滤.和..
		{
			continue;
		}
		NCharString* answer = file_model->m_Duplicate();
		NCharString* new_name = new NCharString(pNextInfo.cFileName, strlen(pNextInfo.cFileName));
		answer->m_PushBackString(new_name);
		delete(new_name);
		all_files->m_PushBack(answer);
		//printf("%s\n", pNextInfo.cFileName);
	}

	return all_files;
}

extern NCharStringLink* g_FindAllFilesInput();
extern NCharStringLink* g_FindAllFilesOutput();
extern NCharStringLink* g_FindAllFilesWord();


#endif