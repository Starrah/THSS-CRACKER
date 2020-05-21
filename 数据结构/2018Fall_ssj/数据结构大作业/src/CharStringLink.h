#pragma once
#include <iostream>
#include <fstream>
#include "CharString.h"
using namespace std;

class CharNode
{
public:
	CharString data;
	CharNode* next;
	CharNode() { data = ""; next = NULL; }
};

class CharStringLink
{
private:
	CharNode * head;
public:
	CharStringLink();
	~CharStringLink();
	void traverse();
	void traverse(CharString filepath);
	void add(int i, CharString str);
	void add(CharString str);
	void remove(CharString str);
	void remove(int i);
	CharNode* search(CharString str);
	int searchpos(CharString str);
};