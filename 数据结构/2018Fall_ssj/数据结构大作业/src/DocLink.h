#pragma once
#include <iostream>
#include <direct.h>
#include <fstream>
#include "CharString.h"

using namespace std;

class info//存储文档编号和出现次数
{
public:
	int article_id;//文档编号
	int num;//出现次数
	info(int article_id, int num) :article_id(article_id), num(num) {}
	info() :article_id(-1), num(0) {}
	info& operator=(const info& in);
};

class DocNode//文档节点
{
public:
	info data;
	DocNode* next;
	DocNode() { next = NULL; }
};

class DocLink
{
private:
	DocNode *head;
	int len;
public:
	DocLink() { head = NULL; len = 0; }
	int GetLength();
	info GetInfo(int pos);
	bool Edit(info in);//将相同文档编号的num相加，并保持有序的函数
	int Search(int article_id);
	bool Add(int article_id);//添加节点，添加时便进行排序，较为简单
	bool Remove(int article_id);
	void Traverse();
	void FileTraverse(wofstream& output);//这个函数把this的文档链表的信息写入到文件
	void addto(DocLink* p1);//这个函数把this的文档链表中的内容增加到p1里面
};
