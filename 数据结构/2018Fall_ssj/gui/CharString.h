#ifndef CHARSTRING_H
#define CHARSTRING_H
#include "utils.h"
#include <iostream>
#include <QString>
#include <cwchar>
#include <Windows.h>
#pragma warning(disable:4996)
using namespace std;

class CharString
{
public:
    CharString();
    ~CharString() {
        delete[] data;
    }
    CharString(const char c);
    CharString(const wchar_t c);
    CharString(const char*str);
    CharString(const string&s);
    CharString(const wstring&s);
    CharString(const wchar_t *str);
    CharString(const CharString&cs);
    QString Get_QString();
    int getSize();
    int indexOf(const CharString& s);
    int indexOf(const wchar_t* s);
    int indexOf(const wchar_t* s, int slen);
    int toint();
    CharString substring(int pos, int len);
    CharString& operator=(const CharString&s);
    CharString& operator=(const wchar_t* s);
    void concat(const CharString& str);
    void concat(const wchar_t* str);
    void removeBlank();
    wchar_t& operator[](int index);
    wchar_t operator[](int index)const;
    CharString& operator+=(const CharString&s);
    CharString& operator+=(const wchar_t* str);
    bool operator==(const CharString& s);
    bool operator!=(const CharString& s);
    bool operator<(const CharString& s);
    bool operator>(const CharString& s);
    wchar_t* getData();
public:
    wchar_t* data;
    int length;
    void init(const wchar_t* s);
};
#endif // CHARSTRING_H
