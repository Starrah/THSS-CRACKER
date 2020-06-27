/*
    文件名：StatusChange.c
    描述：修改后端服务器状态的函数，每个函数的输入都是处理好的命令，负责状态转换。
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#include "RequestHandle.h"
#include "SocketHandle.h"
#include "StatusChange.h"
#include "PathHandle.h"




void PrintPlace(struct Connection* Link)
{
    if(Link -> WorkingPlace == NULL)
    {
        printf("Current Working Place is NULL\n");
    }
    else
    {
        printf("Current Working Place is %s\n",Link->WorkingPlace);
    }
    if(Link -> CurrentPlaceToBeChanged == NULL)
    {
        printf("Current Place to be Changed is NULL\n");
    }
    else
    {
        printf("Current Place to be Changed is %s\n", Link->CurrentPlaceToBeChanged);
    }
}


/*
描述：初始化服务器
参数：无
返回：成功1失败0
*/
bool InitServer()
{

	struct sockaddr_in Address;
	//在21号端口创立socket
	if ((ServerListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//设置ip和端口
	memset(&Address, 0, sizeof(Address));
	Address.sin_family = AF_INET;
	Address.sin_port = htons(ServerControlPort);
	if (inet_pton(AF_INET, ServerIP, &Address.sin_addr) <= 0) {			
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//设置21号socket
	if (bind(ServerListenSocket, (struct sockaddr*)&Address, sizeof(Address)) == -1) {
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//监听21号socket
	if (listen(ServerListenSocket, 10) == -1) {
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

}

/*
描述：获取控制台参数，来得到Root和端口信息
参数：参数个数，参数
返回：成功1失败0
*/
bool HandleArguments(int argc, char* argv[])
{
    char PortName[10] = "-port";
    char RootName[10] = "-root";
    char Tmp[50] = "/tmp";
    if(argc == 1)
    {
        //没有参数
        ServerControlPort = 21;
        Root = (char*)malloc(sizeof(char)*strlen(Tmp));
	    strcpy(Root, Tmp);
        return 1;
    }
    else if(argc == 3)
    {
        //有root或者port
        if(strcmp(PortName, argv[1]) == 0)
        {
            int port = atoi(argv[2]);
            if(port >= 0 && port < 65536)
            {
                ServerControlPort = port;
                Root = (char*)malloc(sizeof(char)*strlen(Tmp));
	            strcpy(Root, Tmp);
                return 1;
            }
        }
        else if(strcmp(RootName, argv[1]) == 0)
        {
            if(argv[2] != NULL)
            {
                Root = (char*)malloc(sizeof(char) * strlen(argv[2]));
                strcpy(Root, argv[2]);
                ServerControlPort = 21;
                return 1;
            }
        }
    }
    else if(argc == 5)
    {
        if(strcmp(PortName, argv[1]) == 0 && strcmp(RootName, argv[3]) == 0)
        {
            int port = atoi(argv[2]);
            if(port >= 0 && port < 65536 && argv[4] != NULL)
            {
                ServerControlPort = port;
                Root = (char*)malloc(sizeof(char) * strlen(argv[4]));
                strcpy(Root, argv[4]);
                return 1;
            }
        }
        else if(strcmp(RootName, argv[1]) == 0 && strcmp(PortName, argv[3]) == 0)
        {
            int port = atoi(argv[4]);
            if(port >= 0 && port < 65536 && argv[2] != NULL)
            {
                ServerControlPort = port;
                Root = (char*)malloc(sizeof(char) * strlen(argv[2]));
                strcpy(Root, argv[2]);
                return 1;
            }
        }
    }
    return 0;
}

/*
描述：控制处理客户端请求的主函数
参数：ID指针
返回: 无
*/
void* HandleClient(void* ID)
{
    struct Connection* Link = InitConnection(*(int*)ID);
    HandleLogin(Link);
    HandleCommand(Link);
}


/*
描述：处理登录请求
参数：连接
返回：无
*/
void HandleLogin(struct Connection* Link)
{
    char* CommandString = (char*)malloc(sizeof(char) * MAX_LENGTH_SOCKET);


    while (1)
    {
        //读取用户名
        memset(CommandString, 0, MAX_LENGTH_SOCKET);
		ReadMessage(Link -> ControlID, CommandString, MAX_LENGTH_SOCKET);
        printf("%s\n", CommandString);
        struct FTPRequest Command = InitializeRequest(CommandString);
        char User[100] = "anonymous";
        //printf("%s\n", Command.Parameter);
        if(Command.RequestType == USER && Command.Parameter != NULL && strcmp(Command.Parameter, User) == 0)
        {
            printf("The Link %d has given the userID\n", Link -> ControlID);
            char ReturnString[300] = "331 Guest login ok, send your complete e-mail address as password.\r\n";
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
            break;
        }
        else if(Command.RequestType != USER)
        {
            char ReturnString[300] = "500 Need User!\r\n";
            printf("Error Username(): %s(%d)\n", strerror(errno), errno);
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
			continue;
        }
        else
        {
            char ReturnString[300] = "500 User need to be anonymous!\r\n";
            printf("Error Username(): %s(%d)\n", strerror(errno), errno);
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
			continue;
        }
    }


    while(1)
    {
        //读取密码
        memset(CommandString, 0, MAX_LENGTH_SOCKET);
		ReadMessage(Link -> ControlID, CommandString, MAX_LENGTH_SOCKET);
        printf("%s\n", CommandString);
        struct FTPRequest Command = InitializeRequest(CommandString);
        //if(Command.RequestType == PASS && Command.Parameter != NULL && strlen(Command.Parameter) > 0)
        if(Command.RequestType == PASS)
        {
            printf("The Link %d has login Successfully\n", Link -> ControlID);
            char ReturnString[300] = "230 Login Successful\r\n";
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
            break;
        }
        else if(Command.RequestType != PASS)
        {
            printf("Error Password(): %s(%d)\n", strerror(errno), errno);
            char ReturnString[300] = "500 Need Password\r\n";
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
			continue;
        }
        else
        {
            printf("Error Password(): %s(%d)\n", strerror(errno), errno);
            char ReturnString[300] = "500 Password Error\r\n";
            DeleteRequest(Command);
            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
			continue;
        }
    }
    free(CommandString);
	//close(ServerConnectSocket);
	
}

/*
描述：处理除了登录外的请求
参数：连接,类型
返回：无
*/
void HandleCommand(struct Connection* Link)
{
    char* CommandString = (char*)malloc(sizeof(char) * MAX_LENGTH_SOCKET);
    memset(CommandString, 0, sizeof(char) * MAX_LENGTH_SOCKET);

       
        while (1)
        {
            //读取用户名
            memset(CommandString, 0, sizeof(char) * MAX_LENGTH_SOCKET);
		    ReadMessage(Link -> ControlID, CommandString, MAX_LENGTH_SOCKET);
            //printf("%s", CommandString);
            //PrintPlace(Link);
            struct FTPRequest Command = InitializeRequest(CommandString);
            //printf("%d\n",Command.RequestType);

            //PORT模式
            if(Command.RequestType == PORT)
            {
                
                if(Command.Parameter != NULL && strlen(Command.Parameter) > 0)
                {
                    printf("%s\n",Command.Parameter);
                    struct ConnectPlace Place = GetIPAndPort(Command.Parameter);
                    if(Place.IP != NULL)
                    {
                        //连接PORT
                        if(AcceptPORT(Link, Place))
                        {
                            char ReturnString[300] = "200 PORT command successful.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                            Link -> WhetherConnected = 0;
                        }
                        else
                        {
                            char ReturnString[300] = "425 PORT command faliure.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                        }
                        DeletePlace(Place);
                    }
                }
                else
                {
                    char ReturnString[300] = "425 PORT command faliure.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            //PASV模式
            else if(Command.RequestType == PASV)
            {
                //连接PASV
                char* ReturnData = AcceptPASV(Link);
                if(ReturnData != NULL)
                {
                    printf("%s\n", ReturnData);
                    char ReturnString[300] = "227 Entering Passive Mode (";
                    int LenData = strlen(ReturnData);
                    int LenReturn = strlen(ReturnString);
                    for(int i = 0; i < LenData; i ++)
                    {
                        ReturnString[LenReturn] = ReturnData[i];
                        LenReturn ++;
                    }
                    ReturnString[LenReturn] = ')';
                    LenReturn ++;
                    ReturnString[LenReturn] = '\r';
                    LenReturn ++;
                    ReturnString[LenReturn] = '\n';
                    LenReturn ++;
                    free(ReturnData);
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    Link -> WhetherConnected = 0;
                }
                else
                {
                    char ReturnString[300] = "425 PASV command faliure.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            //下载文件
            else if(Command.RequestType == RETR)
            {
                printf("%s\n",Command.Parameter);
                if(Link -> ConnectType == TYPE_PASV)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPASV(Link))
                    {
                        struct FileInfo* Info = InitFileInfo(Link -> ControlID, Link -> DataID, Link -> StartPoint, Link -> WorkingPlace, Command.Parameter);
                        DropConnection(Link -> ListenID);
                        Link -> ConnectType = TYPE_NO;
                        Link -> StartPoint = 0;
                        Link -> WhetherConnected = 0;
                        pthread_t NewLinkThread;
                        pthread_create(&NewLinkThread, NULL, SendFile, (void*)Info);
                        pthread_detach(NewLinkThread);
                    }
                    else
                    {
                        char ReturnString[300] = "426 PASV Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else if(Link -> ConnectType == TYPE_PORT)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPORT(Link))
                    {
                        struct FileInfo* Info = InitFileInfo(Link -> ControlID, Link -> DataID, Link -> StartPoint, Link -> WorkingPlace, Command.Parameter);
                        DropConnection(Link -> ListenID);
                        Link -> ConnectType = TYPE_NO;
                        Link -> StartPoint = 0;
                        Link -> WhetherConnected = 0;
                        pthread_t NewLinkThread;
                        pthread_create(&NewLinkThread, NULL, SendFile, (void*)Info);
                        pthread_detach(NewLinkThread);

                    }
                    else
                    {
                        char ReturnString[300] = "426 PORT Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else
                {
                    char ReturnString[300] = "425 No Data Connection.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            //上传文件
            else if(Command.RequestType == STOR)
            {
                printf("%s\n",Command.Parameter);
                if(Link -> ConnectType == TYPE_PASV)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPASV(Link))
                    {
                        struct FileInfo* Info = InitFileInfo(Link -> ControlID, Link -> DataID, Link -> StartPoint, Link -> WorkingPlace, Command.Parameter);
                        DropConnection(Link -> ListenID);
                        Link -> ConnectType = TYPE_NO;
                        Link -> StartPoint = 0;
                        Link -> WhetherConnected = 0;
                        pthread_t NewLinkThread;
                        pthread_create(&NewLinkThread, NULL, ReceiveFileAppend, (void*)Info);
                        pthread_detach(NewLinkThread);
                    }
                    else
                    {
                        char ReturnString[300] = "426 PASV Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else if(Link -> ConnectType == TYPE_PORT)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPORT(Link))
                    {
                        struct FileInfo* Info = InitFileInfo(Link -> ControlID, Link -> DataID, Link -> StartPoint, Link -> WorkingPlace, Command.Parameter);
                        DropConnection(Link -> ListenID);
                        Link -> ConnectType = TYPE_NO;
                        Link -> StartPoint = 0;
                        Link -> WhetherConnected = 0;
                        pthread_t NewLinkThread;
                        pthread_create(&NewLinkThread, NULL, ReceiveFileAppend, (void*)Info);
                        pthread_detach(NewLinkThread);

                    }
                    else
                    {
                        char ReturnString[300] = "426 PORT Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else
                {
                    char ReturnString[300] = "425 No Data Connection.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            //断点续传
            else if(Command.RequestType == REST)
            {
                if(Link -> ConnectType != TYPE_PASV && Link -> ConnectType != TYPE_PORT)
                {
                    char ReturnString[300] = "425 No Connection\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    continue;
                }
                int StartPoint = SwitchToInt(Command.Parameter);
                if(StartPoint == -1)
                {
                    char ReturnString[300] = "425 Invalid Parameter\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    continue;
                }
                Link -> StartPoint = StartPoint;
                char ReturnString[300] = "226 Set StartPlace Success\r\n";
                WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
            }

            //杂指令
            else if(Command.RequestType == SYST)
            {
                char ReturnString[300] = "215 UNIX Type: L8\r\n";
                WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
            }
            else if(Command.RequestType == TYPE)
            {
                printf("%s\n",Command.Parameter);
                if(Command.Parameter != NULL && Command.Parameter[0] == 'I' && strlen(Command.Parameter) == 1)
                {
                    char ReturnString[300] = "200 Type set to I.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "425 Invalid Type\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == QUIT || Command.RequestType == ABOR)
            {
                
                char ReturnString[300] = "221 Goodbye\r\n";
                WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                DropAllConnections(Link);
                DeleteConnection(*Link);
                free(Link);
                pthread_exit(0);
            }
            else if(Command.RequestType == CWD)
            {
                if(Command.Parameter == NULL || ChangePath(Link, Command.Parameter) == 0)
                {
                    char ReturnString[300] = "421 Change Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "250 Change Working Directory Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == PWD)
            {   
                char* CurrentPlace = PrintPath(Link);
                if(CurrentPlace == NULL)
                {
                    char ReturnString[300] = "421 Printing Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    //printf("%s\n", ReturnString);
                }
                else
                {
                    char a[2] = "\"";
                    char b[2] = "\"";
                    char c[20] = "\r\n";
                    char ReturnString[400] = "257 ";
                    strcat(ReturnString, a);
                    strcat(ReturnString, CurrentPlace);
                    strcat(ReturnString, b);
                    strcat(ReturnString, c);
                    //free(CurrentPlace);
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    //printf("%s\n", ReturnString);
                    free(CurrentPlace);
                }
            }
            else if(Command.RequestType == MKD)
            {
                if(Command.Parameter == NULL || MakePath(Link, Command.Parameter) == 0)
                {
                    char ReturnString[300] = "421 Make Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "257 Make Working Directory Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == RMD)
            {
                if(Command.Parameter == NULL || DeletePath(Link, Command.Parameter) == 0)
                {
                    char ReturnString[300] = "421 Remove Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "250 Remove Working Directory Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == CDUP)
            {
                if(ChangeToRoot(Link) == 0)
                {
                    char ReturnString[300] = "421 Moving to root Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "250 Moving to root Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == RNFR)
            {
                if(Command.Parameter == NULL || SelectPath(Link, Command.Parameter) == 0)
                {
                    char ReturnString[300] = "421 Select Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "350 Select Working Directory Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }
            else if(Command.RequestType == RNTO)
            {
                if(Command.Parameter == NULL || RenamePath(Link, Command.Parameter) == 0)
                {
                    char ReturnString[300] = "421 Rename Working Directory Fail\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
                else
                {
                    char ReturnString[300] = "250 Rename Working Directory Successful\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            //传输文件信息
            else if(Command.RequestType == LIST)
            {
                if(Link -> ConnectType == TYPE_PASV)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPASV(Link))
                    {
                        char* Data = PrintFileList(Link, Command.Parameter);
                        if(Data != NULL)
                        {
                            WriteMessage(Link -> DataID, Data, strlen(Data));
                            free(Data);
                            char ReturnString[300] = "226 Sending List Success.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                        }
                        else
                        {
                            char ReturnString[300] = "451 List Fail.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                        }
                    }
                    else
                    {
                        char ReturnString[300] = "426 PASV Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else if(Link -> ConnectType == TYPE_PORT)
                {
                    char ReturnString1[300] = "150 Start Transporting\r\n";
                    WriteMessage(Link -> ControlID, ReturnString1, strlen(ReturnString1));
                    if(ConnectPORT(Link))
                    {
                        char* Data = PrintFileList(Link, Command.Parameter);
                        if(Data != NULL)
                        {
                            WriteMessage(Link -> DataID, Data, strlen(Data));
                            free(Data);
                            char ReturnString[300] = "226 Sending List Success.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                        }
                        else
                        {
                            char ReturnString[300] = "451 List Fail.\r\n";
                            WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                        }

                    }
                    else
                    {
                        char ReturnString[300] = "426 PORT Connection Failure.\r\n";
                        WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                    }
                }
                else
                {
                    char ReturnString[300] = "425 No Data Connection.\r\n";
                    WriteMessage(Link -> ControlID, ReturnString, strlen(ReturnString));
                }
            }

            DeleteRequest(Command);
        }

    free(CommandString);
}