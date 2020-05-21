#ifndef DOCLINK_H
#define DOCLINK_H
#include <iostream>

using namespace std;

class info
{
public:
    int article_id = -1;
    int num = 0;
    info(int article_id, int num) :article_id(article_id), num(num) {}
    info() :article_id(-1), num(0) {}
    info& operator=(const info& w);
};

class DocNode
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
    bool Edit(info in);
    int Search(int article_id);
    bool Add(int article_id);
    bool Remove(int article_id);
    void Traverse();
    void addto(DocLink* p1);
    void arrayTraverse(int* &p1);
};
#endif // DOCLINK_H
