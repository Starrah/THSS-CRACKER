/*
    文件名：SocketHandle.h
    描述：socket连接的各种函数
*/

#include <stdbool.h>

#include "RequestHandle.h"

#ifndef SOCKETHANDLE_H
#define SOCKETHANDLE_H

#define MAX_LENGTH_SOCKET 8192
#define MAX_LENGTH_DIR 300

#define TYPE_NO -1
#define TYPE_PORT 0
#define TYPE_PASV 1

//服务器连接的类
struct Connection
{
    int ControlID; //控制连接ID
    int DataID;    //数据连接ID
    int ListenID; //监听pasv的ID
    int DataPort; //此连接的数据端口
    int ConnectType; //连接类型：0：POST，1：PASV，没有：-1
    int StartPoint;     //文件传输和接收的开始位置，默认0
    char* WorkingPlace; //当前工作目录
    char* CurrentPlaceToBeChanged; //要重命名的目录
    struct ConnectPlace ClientInfo;
    bool WhetherConnected;
};

extern char* Root;
extern int ServerListenSocket, ServerConnectSocket, ClientSocket;
extern char ServerIP[100];
extern int ServerControlPort;
extern int ServerPositivePort;

/*
描述：获得连接
参数：无
返回：连接号
*/
extern int GetConnection();

/*
描述：初始化连接
参数：ID
返回：连接
*/
extern struct Connection* InitConnection(int ID);

/*
描述：删除连接
参数：连接
返回：无
*/
extern void DeleteConnection(struct Connection Link);

/*
描述：关闭连接
参数：连接ID
返回：无
*/
extern void DropConnection(int ID);

/*
描述：关闭所有连接
参数：连接
返回：无
*/
extern void DropAllConnections(struct Connection* Link);

/*
描述：接收请求
参数：连接，接收请求的指针，最大长度
返回：成功1失败0
*/
extern bool ReadMessage(int Connection, char* Data, int MaxLen);

/*
描述：发送请求
参数：连接，待发送的指针，长度
返回：成功1失败0
*/
extern bool WriteMessage(int Connection, char* Data, int Length);

/*
描述：接收数据
参数：连接，接收请求的指针，最大长度
返回：数据长度，失败返回-1
*/
extern int ReadData(int Connection, char* Data, int MaxLen);

/*
描述：接受Port连接
参数：连接，IP和端口
返回：成功1失败0
*/
extern bool AcceptPORT(struct Connection* Link, struct ConnectPlace Place);

/*
描述：PORT连接
参数：连接，IP和端口
返回：成功1失败0
*/
extern bool ConnectPORT(struct Connection* Link);

/*
描述：接收PASV连接
参数：连接
返回：成功:返回IP端口格式字符串。失败：返回NULL
*/
extern char* AcceptPASV(struct Connection* Link);

/*
描述：进行PASV连接
参数：连接
返回：成功1失败0
*/
extern bool ConnectPASV(struct Connection* Link);

/*
描述：随机生成一个端口
参数：无
返回：端口
*/
extern int GetRandomPort();

#endif 