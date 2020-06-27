/*
    文件名：PathHandle.h
    描述：定义文件处理的函数，还有当前目录这个全局变量
*/

#include <stdbool.h>
#include <stdio.h>

#include "SocketHandle.h"

#ifndef PATHHANDLE_H
#define PATHHANDLE_H



//文件传输信息
struct FileInfo
{
    int ControlID;      //控制连接ID
    int ConnectionID;   //数据连接ID
    int StartPoint;     //开始位置
    char* WorkingPlace; //当前工作位置
    char* FileName;     //文件目录和名称
};


/*
描述：初始化文件信息
参数：连接ID， 开始位置，当前目录，文件名
返回：文件信息
*/
extern struct FileInfo* InitFileInfo(int ControlID, int ID, int Start, char* Current, char* FileName);

/*
描述：删除文件信息
参数：文件信息
返回：无
*/
extern void DeleteFileInfo(struct FileInfo Info);

/*
描述：从路径中提取文件名
参数：路径名
返回：文件名，如果没有返回NULL
*/
extern char* GetFileName(char PathName[]);

/*
描述：获得全局目录
参数：目录名
返回：目录，失败是NULL
*/
extern char* GetFullPath(char* PathName);

/*
描述：获得相对根目录的目录
参数：目录名
返回：目录，失败是NULL
*/
extern char* GetPartPath(char* PathName);

/*
描述：新建一个文件目录，对应MKD
参数：连接，目录名
返回：成功1失败0
*/
extern bool MakePath(struct Connection* Link, char PathName[]);

/*
描述：修改当前目录，对应CWD
参数：连接，目录名
返回：成功1失败0
*/
extern bool ChangePath(struct Connection* Link, char PathName[]);


/*
描述：修改当前目录到根目录，对应CUDP
参数：无
返回：成功1失败0
*/
extern bool ChangeToRoot(struct Connection* Link);



/*
描述：删除当前目录，对应RWD
参数：连接，目录名
返回：成功1失败0
*/
extern bool DeletePath(struct Connection* Link, char PathName[]);

/*
描述：输出当前目录，对应PWD
参数：连接
返回：目录
*/
extern char* PrintPath(struct Connection* Link);

/*
描述：选择当前目录，并且把它重命名为原名，看是否能成功重命名
参数：连接，目录名
返回：成功1失败0
*/
extern bool SelectPath(struct Connection* Link, char PathName[]);

/*
描述：选择当前目录，并且把它重命名，看是否能成功重命名
参数：连接，新名字
返回：成功1失败0
*/
extern bool RenamePath(struct Connection* Link, char PathName[]);


/*
描述：输出当前目录下所有文件或单一文件信息，对应LIST
参数：链接，目录/文件名
返回：信息
*/
extern char* PrintFileList(struct Connection* Link, char* PathName);

/*
描述：打开文件
参数：要求的工作路径，文件名
返回：文件指针，错误返回NULL
*/
extern FILE* OpenFile(char* WorkingPlace, char* FileName);

/*
描述：发送文件
参数：文件发送信息
返回：无
*/
extern void* SendFile(void* Info);

/*
描述：接收文件，保存在新文件里
参数：文件发送信息
返回：无
*/
extern void* ReceiveFileNew(void* Info);

/*
描述：接收文件，保存在旧文件后面
参数：文件发送信息
返回：无
*/
extern void* ReceiveFileAppend(void* Info);
#endif