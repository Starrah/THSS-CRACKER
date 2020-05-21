#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QDebug>

#include "CharString.h"
#include "CharStringLink.h"
#include "Stack.h"
#include "getfiles.h"
#include "utils.h"
#include "mydic.h"
#include "DocLink.h"
#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <QTextCodec>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void prepare();
    void Query1(CharString s1);
    void Query2(CharString s1);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    vector<string>files;
    CharString* filename;
    CharString* filepath;
    MyDicNode* dictionary;
    CharStringLink** link;
    AVLTree* tree;
    double** connection;
};

#endif // MAINWINDOW_H
