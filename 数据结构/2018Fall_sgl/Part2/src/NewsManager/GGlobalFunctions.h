#pragma once
/*
filename: GGlobalFunctions.h
description: global mathematical functions
version: 1.0
date: 10/21/2018
*/

#ifndef GGLOBALFUNCTIONS_HPP
#define GGLOBALFUNCTIONS_HPP
#include "NCharString.h"

//return the least of a1,a2,a3,a4
extern int g_JudgeMin(int a1, int a2, int a3);

//return the number of least of a1,a2,a3,a4, if no:return -1
extern int g_GetMinNumber(int a1, int a2, int a3);

//switch a char to int
extern int g_SwitchCharToInt(char c[], int end);

//right move a list
extern void g_MoveRightAList(int c[], int length, int new_num);
extern void g_MoveRightAList(double c[], int length, double new_num);


//switch a num < 1000 to charstring
extern NCharString* g_SwitchIntToChar(int to_be_done);

//get the .info from the code of the file
extern NCharString* g_GetFileName(int file_num);

#endif
