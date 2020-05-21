#ifndef GETFILES_H
#define GETFILES_H
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <io.h>
#include <direct.h>
#include <vector>
#include <QDebug>
#include <QString>
#include "CharString.h"
#include "CharStringLink.h"
#include "AVLTree.h"
#include "DocLink.h"
#include "mydic.h"
#include <string.h>
#include <QFile>
#include "Stack.h"
#include "utils.h"

using namespace std;

void getFiles_html(string path, vector<string>& files);
void getAllFile(vector<string>& files);
void initDictionary(MyDicNode* dictionary);
void divideWords(CharString* result, MyDicNode* dictionary, CharStringLink* link);
void creatAVL(CharString* filepath,AVLTree* tree,int size);
int findArticle(CharString title);
#endif // GETFILES_H
