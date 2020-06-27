#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>



#include "RequestHandle.h"
#include "StatusChange.h"
#include "PathHandle.h"
#include "SocketHandle.h"


int main(int argc,char *argv[])
{   
    HandleArguments(argc, argv);
    printf("Current Port is %d\n", ServerControlPort);
    printf("Current Root is %s\n", Root);
    InitServer();
    while (1) 
    {
        int NewLink = GetConnection();
        pthread_t NewLinkThread;
        pthread_create(&NewLinkThread, NULL, HandleClient, (void*)&NewLink);
        pthread_detach(NewLinkThread);
    }
    return 0;
}