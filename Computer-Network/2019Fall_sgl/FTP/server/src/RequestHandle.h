/*
    文件名：RequestHandle.h
    描述：定义FTPRequest类， 编码request指令，定义request切分函数
*/

#ifndef REQUESTHANDLE_H
#define REQUESTHANDLE_H

#include <string.h>
#include <stdbool.h>


//请求种类定义
#define USER 0
#define PASS 1
#define PORT 10
#define PASV 11
#define RETR 20
#define STOR 21
#define SYST 30
#define TYPE 31
#define QUIT 32
#define ABOR 33
#define REST 34
#define MKD 40
#define CWD 41
#define PWD 42
#define RMD 43
#define CDUP 44
#define RNFR 45
#define RNTO 46
#define LIST 50


//请求：请求种类和参数
struct FTPRequest
{
    int RequestType;    //请求种类，在上方定义了
    char* Parameter;    //请求参数，如果没有就是空

};

//连接：ip和端口
struct ConnectPlace
{
    char* IP;
    int Port;
};

/*
功能：判断请求头是否合法
参数：请求头四个字母
返回：失败-1,其余返回请求码
*/
extern int JudgeRequestHeader(char Header0, char Header1, char Header2, char Header3);

/*
功能：判断请求头是否合法（长度3）
参数：请求头3个字母
返回：失败-1,其余返回请求码
*/
extern int JudgeRequestHeaderLength3(char Header0, char Header1, char Header2);

/*
功能：初始化FTP请求
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
extern struct FTPRequest InitializeRequest(char* RawRequest);

/*
功能：初始化FTP请求(长度4)
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
extern struct FTPRequest InitializeRequestLength4(char* RawRequest);

/*
功能：初始化FTP请求(长度3)
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
extern struct FTPRequest InitializeRequestLength3(char* RawRequest);

/*
功能：删除FTP请求
参数：FTP请求
返回：无
*/
extern void DeleteRequest(struct FTPRequest Request);

/*
功能：将一个数字字符串转化为数字
参数：字符串
返回：数字，如果不是数字，返回-1
*/
int SwitchToInt(char* Str);

/*
功能：拷贝链接信息
返回：新信息
*/
extern struct ConnectPlace CopyPlace(const struct ConnectPlace Place);

/*
功能：删除链接信息
返回：无
*/
void DeletePlace(struct ConnectPlace Place);

/*
功能：解析IP端口
参数：请求的参数字符串
返回：IP端口
*/
extern struct ConnectPlace GetIPAndPort(char* Parameter);

/*
功能：将一个数字字符串转化为IP和端口的数字
参数：字符串
返回：数字，如果不合法，返回-1
*/
extern int SwitchToIntIP(char* Str);

/*
描述：把IP地址的三位正整数转化为字符串
参数：正整数
返回：字符串
*/
extern char* IntToString(int a);

/*
描述：把IP地址转化为字符串
参数：IP的4个正整数
返回：字符串
*/
extern char* IPListToString(int IP1, int IP2, int IP3, int IP4);

/*
描述：把IP地址字符串和端口数转化为输出标准格式
参数：IP地址，端口
返回：字符串
*/
extern char* IPPortToReturnString(char IP[], int Port);

#endif
