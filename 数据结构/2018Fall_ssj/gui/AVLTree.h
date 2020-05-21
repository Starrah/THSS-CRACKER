#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <QTextCodec>
#include "CharString.h"
#include "DocLink.h"

using namespace std;



class AVLTreeNode
{
public:
    CharString key;
    int height;
    AVLTreeNode *lchild;
    AVLTreeNode *rchild;
    DocLink* doc;
    AVLTreeNode(CharString k, AVLTreeNode *l, AVLTreeNode *r) :
        key(k), height(0), lchild(l), rchild(r)
    {
        doc = new DocLink;
    }
    ~AVLTreeNode()
    {
        delete doc;
    }
};

class AVLTree
{
private:
    AVLTreeNode* root;
public:
    AVLTree();
    ~AVLTree();

    int GetHeight();
    void preOrder();
    void initConnection(double** connection);
    AVLTreeNode* Search(CharString key);
    void Insert(CharString ket, int article_id);
private:
    void initConnection(AVLTreeNode* root, double** connection);//建立文档之间的关系，推荐时使用
            //是一个781*781的double类型矩阵，表示文档i，j的联系值大小
    void Adjust(AVLTreeNode* &root);//要求实现的接口
    void LL(AVLTreeNode* &k2);//双左旋
    void RR(AVLTreeNode* &k2);//双右旋
    void LR(AVLTreeNode* &k2);//左旋再右旋
    void RL(AVLTreeNode* &k2);//右旋再左旋
    void Destroy(AVLTreeNode* &root);
    int GetHeight(AVLTreeNode* root);
    void Insert(AVLTreeNode* &root, CharString key, int article_id);
};

#endif // AVLTREE_H
