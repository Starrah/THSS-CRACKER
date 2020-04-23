#include "handle.h"


int serverSocket;
void handler(int x)
{
    printf("exit\n");
    close(serverSocket);
    exit(0);
}

int main(int argc, char *argv[])
{
    int port = 21;
    char root[BUFFER_SIZE] = "/tmp";
    if(argc >= 3)
    {
        if(strcmp(argv[1],"-port") == 0)
        {
            sscanf(argv[2], "%d", &port);
        }else if(strcmp(argv[1], "-root") == 0)
        {
            sscanf(argv[2], "%s", root);
        }
    }
    if(argc == 5)
    {
        if(strcmp(argv[3],"-port") == 0)
        {
            sscanf(argv[4], "%d", &port);
        }else if(strcmp(argv[3], "-root") == 0)
        {
            sscanf(argv[4], "%s", root);
        }
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[BUFFER_SIZE];
    int iDataNum;
    int fpid;      // fpid指fork返回的子程序的pid

    //socket函数，失败返回-1
    if((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("socket");
        return 1;
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);//让端口释放后立即就可以被再次使用

    if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect1");
        return 1;
    }

    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    while(1)
    {
        //调用accept函数后，会进入阻塞状态
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }

        fpid = fork();    // fork函数拷贝当前进程副本 用于处理多个client

        if(fpid<0)
        {
            close(serverSocket);
            close(client);
            fprintf(stderr, "Create child handle failed");
            exit(EXIT_FAILURE);
        }else if(fpid == 0)   //当前进程是刚创建的子进程
        {
            State *state = malloc(sizeof(State));
            initState(state);
            strcpy(state->root,root);
            close(serverSocket);

            send(client, "220 ftp.ssast.org FTP server ready.\r\n", 37, 0);
            while((iDataNum = recv(client, buffer, BUFFER_SIZE, 0))>0)
            {
                buffer[iDataNum] = '\0';
                handle(buffer, client, state);
            }
            close(client);
            close(state->psv_socket);
            free(state);
            exit(0);
        }else   //当前进程是父进程
        {
            close(client);    //关闭父进程 使其跳出阻塞进入等待accept的状态
        }
    }
    close(serverSocket);
}