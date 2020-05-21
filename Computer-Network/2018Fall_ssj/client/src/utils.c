#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "global.h"

int is_valid_command(char* command) {
	/*
	Return 1 if the command is valid, 0 otherwise
	*/
	char verb[5];
	int blank_idx;
	int i;
	int len_command = strlen(command);

	for (blank_idx = 0; blank_idx < len_command; blank_idx++) {
		if (command[blank_idx] == ' ' || command[blank_idx] == '\n' || command[blank_idx] == '\r') break;
	}
	blank_idx--;

	if (command[blank_idx] != ' ') blank_idx++;

	if (blank_idx < 3 || blank_idx > 4) return 0;

	for (i = 0; i < blank_idx; i++) {
		verb[i] = command[i];
	}

	verb[blank_idx] = '\0';

	return (strcmp(verb, USER) == 0) || \
		(strcmp(verb, PASS) == 0) || \
		(strcmp(verb, PORT) == 0) || \
		(strcmp(verb, PASV) == 0) || \
		(strcmp(verb, RETR) == 0) || \
		(strcmp(verb, STOR) == 0) || \
		(strcmp(verb, SYST) == 0) || \
		(strcmp(verb, TYPE) == 0) || \
		(strcmp(verb, QUIT) == 0) || \
		(strcmp(verb, MKD) == 0) || \
		(strcmp(verb, CWD) == 0) || \
		(strcmp(verb, PWD) == 0) || \
		(strcmp(verb, LIST) == 0) || \
		(strcmp(verb, RMD) == 0) || \
		(strcmp(verb, RNFR) == 0) || \
		(strcmp(verb, RNTO) == 0) || \
		(strcmp(verb, REST) == 0) || \
		(strcmp(verb, ABRT) == 0);

}

void parse_request(char* request, char* verb, char* info) {
    /*
    For request format: `245 test`
    
    Parameters
    ---------------------------------
    code: int, status code, to save code in request
    info: char*, store end postfix
    */
    sscanf(request, "%s %s", verb, info);
}

void parse_response(char* response, int* code, char* info) {
    /*
    For response format: `245 test`
    
    Parameters
    ---------------------------------
    code: int, status code, to save code in request
    info: char*, store end postfix
    */
    sscanf(response, "%d %s", code, info);
}

void merge_verb_param(char* buff, char* verb, char* param) {
    /*
    Merge verb and param to the form:
        verb param
    to buff
    */
    sprintf(buff, "%s %s", verb, param);
}

int send_to_server(int sockfd, char* buff, int len) {
    int n = write(sockfd, buff, len);
    if (n < 0) {
        printf("Error write(): %s(%d)\n", strerror(errno), errno);
        exit(1);
    }
    return n;
}

int recv_from_server(int sockfd, char* buff, int file_cnn) {
    int p = 0;
    int n = 0;

    if (!file_cnn) {
        while (1) {
            n = read(sockfd, buff + p, BUFF_SIZE - p);
            if (n < 0) {
                printf("Error read(): %s(%d)\n", strerror(errno), errno);	//read²»±£Ö¤Ò»´Î¶ÁÍê£¬¿ÉÄÜÖÐÍ¾ÍË³ö
                return 1;
            } else if (n == 0) {
                break;
            } else {
                p += n;
                if (buff[p - 1] == '\n' || buff[p-1] == '\r') {
                    break;
                }
            }
        }
        buff[p - 1] = '\0';
        return p;
    } else {
        n = read(sockfd, buff, BUFF_SIZE);
        return n;
    }

}

void parse_addr_and_port(char* addr, char* formatted_addr, int* port) {
    /*
    Parse the form
        h1,h2,h3,h4,p1,p2
    */
    int h1, h2, h3, h4, p1, p2;
    
    sscanf(addr, "%d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2);
    sprintf(formatted_addr, "%d.%d.%d.%d", h1, h2, h3, h4);

    *port = p1 * 256 + p2;
}

int start_socket(int port, char* ip, struct sockaddr_in* addr) {
    /*
    Init socket and addr
    */ 
    int listenfd;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        return -1;
    }

    int option = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    //addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (strcmp(ip, "") == 0)
        addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        addr->sin_addr.s_addr = inet_addr(ip);

    return listenfd;
}

void get_addr_and_port(char* buff, char* info) {
    int len_buff = strlen(buff);
    int i;
    int start = 0;
    int idx = 0;
    
    for (i = 0; i < len_buff; i++) {
        if (buff[i] == '(') {
            start = 1;
            continue;
        } else if(buff[i] == ')')
            break;

        if (start) {
            info[idx++] = buff[i];
        }
    }

    info[idx] = '\0';
}
