#include "mydic.h"
#include "mydic.h"

MyDicNode::MyDicNode()
{
    head = this;
    next = NULL;
    down = NULL;
    endmark = 0;
}

void MyDicNode::insert(CharString str)
{
    int len = str.getSize();
    MyDicNode* m_present = this->down;
    MyDicNode* m_up = this;
    MyDicNode *p;
    for (int i = 0; i < len; i++)
    {
        p = this->findNode_not(m_present, str[i]);
        if (p == NULL)
        {
            MyDicNode* q = new MyDicNode;
            q->data = str[i];
            m_up->down = q;
            m_up = q;
            m_present = q->down;//自解释，等同于NULL
            //cout << "NULL: ";
            //wcout << q->data << endl;
            if (i == len - 1)
            {
                q->endmark = 1;
            }
        }
        else
        {
            if (p->next == NULL && p->data != str[i])
            {
                MyDicNode* q = new MyDicNode;
                q->data = str[i];
                p->next = q;
                m_up = q;
                m_present = q->down;//自解释，等同于NULL
                //cout << "not exist: ";
                //wcout << q->data << endl;
                if (i == len - 1)
                {
                    q->endmark = 1;
                }
            }
            else if(p->next == NULL && p->data == str[i])
            {
                m_up = p;
                m_present = p->down;
                //cout << "find: ";
                //wcout << p->data << endl;
                if (i == len - 1)
                {
                    p->endmark = 1;
                }
            }
            else if (p->next != NULL)
            {
                m_up = p;
                m_present = p->down;
                //cout << "find: ";
                //wcout << p->data << endl;
                if (i == len - 1)
                {
                    p->endmark = 1;
                }
            }
        }
    }
}

MyDicNode * MyDicNode::findNode_not(MyDicNode * first, wchar_t c)//如果找到返回找到的那一个，如果没找到返回最后一个
{
    MyDicNode* p = first;
    if (first == NULL)
    {
        //cout << "first dont exist" << endl;
        return NULL;
    }
    while (p->data != c)
    {
        if (p->next == NULL)
        {
            //cout << "not find " << endl;
            return p;
        }
        p = p->next;
    }
    return p;
}

MyDicNode * MyDicNode::findNode(MyDicNode * first, wchar_t c)
{
    MyDicNode* p = first;
    if (first == NULL)
    {
        return NULL;
    }
    while (p->data != c)
    {
        if (p->next == NULL)
        {
            return NULL;
        }
        p = p->next;
    }
    return p;
}

bool MyDicNode::findString(CharString str)
{
    int len = str.getSize();
    MyDicNode* first = this->down;
    MyDicNode* f;
    if (len == 0)
        return false;
    for (int i = 0; i < len; i++)
    {
        f = findNode(first, str[i]);
        if (f == NULL)
        {
            return false;
        }

        if (i == len - 1)
        {
            if (f->endmark == 1)
            {
                return true;
            }
        }
        first = f->down;
    }
    return false;
}
