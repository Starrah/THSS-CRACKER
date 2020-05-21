#include "DocLink.h"
#include "DocLink.h"


int DocLink::GetLength()
{
    return len;
}

info DocLink::GetInfo(int pos)
{
    DocNode* p = head;
    int i = 0;
    while (i < pos&&p != NULL)
    {
        p = p->next;
        i++;
    }
    if (p != NULL)
        return p->data;
}



bool DocLink::Edit(info in)
{
    int article_id = in.article_id;
    int num = in.num;
    DocNode* p;
    p = head;
    int pos = Search(article_id);
    if (pos == -1)
    {
        DocNode* t = new DocNode;
        t->data.article_id = article_id;
        t->data.num = num;
        if (head == NULL)
        {
            head = t;
            len++;
            return true;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = t;
            len++;
            while (1)
            {
                DocNode* temp = head;
                if (head == p)
                    return true;
                while (temp->next != p)
                {
                    temp = temp->next;
                }
                if (temp->data.num < p->data.num)
                {
                    info t = temp->data;
                    temp->data = p->data;
                    p->data = t;
                    p = temp;
                }
                else
                {
                    return true;
                }
            }
            return true;
        }
    }
    else
    {
        int i = 0;
        while (i < pos)
        {
            if (p == NULL)
                return false;
            p = p->next;
            i++;
        }
        p->data.num += num;

        if (head == p)
            return true;
        else
        {
            while (1)
            {
                DocNode* temp = head;
                if (head == p)
                    return true;
                while (temp->next != p)
                {
                    temp = temp->next;
                }
                if (temp->data.num < p->data.num)
                {
                    info t = temp->data;
                    temp->data = p->data;
                    p->data = t;
                    p = temp;
                }
                else
                {
                    return true;
                }
            }
            return true;
        }
        return true;
    }
}

int DocLink::Search(int article_id)
{
    DocNode* p;
    p = head;
    int pos = 0;
    while (p != NULL)
    {
        if (p->data.article_id == article_id)
            return pos;
        else
        {
            p = p->next;
            pos++;
        }
    }
    return -1;
}

bool DocLink::Add(int article_id)
{
    DocNode* p;
    p = head;
    int pos = Search(article_id);
    if (pos == -1)
    {
        DocNode* t = new DocNode;
        t->data.article_id = article_id;
        t->data.num = 1;
        if (head == NULL)
        {
            head = t;
            len++;
            return true;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = t;
            len++;
            return true;
        }
    }
    else
    {
        int i = 0;
        while (i < pos)
        {
            if (p == NULL)
                return false;
            p = p->next;
            i++;
        }
        p->data.num++;

        if (head == p)
            return true;
        else
        {
            while (1)
            {
                DocNode* temp = head;
                if (head == p)
                    return true;
                while (temp->next != p)
                {
                    temp = temp->next;
                }
                if (temp->data.num < p->data.num)
                {
                    info t = temp->data;
                    temp->data = p->data;
                    p->data = t;
                    p = temp;
                }
                else
                {
                    return true;
                }
            }
            return true;
        }
        return true;
    }
}

bool DocLink::Remove(int article_id)
{
    int pos = Search(article_id);
    int i = 0;
    if (pos < 0 || pos > len - 1)
    {
        return false;
    }
    DocNode* p = head;
    DocNode* q;
    if (pos == 0)
    {
        head = head->next;
        delete p;
        p = NULL;
        len--;
        return true;
    }
    while (i < pos - 1)
    {
        p = p->next;
        i++;
    }
    q = p->next;
    p->next = q->next;
    delete q;
    q = NULL;
    len--;
    return true;
}

void DocLink::Traverse()
{
    if (len > 0)
    {
        DocNode*p = head;
        int index = 0;
        while (index < GetLength() - 1)
        {
            cout << "(" << p->data.article_id << "," << p->data.num << ")" << " ";
            p = p->next;
            index++;
        }
        cout << "(" << p->data.article_id << "," << p->data.num << ")" << endl;
    }
}

void DocLink::addto(DocLink* p1)
{
    if (len > 0)
    {
        DocNode*p = head;
        int index = 0;
        while (index < GetLength() - 1)
        {
            p1->Edit(p->data);
            p = p->next;
            index++;
        }
        p1->Edit(p->data);
    }
}

void DocLink::arrayTraverse(int* &p1)
{
    if (len > 0)
    {
        p1=new int[len];
        DocNode* p = head;
        int index = 0;
        while (index < GetLength() - 1)
        {
            p1[index]=p->data.article_id;
            p = p->next;
            index++;
        }
        p1[index]=p->data.article_id;
    }
    else
    {
        p1 = NULL;
    }
}

info & info::operator=(const info & w)
{
    this->article_id = w.article_id;
    this->num = w.num;
    return *this;
}
