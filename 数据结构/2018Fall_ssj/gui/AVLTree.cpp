#include "AVLTree.h"
#include "AVLTree.h"

int Max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

AVLTree::AVLTree() :root(NULL) {}

AVLTree::~AVLTree()
{
    Destroy(root);
}

int AVLTree::GetHeight()
{
    return GetHeight(root);
}


void AVLTree::initConnection(double ** connection)
{
    initConnection(root, connection);
}

AVLTreeNode * AVLTree::Search(CharString key)
{
    //setlocale(LC_ALL, "chs");
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //wcout<<key.data<<endl;
    AVLTreeNode* temp = root;
    //wcout<<root->key.data<<" ";
    while ((temp != NULL) && (temp->key != key))
    {
        //wcout << temp->key.data << " ";
        if (key < temp->key)
        {
            temp = temp->lchild;
            //cout << "lchild" << endl;
        }
        else
        {
            temp = temp->rchild;
            //cout << "rchild" << endl;

        }
    }
    return temp;
}

void AVLTree::Insert(CharString key, int article_id)
{
    Insert(root, key, article_id);
}

void AVLTree::initConnection(AVLTreeNode * root, double ** connection)
{
    //setlocale(LC_ALL, "chs");
    if (root != NULL)
    {
        //wcout << root->key.data << " " << endl;
        double len = root->doc->GetLength();
        //int i, j;
        for (int i = 0; i < len - 1; i++)
        {
            double a1 = root->doc->GetInfo(i).article_id;
            double n1 = root->doc->GetInfo(i).num;
            for (int j = i + 1; j < len; j++)
            {
                double a2 = root->doc->GetInfo(j).article_id;
                double n2 = root->doc->GetInfo(j).num;
                connection[(int)a1][(int)a2] += (n1 + n2) / len;
                connection[(int)a2][(int)a1] += (n1 + n2) / len;
            }
        }
        initConnection(root->lchild, connection);
        initConnection(root->rchild, connection);
    }
}

void AVLTree::Adjust(AVLTreeNode * &r)
{
    if (r == NULL)
        return;
    if (GetHeight(r->lchild) - GetHeight(r->rchild) >= 2)
    {
        if (GetHeight(r->lchild->lchild) >= GetHeight(r->lchild->rchild))
        {
            LL(r);
        }
        else
        {
            LR(r);
        }
    }
    else if (GetHeight(r->rchild) - GetHeight(r->lchild) >= 2)
    {
        if (GetHeight(r->rchild->rchild) >= GetHeight(r->rchild->lchild))
        {
            RR(r);
        }
        else
        {
            RL(r);
        }
    }
    r->height = Max(GetHeight(r->lchild), GetHeight(r->rchild)) + 1;
}

void AVLTree::LL(AVLTreeNode * &k2)
{
    AVLTreeNode* k1;
    k1 = k2->lchild;
    k2->lchild = k1->rchild;
    k1->rchild = k2;

    k2->height = Max(GetHeight(k2->lchild), GetHeight(k2->rchild)) + 1;
    k1->height = Max(GetHeight(k1->lchild), GetHeight(k2)) + 1;
    k2 = k1;
}

void AVLTree::RR(AVLTreeNode * &k2)
{
    AVLTreeNode* k1;
    k1 = k2->rchild;
    k2->rchild = k1->lchild;
    k1->lchild = k2;

    k2->height = Max(GetHeight(k2->lchild), GetHeight(k2->rchild)) + 1;
    k1->height = Max(GetHeight(k1->lchild), GetHeight(k2)) + 1;
    k2 = k1;
}

void AVLTree::LR(AVLTreeNode * &k2)
{
    RR(k2->lchild);
    LL(k2);
}

void  AVLTree::RL(AVLTreeNode * &k2)
{
    LL(k2->rchild);
    RR(k2);
}

void AVLTree::Destroy(AVLTreeNode *& root)
{
    if (root == NULL)
        return;
    if (root->lchild != NULL)
        Destroy(root->lchild);
    if (root->rchild != NULL)
        Destroy(root->rchild);
    delete root;
}

int AVLTree::GetHeight(AVLTreeNode * root)
{
    if (root != NULL)
        return root->height;
    else
        return 0;
}

void AVLTree::Insert(AVLTreeNode *& root, CharString key, int article_id)
{
    if (root == NULL)
    {
        root = new AVLTreeNode(key, NULL, NULL);
        root->doc->Add(article_id);
    }
    else if (key < root->key)
    {
        Insert(root->lchild, key, article_id);
    }
    else if (key > root->key)
    {
        Insert(root->rchild, key, article_id);
    }
    else
    {
        root->doc->Add(article_id);
    }
    Adjust(root);
}

