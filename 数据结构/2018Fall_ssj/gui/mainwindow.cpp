#include "mainwindow.h"
#include "ui_mainwindow.h"

CharString QStringToCharStr(QString text)
{
    QByteArray cdata = text.toLocal8Bit();
    string str=string(cdata);
    CharString charstr=str;
    return charstr;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    prepare();
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Query1(CharString s1)
{
    wstring line = s1.data;
    DocLink doc;
    CharString word(line);
    word+=L" ";
    CharString s = L"";
    DocLink* result = new DocLink;
    for (int i = 0; i < word.length; i++)
    {
        wchar_t temp = word[i];
        if (temp != L' ')
        {
            s += temp;
        }
        else
        {
            if (s != L"")
            {
                DocLink* p = NULL;

                if (tree->Search(s) != NULL)
                {
                    p = tree->Search(s)->doc;
                    p->addto(result);
                }
                else
                {
                    cout<<"NULL!"<<endl;
                }
                s = L"";
            }
        }
    }
    int* p1=NULL;
    result->Traverse();
    result->arrayTraverse(p1);
    if(p1==NULL)
    {
        cout<<"NULL"<<endl;
        return;
    }
    for(int i=0;i<5;i++)
    {
        cout<<p1[i]<<" ";
    }
    cout<<endl;

    char path[128];
    _getcwd(path, 128);
    string outputpath = path;
    outputpath += "\\result2_gui.txt";
    wofstream output(outputpath);
    if (!output)
    {
        cout << "cannot open output file!" << endl;
        return;
    }
    for (int i = 0; i < 5; i++)
    {
        CharString filetemp = path;
        filetemp += "\\output";
        filetemp += "\\";
        string s = to_string(p1[i]);
        filetemp += s;
        filetemp += "_info.txt";
        wifstream input(filetemp.data);
        if (!input)
        {
            cout << "cannot open input file!" << endl;
            return;
        }
        wstring wtemptitle;
        getline(input, wtemptitle);
        CharString temptitle(wtemptitle);
        input.close();
        output <<temptitle.data << endl;
    }
    output.close();

    QFile file("result2_gui.txt");
    if(!file.open(QIODevice::ReadOnly|QFile::Text))
    {
        cout<<"cannot open"<<endl;
        return;
    }
    QTextCodec* codec=(QTextCodec::codecForName("GBK"));
    while(!file.atEnd())
    {
        QByteArray line1 = file.readLine();
        QString str = codec->toUnicode(line1);
        ui->textEdit->append(str);
    }
    file.close();
}

void MainWindow::Query2(CharString s1)
{
    s1.removeBlank();
    wstring line = s1.data;
    wcout<<line<<endl;
    char path[128];
    _getcwd(path, 128);
    string outputpath = path;
    outputpath += "\\result2_GUI.txt";
    wofstream output(outputpath);
    if (!output)
    {
        cout << "cannot open output file!" << endl;
        return;
    }
    int id = findArticle(line);
    if(id==-1)
    {
        cout<<"cannot find!"<<endl;
        return;
    }
    cout<<id<<endl;
    info* temp = new info[781];
    for (int i = 0; i < 781; i++)
    {
        temp[i].num = connection[id][i];
        temp[i].article_id = i;
    }
    bubblesort(temp, 781);
    cout << endl;
    for (int i = 0; i < 5; i++)
    {
        //cout << temp[i].article_id << endl;
        CharString filetemp = path;
        filetemp += "\\output";
        filetemp += "\\";
        string s = to_string(temp[i].article_id);
        filetemp += s;
        filetemp += "_info.txt";
        wifstream input(filetemp.data);
        if (!input)
        {
            cout << "cannot open input file!" << endl;
            return;
        }
        wstring wtemptitle;
        getline(input, wtemptitle);
        CharString temptitle(wtemptitle);
        input.close();
        output << L"(" << temp[i].article_id << L"," << temptitle.data << L")" <<endl;
    }
    output << endl;
    delete[]temp;
    temp = NULL;
    output.close();

    QFile file("result2_GUI.txt");
    if(!file.open(QIODevice::ReadOnly|QFile::Text))
    {
        cout<<"cannot open"<<endl;
        return;
    }
    QTextCodec* codec=(QTextCodec::codecForName("GBK"));
    //QTextStream out(&file);
    //out.setCodec(codec);
    while(!file.atEnd())
    {
        QByteArray line1 = file.readLine();
        QString str = codec->toUnicode(line1);
        ui->textEdit->append(str);
    }
}
void MainWindow::prepare()
{
    int size = 781;
    filename = new CharString[size];
    filepath = new CharString[size];

    for (int i = 0; i < size; i++)
    {
        string s=to_string(i);
        CharString name = s;
        //wcout<<name.data<<endl;
        char path[128];
        _getcwd(path, 128);
        filepath[i] = path;
        filepath[i] += "\\output";
        filepath[i] += "\\";
        filepath[i] += name;
        filepath[i] += L".txt";
        wcout<<filepath[i].data<<endl;
    }

    delete[]filename;
    tree = new AVLTree;
    //qDebug()<<"prepare"<<endl;
    creatAVL(filepath, tree, size);
    //tree->preOrder();
    delete[]filepath;

    connection = new double*[781];
    for (int i = 0; i < 781; i++)
    {
        connection[i] = new double[781];
        for (int j = 0; j < 781; j++)
        {
            connection[i][j] = 0;
        }
    }
    tree->initConnection(connection);
}

void MainWindow::on_pushButton_clicked()
{
    //query1
    QString text=ui->lineEdit->text();
    //qDebug()<<text;
    ui->textEdit->clear();
    QFile file("temp.txt");//tempfile,for correctly get Chinese characters
    if(!file.open(QIODevice::WriteOnly|QFile::Text))
    {
        cout<<"cannot open"<<endl;
    }
    QTextCodec* codec=(QTextCodec::codecForName("GBK"));//set code
    QTextStream out(&file);
    out.setCodec(codec);
    out<<text;
    file.flush();
    file.close();

    wifstream file_1;
    file_1.open("temp.txt");//read input
    if (!file_1)
    {
        cout<<"cannot open"<<endl;
    }
    wstring buff;
    getline(file_1, buff);
    CharString word(buff);
    file_1.close();
    wcout<<word.data<<endl;
    Query1(word);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString text=ui->lineEdit_2->text();
    //qDebug()<<text;
    ui->textEdit->clear();
    QFile file("temp.txt");//tempfile,for correctly get Chinese characters
    if(!file.open(QIODevice::WriteOnly|QFile::Text))
    {
        cout<<"cannot open"<<endl;
    }
    QTextCodec* codec=(QTextCodec::codecForName("GBK"));
    QTextStream out(&file);
    out.setCodec(codec);
    out<<text;
    file.flush();
    file.close();

    wifstream file_1;
    file_1.open("temp.txt");
    if (!file_1)
    {
        cout<<"cannot open"<<endl;
    }
    wstring buff;
    getline(file_1, buff);
    CharString word(buff);
    wcout<<word.data<<endl;
    Query2(word);
    file_1.close();
}
