#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <QString>
#include <QTextCodec>
#include <string>
#include "DocLink.h"
using namespace std;
bool ischar(wchar_t c);
bool isBlank(wchar_t c);
void bubblesort(info* p, int n);
string qstr2str(QString qstr);

#endif // UTILS_H
