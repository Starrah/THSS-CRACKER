//
// Created by 王世杰 on 2019/10/20.
//

#ifndef FTP_SERVER_UTIL_H
#define FTP_SERVER_UTIL_H

#define NO_MODE 0
#define PASV_MODE 1
#define PORT_MODE 2
#define STATUS_NEED_USER 0
#define STATUS_NEED_PWD 1
#define STATUS_LOGIN 2
#define BUFFER_SIZE 1000

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ifaddrs.h>

typedef struct State
{
    int tranport_mode;
    int status;
    int addr[6];
    int psv_socket;
    int rest_start;    //REST命令指定的文件起始地址
    char remote_path[200];
    char root[200];
    char rn_file[200];
    char last_cmd[200];
}State;

void initState(State* state)
{
    state->tranport_mode = NO_MODE;
    state->status = STATUS_NEED_USER;
    state->rest_start = 0;
    strcpy(state->remote_path, "/");
    strcpy(state->rn_file, "");
    strcpy(state->last_cmd, "");
}

//判断串s1是否是串s2的前缀
int isPrefix(char *little, char *big)
{
    int result = 0;
    char *s = strstr(big, little);//在big中查找little
    if(s && (s == big))
    {
        result = 1;
    }
    return result;
}

void getFileName(char* file_path, char* file_name)
{
    int len = strlen(file_path);
    int start;
    for(start = len-1; start>=0; start--)
    {
        if(file_path[start]=='/')
        {
            break;
        }
    }
    strncpy(file_name, file_path+start+1 , len-start-1);
}


//获取文件大小，返回文件的字节数
//如果文件不存在，则返回-1
int getFileSize(char* filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return -1;
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    fclose(fp);
    return size;
}


void dot2Comma(char *buf)
{
    int len = strlen(buf);
    int i;
    for(i = 0; i < len; i++)
    {
        if (buf[i] == '.')
        {
            buf[i] = ',';
        }
    }
}

char* getIP()
{
    struct ifaddrs *head = NULL, *iter = NULL;
    if (getifaddrs(&head) == -1)
    {
        return NULL;
    }
    for (iter = head; iter != NULL; iter = iter->ifa_next)
    {
        if (iter->ifa_addr == NULL)
        {
            continue;
        }
        if (iter->ifa_addr->sa_family != AF_INET)
        {
            continue;
        }
        char mask[INET_ADDRSTRLEN];
        void* ptr = &((struct sockaddr_in*) iter->ifa_netmask)->sin_addr;
        inet_ntop(AF_INET, ptr, mask, INET_ADDRSTRLEN);
        if (strcmp(mask, "255.0.0.0") == 0)
        {
            continue;
        }
        void* tmp = &((struct sockaddr_in *) iter->ifa_addr)->sin_addr;
        char *rlt = (char*)malloc(20);
        memset(rlt, 0, 20);
        inet_ntop(AF_INET, tmp, rlt, INET_ADDRSTRLEN);
        dot2Comma(rlt);
        return rlt;
    }
    return NULL;
}

//从网络连接接受文件
//in_fp: the local file pointer
//out_fd: socket to the client
int recvFile(FILE *in_fp, int out_fd)
{
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    int receive_count = 0;
    int length = 0;
    while(1)
    {
        usleep(2);   //限速
        receive_count = read(out_fd, buffer, BUFFER_SIZE);
        if(receive_count<=0) {break;}
        length += fwrite(buffer, sizeof(char), receive_count, in_fp);
    }
    if(receive_count < 0)
    {  //if the network is broken
        return -1;
    }else{
        return length;
    }
}

//向网络连接发送文件
//in_fp: the local file pointer
//out_fd: socket to the client
int sendFile(FILE *in_fp, int out_fd)
{
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    int send_count = 0;
    int length = 0;
    while (1)
    {
        usleep(2);   //限速
        length = fread(buffer, sizeof(char), BUFFER_SIZE, in_fp);
        if(length<=0) {break;}
        if(send(out_fd, buffer, length, 0) < 0) {return -1;}
        send_count += length;
        bzero(buffer, BUFFER_SIZE);
    }
    return send_count;
}


//PORT模式主动下连接到客户端，返回连接的套接字
int connectClient(State *state)
{
    int sockfd;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));    //设置目标主机的ip和port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(state->addr[4]*256+state->addr[5]);
    char host[BUFFER_SIZE];
    memset(host, 0, BUFFER_SIZE);
    sprintf(host, "%d.%d.%d.%d", state->addr[0], state->addr[1],state->addr[2], state->addr[3]);
    printf("client addr: %s:%d\n",host, htons(addr.sin_port));

    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0)
    {
        printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
        return -1;
    }
    //连接目标主机
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
        return -1;
    }
    return sockfd;
}

#endif //FTP_SERVER_UTIL_H
