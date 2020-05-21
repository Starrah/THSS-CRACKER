#ifndef CLIENT
#define CLIENT

#include <sys/socket.h>

#include <unistd.h>

#include "global.h"

struct ClientInfo {
    int active;     // specify if the client is still online
    int connfd;     // the connection stablish for info communicate
                    // not for file transfer

    int file_fd;    // socket fd for file transfering

    int port;       // the port of socket for file transfer
                    // Need to save beacase after calling PASV/PORT
                    // the connection will not establish until
                    // STOR/RETR is called
    struct sockaddr_in addr; // For saving addr info

    enum State state;     // client's connecting infomation

    int passive;    // Connect with PASV/PORT

	int rest_len;  // the length of file transfered
};

void init_client_infos(struct ClientInfo* set, int len) {
    /*
    init client info array
    */
    int i;
    for (i = 0; i < len; i++) {
        set[i].active = 0;
        set[i].connfd = -1;
        set[i].file_fd = -1;
        set[i].port = -1;
        set[i].state = start;
        set[i].passive = 0;
		set[i].rest_len = 0;
    }
}

int save_client_infos(struct ClientInfo* set, int len, int connfd) {
    /*
    save client infos to set, 
    return -1 if the connection is full, 0 for succeed
    */
   int i;
   int saved = -1;
   for (i = 0; i < len; i++) {
       if (set[i].active == 0) {
           set[i].active = 1;
           set[i].connfd = connfd;
           set[i].state = start;

           saved = 0; // set flag 0
           break;
       }
   }

   return saved;
}

void log_out_client(struct ClientInfo* set, int idx) {
    /*
    Set invalid the client in set with idx
    */
    set[idx].active = 0;
    if (set[idx].connfd != -1) {
        close(set[idx].connfd);
    }

    if (set[idx].file_fd != -1) {
        close(set[idx].file_fd);
    } 

    set[idx].state = start;
}

void close_file_fd(struct ClientInfo* set, int idx) {
    if (set[idx].file_fd != -1)
        close(set[idx].file_fd);
    set[idx].file_fd = -1;
    set[idx].state = pass;
}

#endif