#ifndef MYDIC_H
#define MYDIC_H

#include <iostream>
#include "CharString.h"

using namespace std;

class MyDicNode
{
private:
    MyDicNode* head;
    MyDicNode* next;
    MyDicNode* down;
    wchar_t data;
    int endmark;
public:
    MyDicNode();
    void insert(CharString str);
    MyDicNode* findNode_not(MyDicNode* first, wchar_t c);
    MyDicNode* findNode(MyDicNode* first, wchar_t c);
    bool findString(CharString str);
};
#endif // MYDIC_H
