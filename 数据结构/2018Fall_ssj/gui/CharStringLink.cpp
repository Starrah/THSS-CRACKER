#include "CharStringLink.h"
#include "CharStringLink.h"

CharStringLink::CharStringLink()
{
    head = new CharNode;
    head->next = NULL;
    len=0;
}

CharStringLink::~CharStringLink()
{
    CharNode* temp;
    while (head->next != NULL)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
    delete head;
}

void CharStringLink::travel()
{
    CharNode* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
        wcout << temp->data.data << endl;
    }
}

void CharStringLink::travel(CharString filepath)
{
    locale::global(locale(""));

    wcout << filepath.data << endl;
    wofstream output;
    output.open(filepath.data);
    if (!output)
    {
        cout << "cannot open file" << endl;
    }
    CharNode* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
        output << temp->data.data << endl;
    }
}

void CharStringLink::add(int i, CharString str)
{
    CharNode* temp = head;
    int j = 0;
    while (j < i - 1 && temp != NULL)
    {
        temp = temp->next;
        j++;
    }
    if (temp == NULL || j > i - 1)
    {
        cout << "wrong pos!" << endl;
    }
    else
    {
        CharNode* n = new CharNode;
        n->data = str;
        n->next = temp->next;
        temp->next = n;
        len++;
    }
}

void CharStringLink::add(CharString str)
{
    CharNode* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    CharNode* n = new CharNode;
    n->data = str;
    temp->next = n;
    len++;
}

void CharStringLink::remove(CharString str)
{
    int i = this->searchpos(str);
    if (i == -1)
        return;
    CharNode* p = this->search(str);
    CharNode* pre = head;
    while (i - 1 != 0)
    {
        pre = pre->next;
        i--;
    }
    pre->next = p->next;
    delete p;
}

void CharStringLink::remove(int i)
{
    CharNode* pre = head;
    CharNode* p;
    while (i - 1 != 0)
    {
        pre = pre->next;
        i--;
    }
    p = pre->next;
    pre->next = p->next;
    delete p;
}

CharNode * CharStringLink::search(CharString str)
{
    CharNode* p = head;
    while (p != NULL)
    {
        if (p->data != str)
        {
            p = p->next;
        }
        else
        {
            return p;
        }
    }
    return NULL;
}

int CharStringLink::searchpos(CharString str)
{
    CharNode* p = head;
    int i = 0;
    while (p != NULL)
    {
        if (p->data != str)
        {
            p = p->next;
            i++;
        }
        else
        {
            return i;
        }
    }
    return -1;
}
