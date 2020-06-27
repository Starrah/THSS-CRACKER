#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>

#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "SocketHandle.h"
#include "RequestHandle.h"

int ServerListenSocket;
char ServerIP[100] = "127.0.0.1";
int ServerControlPort = 21;
int ServerPositivePort = 20;
char* Root = NULL;

/*
描述：获得连接
参数：无
返回：连接ID
*/
int GetConnection()
{
	int ID;
	//char* ReturnString = (char*) malloc(sizeof(char) * MAX_LENGTH_SOCKET);
	while (1) 
    {
        //初始化
        if ((ID = accept(ServerListenSocket, NULL, NULL)) == -1) 
        {
			printf("Error accept(): %s(%d)\n", strerror(errno), errno);
			continue;
		}
        else
        {
            printf("The Link %d has Initialized\n", ID);
			char Message[100] = "220 Anonymous FTP server ready.\r\n";
			WriteMessage(ID, Message, strlen(Message));
            break;
        }
    }
	return ID;
}

/*
描述：初始化连接
参数：ID
返回：连接
*/
struct Connection* InitConnection(int ID)
{
	struct Connection* Link = (struct Connection*)malloc(sizeof(struct Connection));
	Link -> ControlID = ID;
	Link -> ListenID = -1;
	Link -> DataID = -1;
	Link -> ConnectType = -1;
	Link -> StartPoint = 0;
	Link -> WorkingPlace = (char*)malloc(sizeof(char) * MAX_LENGTH_DIR);
	Link -> WhetherConnected = 0;
	strcpy(Link->WorkingPlace, Root);
	Link -> CurrentPlaceToBeChanged = NULL;
	return Link;
}

/*
描述：删除连接
参数：连接
返回：无
*/
void DeleteConnection(struct Connection Link)
{
	if(Link.WorkingPlace != NULL)
	{
		free(Link.WorkingPlace);
	}
	if(Link.CurrentPlaceToBeChanged != NULL)
	{
		free(Link.CurrentPlaceToBeChanged);
	}
}

/*
描述：关闭连接
参数：连接ID
返回：无
*/
void DropConnection(int ID)
{
	if(ID != -1)
	{
		printf("Connection Dropped %d\n", ID);
		close(ID);
	}
}

/*
描述：关闭所有连接
参数：连接
返回：无
*/
void DropAllConnections(struct Connection* Link)
{
	DropConnection(Link -> DataID);
	DropConnection(Link -> ListenID);
	Link -> DataID = -1;
	Link -> ListenID = -1;
	Link -> ConnectType = TYPE_NO;
}



/*
描述：接收请求
参数：连接，接收请求的指针，最大长度
返回：成功1失败0
*/
bool ReadMessage(int Connection, char* Data, int MaxLen)
{
	int Pointer = 0;
	while (1) 
    {
		int Length = read(Connection, Data + Pointer, MaxLen - Pointer);
		if (Length < 0) 
        {
			printf("Error read(): %s(%d)\n", strerror(errno), errno);
			close(Connection);
			return 0;
    	} 
        else if (Length == 0) 
        {
			break;
		} 
        else 
        {
			Pointer += Length;
			if (Data[Pointer - 1] == '\n') 
            {
				break;
			}
		}
	}

	return 1;
}

/*
描述：接收数据
参数：连接，接收请求的指针，最大长度
返回：数据长度，失败返回-1
*/
int ReadData(int Connection, char* Data, int MaxLen)
{
	int Pointer = 0;
	while (1) 
    {
		int Length = read(Connection, Data + Pointer, MaxLen - Pointer);
		printf("%d\n",Length);
		if (Length < 0) 
        {
			printf("Error read(): %s(%d)\n", strerror(errno), errno);
			close(Connection);
			return -1;
    	} 
        else if (Length == 0) 
        {
			break;
		} 
        else 
        {
			Pointer += Length;
			if(Pointer >= MaxLen)
			{
				break;
			}
			if (Data[Pointer - 1] == EOF) 
            {
				break;
			}
		}
	}
	return Pointer;
}

/*
描述：发送请求
参数：连接，待发送的指针，长度
返回：成功1失败0
*/
bool WriteMessage(int Connection, char* Data, int Length)
{
	//printf("%c %c\n",Data[0],Data[1]);
    int Pointer = 0;
	while (Pointer < Length) 
    {
		int Len = write(Connection, Data + Pointer, Length - Pointer);
		if (Len < 0) 
        {
			printf("Error write(): %s(%d)\n", strerror(errno), errno);
			return 0;
	 	} 
        else 
        {
			Pointer += Len;
		}			
	}
	//printf("%d\n", Pointer);
    return 1;
}


/*
描述：接受Port连接
参数：连接，IP和端口
返回：成功1失败0
*/
bool AcceptPORT(struct Connection* Link, struct ConnectPlace Place)
{
	DropAllConnections(Link);
	Link -> ConnectType = TYPE_PORT;
	Link -> ClientInfo = CopyPlace(Place);
	return 1;
}

/*
描述：进行PORT连接
参数：连接
返回：成功1失败0
*/
bool ConnectPORT(struct Connection* Link)
{
	struct sockaddr_in addr;
	if(Link -> WhetherConnected == 1)
	{
		printf("Already Connected PORT\n");
		return 1;
	}
	if ((Link -> DataID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 0;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Link -> ClientInfo.Port);
	printf("%s:%d\n",Link -> ClientInfo.IP, Link -> ClientInfo.Port);
	if (inet_pton(AF_INET, Link -> ClientInfo.IP, &addr.sin_addr) <= 0) {			
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 0;
	}

	if (connect(Link -> DataID, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return 0;
	}
	printf("Connect PORT Success\n");
	Link -> WhetherConnected = 1;
	return 1;
}

/*
描述：接收PASV连接
参数：连接
返回：成功:返回IP端口格式字符串。失败：返回NULL
*/
char* AcceptPASV(struct Connection* Link)
{
	DropAllConnections(Link);
	Link -> ConnectType = TYPE_PASV;
	Link -> DataPort = GetRandomPort(); 
	char* Return = NULL;
	//printf("%d\n", Link -> DataPort);
	struct sockaddr_in Address;
	//创立socket
	if ((Link -> ListenID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return Return;
	}

	//设置ip和端口
	memset(&Address, 0, sizeof(Address));
	Address.sin_family = AF_INET;
	Address.sin_port = htons(Link -> DataPort);

	if (inet_pton(AF_INET, ServerIP, &Address.sin_addr) <= 0) {			
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return Return;
	}

	//设置socket
	if (bind(Link -> ListenID, (struct sockaddr*)&Address, sizeof(Address)) == -1) {
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return Return;
	}

	//监听socket
	if (listen(Link -> ListenID, 10) == -1) {
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return Return;
	}

	Return = IPPortToReturnString(ServerIP, Link -> DataPort);
	return Return;
}

/*
描述：进行PASV连接
参数：连接
返回：成功1失败0
*/
bool ConnectPASV(struct Connection* Link)
{
	if(Link -> WhetherConnected == 1)
	{
		printf("Already Connected PASV\n");
		return 1;
	}
	if((Link -> DataID = accept(Link -> ListenID, NULL, NULL)) == -1)
	{
		return 0;
	}
	printf("Connect PASV Success\n");
	Link -> WhetherConnected = 1;
	return 1;
}

/*
描述：随机生成一个端口
参数：无
返回：端口
*/
int GetRandomPort()
{
	int Port = rand() % 45536 + 20000;
	return Port;
}