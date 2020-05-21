#include "getfiles.h"
void getFiles_html(string path, vector<string>& files)
{
    long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*.html").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles_html(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

void getAllFile(vector<string>& files)
{
    char path[128];
    _getcwd(path, 128);
    string filepath = path;
    filepath += "\\input";
    getFiles_html(filepath, files);
}
void initDictionary(MyDicNode* dictionary)
{
    setlocale(LC_ALL, "chs");
    //locale::global(locale(""));
    char path[128];
    _getcwd(path, 128);
    string filepath = path;
    filepath += "\\dic.txt";


    QFile file(filepath.c_str());
    if (file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            QByteArray encodedString = file.readLine();
            QTextCodec::ConverterState state;
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString text = codec->toUnicode( encodedString.constData(),
                                             encodedString.size(), &state);
            if (state.invalidChars > 0)  //GBK编码
            {
                text = QTextCodec::codecForName( "GBK" )->toUnicode(encodedString);

            }
            else   //UTF-8编码
            {
                text = encodedString;
            }
            QByteArray cdata = text.toLocal8Bit();
            string str=string(cdata);
            CharString charstr=str;

            dictionary->insert(charstr);
        }

        file.close();
    }
}

void divideWords(CharString* result, MyDicNode* dictionary, CharStringLink* link)
{
    locale::global(locale(""));
    CharString temp;
    while (1)
    {
        int len = 14;
        int flag = 0;
        while (len >= 2)
        {
            temp = result->substring(0, len);
            //wcout << temp.data << endl;
            if (dictionary->findString(temp))
            {
                link->add(temp);
                *result = result->substring(len, result->getSize() - len);
                flag = 1;
                break;
            }
            else
            {
                len--;
                //cout << "len=" << len << endl;
            }
        }
        if (flag == 0)
        {
            *result = result->substring(1, result->getSize() - 1);
        }
        if (result->getSize() <= 0)
            break;
    }
}

void creatAVL(CharString* filepath,AVLTree* tree,int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        //qDebug()<<"entered"<<endl;
        wifstream file;
        int j = filepath[i].indexOf(L".");
        int m;
        for (m = j; m >= 0; m--)
        {
            if (filepath[i][m] == (wchar_t)'\\')
                break;
        }

        CharString name = filepath[i].substring(m + 1, j - m - 1);
        int article_id = name.toint();
        file.open(filepath[i].data);

        if (!file)
        {
            qDebug() << "cannot open avl file!" << endl;
            break;
        }

        //wcout << filepath[i].data << " ";
        //cout << article_id << endl;
        while (!file.eof())
        {
            wstring buff;
            getline(file, buff);
            CharString word(buff);
            tree->Insert(word, article_id);
        }
        file.close();
    }
}

int findArticle(CharString title)
{

    for (int i = 0; i < 781; i++)
    {
        char path[128];
        _getcwd(path, 128);
        string filepath = path;
        filepath += "\\output";
        filepath += "\\";
        string s = to_string(i);
        filepath += s;
        filepath += "_info.txt";
        //cout << filepath << endl;
        wifstream input(filepath);
        if (!input)
        {
            cout << "cannot open article file!" << endl;
            return -1;
        }
        wstring wtemptitle;
        getline(input, wtemptitle);
        CharString temptitle(wtemptitle);
        //wcout<<temptitle.data<<endl;
        if (temptitle.indexOf(title.data) != -1)
        {
            input.close();
            return i;
        }
        input.close();
    }

    return -1;
}





/*CharString extractInfo(CharString filename)
{
    locale::global(locale(""));
    wifstream input(filename.getData());
    if (!input)
    {
        cout << "cannot open file!" << endl;
        return "";
    }
    wcout << filename.data << endl;
    input >> noskipws;
    CharString result = "";
    CharString tempResult = "";
    CharString tag = "";
    CharString tagchar = "";
    CharString t1 = "/";
    CharString time = "";
    CharString source = "";
    CharString title = "";
    Stack<CharString> tags;
    wchar_t temp;
    int explain = 0;
    int flag = 0;
    int getflag = 0;
    int oldgetflag = 0;
    int outertagget = 0;
    int getsource = 0;
    int gettime = 0;
    int gettitle = 0;
    while (input.good())
    {
        input >> temp;
        if (isBlank(temp) && gettime != 1)
            continue;
        if (temp == (wchar_t)'<')
        {
            oldgetflag = getflag;
            getflag = 0;
            int blank = 0;
            flag = 1;
            t1 = "/";
            tag = "";
            tagchar = "";
            while (temp != (wchar_t)'>'&&explain == 0)
            {

                input >> temp;
                if (isBlank(temp))
                    blank = 1;
                if (blank != 1 && temp != (wchar_t)'>')
                    tag += temp;
                if (temp != (wchar_t)'>')
                    tagchar += temp;
                if (tagchar.indexOf(L"!--") != -1)
                {
                    explain = 1;
                }
            }
            while (explain == 1)
            {
                input >> temp;
                tagchar += temp;
                if (tagchar.indexOf(L"-->") != -1)
                    explain = 0;
            }
            t1.concat(tag);
            //wcout << tag.data << L" " << tagchar.data << endl;
            if (tag == L"!DOCTYPE")
            {
                tag = L"";
                tagchar = L"";
                continue;
            }
            else if (tag == L"meta" || tag == L"img" || tag == L"input" || tag == L"link" || tag == L"base" || tag == L"br" || tag == L"hr" || tag == L"option")
            {
                tag = L"";
                tagchar = L"";
                continue;
            }
            else if (tag == L"p" || tag == L"/p")
            {
                if (tag == L"p")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"p")
                        {
                            tags.pop();
                        }
                    }
                    if (tagchar == L"p")
                    {
                        tags.push(tag);
                        getflag = 1;
                    }
                    else
                    {
                        tags.push(L"p1");
                    }
                }
                if (tag == L"/p")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"p" || tags.top() == L"p1")
                        {
                            tags.pop();
                            if (tempResult != L"用微信扫码二维码"&&tempResult != L"分享至好友和朋友圈")
                                result += tempResult;
                            tempResult = L"";
                        }
                    }
                }
            }
            else if (tag == L"font" || tag == L"/font")
            {
                if (tag == L"font")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"font")
                        {
                            tags.pop();
                        }
                    }
                    tags.push(tag);
                    if (tagchar == L"font")
                        getflag = 1;
                }
                if (tag == L"/font")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"font")
                        {
                            tags.pop();
                            result += tempResult;
                            tempResult = L"";
                        }
                    }
                }
            }
            else if (tag == L"a" || tag == L"/a")
            {
                if (tag == L"a")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"p")
                        {
                            getflag = 1;
                        }
                    }
                    tags.push(tag);
                }
                if (tag == L"/a")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"a")
                        {
                            tags.pop();
                            if (!tags.empty())
                            {
                                if (tags.top() == L"p")
                                {
                                    getflag = 1;
                                }
                            }
                        }
                    }
                    if (getsource == 1)
                        getsource = 0;
                }
            }
            else if (tag == L"div" || tag == L"/div")
            {
                if (tag == L"div")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"p" || tags.top() == L"p1")
                        {
                            tags.pop();
                        }
                    }
                    tags.push(tag);
                    if (tagchar.indexOf("class=\"post_time_source\"") != -1|| tagchar.indexOf("class=\"ep-time-soure") != -1)
                    {
                        getsource = 1;
                        int blankflag = 0;
                        while (1)
                        {
                            input >> temp;
                            if (isBlank(temp))
                            {
                                if (blankflag == 0)
                                {
                                    continue;
                                }
                                else if (blankflag == 1)
                                {
                                    blankflag = 2;
                                }
                                else if (blankflag == 2)
                                {
                                    break;
                                }
                            }
                            else if (blankflag == 0)
                            {
                                blankflag = 1;
                            }
                            time += temp;
                        }
                    }
                }
                if (tag == L"/div")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"div")
                        {
                            tags.pop();
                        }
                    }
                }
            }
            else if (tag == L"title" || tag == L"/title")
            {
                if (tag == L"title")
                {
                    tags.push(tag);
                    gettitle = 1;
                }
                if (tag == L"/title")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"title")
                        {
                            tags.pop();
                            gettitle = 0;
                        }
                    }
                }
            }
            else if (tag == L"span" || tag == L"/span")
            {
                if (tag == L"span")
                {
                    tags.push(tag);
                    if (tagchar.indexOf("class=\"ptime\"") != -1)
                    {
                        gettime = 1;
                    }
                }
                if (tag == L"/span")
                {
                    if (!tags.empty())
                    {
                        if (tags.top() == L"span")
                        {
                            tags.pop();
                            gettime = 0;
                        }
                    }
                }
            }
            else if (tagchar.indexOf(t1) != -1)
            {
                t1 = "/";
                tag = L"";
                tagchar = L"";
                continue;
            }
            else if (tagchar[tagchar.getSize() - 1] == (wchar_t)'/')
            {
                t1 = "/";
                tag = L"";
                tagchar = L"";
                continue;
            }
            else if (tag[0] == L'/')
            {
                if (!tags.empty())
                {
                    if (tags.top() == tag.substring(1, tag.getSize() - 1))
                    {
                        tags.pop();
                        if (!tags.empty())
                        {
                            if (tags.top() == L"p")
                            {
                                getflag = 1;
                            }
                        }
                        t1 = "/";
                        tag = L"";
                        tagchar = L"";
                        continue;
                    }
                }
            }
            else
            {
                tags.push(tag);
                t1 = "/";
                tag = L"";
                tagchar = L"";
                continue;
            }
            continue;
        }
        if (!tags.empty())
        {
            if ((tags.top() == L"p" || tags.top() == L"a" || tags.top() == L"font") && getflag == 1)
            {
                tempResult += temp;
            }
            if (getsource == 1)
            {
                source += temp;
            }
            if (gettitle == 1)
            {
                title += temp;
            }
            if (gettime == 1)
            {
                time += temp;
            }
        }
    }
    input.close();

    wofstream output;
    int j = filename.indexOf(L".");
    int i;
    for (i = j; i >= 0; i--)
    {
        if (filename[i] == (wchar_t)'\\')
            break;
    }
    CharString name = filename.substring(i + 1, j - i - 1);
    char path[128];
    _getcwd(path, 128);
    CharString filepath = path;
    filepath += "\\output";
    filepath += "\\";
    filepath += name;
    filepath += L"_info.txt";
    wcout << filepath.data << endl;
    output.open(filepath.data);
    //free(filepath.data);
    if (!output)
    {
        cout << "cannot open file" << endl;
    }
    //int cut = title.indexOf(L"_网易");
    output << title.data << endl;
    output << time.data << endl;
    output << source.data << endl;
    output << result.data << endl;
    output.close();
    return result;
}*/
