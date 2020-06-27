#pragma once
/*
filename: NNews.h
description: the class of a news file, including construction and handling
version: 1.0
date: 10/21/2018
*/

#ifndef NNEWS_H
#define NNEWS_H

#include <iostream>
#include <fstream>


#include "NCharString.h"
#include "NWCharString.h"
#include "NStack.hpp"
#include "NCharStringLink.hpp"
#include "NDictionary.hpp"
#include "GGlobalFunctions.h"



class NNews
{
	//data and stack
public:
	NCharString * m_Title{ NULL };
	NCharString* m_Source{ NULL };
	NCharString* m_Time{ NULL };
	NCharString* m_Content{ NULL };
	NCharString* m_Content_Chinese{ NULL };
	NCharString* m_FileName{ NULL };
	NDictionary* m_Dictionary{ NULL };
	NCharStringLink m_WordList;
	NStack m_NewsStack;


	//function
public:
	//function: construct from file
	//variable: filename
	NNews(NCharString* filename);

	//destruction
	~NNews()
	{
		delete(m_Title);
		delete(m_Source);
		delete(m_Time);
		delete(m_Content);
		delete(m_Content_Chinese);
		delete(m_FileName);
		m_WordList.m_Clear();
		m_NewsStack.m_Clear();
		//delete(m_Dictionary);
	}


	//function divide words
	//variable: 
	void m_DivideWord();

	//function:output
	void m_OutPut();







};


#endif