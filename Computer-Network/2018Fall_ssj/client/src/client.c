#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "global.h"
#include "utils.h"

#define DEBUG

int main(int argc, char **argv) {
    // For init socket connection
    int sockfd;
    int listenfd = -1;
    int connfd = -1;
    struct sockaddr_in addr;
    char buff[BUFF_SIZE];

    char input_buff[BUFF_SIZE];
    char verb[5];
    char info[BUFF_SIZE];

    int code;

    // Response Lenght
    int len_response;
    int read_len;

    // Basic config
    int port = 21;

    int passive = 0;
    int i;
    int merge = 0;
    int tmp_len;
	int rest_len = 0;

    // Record status
    int has_build_conn = 0;


    // For file reading and wring
    FILE* fp;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

#ifdef DEBUG
    printf("Client Started on port: %d\n", port);
#endif

    // The server will response with welcome msg
    recv_from_server(sockfd, buff, 0);
    printf("<<<Server: %s\n", buff);

    while(1) {
        printf(">>>Client: ");
        fgets(input_buff, BUFF_SIZE, stdin);
        send_to_server(sockfd, input_buff, strlen(input_buff));

        parse_request(input_buff, verb, info);
        if (strcmp(verb, PORT) == 0) {
            if (listenfd != -1) {
                close(listenfd);
                listenfd = -1;
            }
            passive = 0;
            parse_addr_and_port(info, input_buff, &port);

            // create listening socket for receiving file
            listenfd = start_socket(port, input_buff, &addr);
            if (listenfd == -1) {
                return 1;
            }
            if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                printf("Error bind(): %s(%d)\n", strerror(errno), errno);
                return 1;
            }
            if (listen(listenfd, MAX_LISTEN) == -1) {
                printf("Error listen(): %s(%d)\n", strerror(errno), errno);
                return 1;
            }

            has_build_conn = 1;

        } 
		else if(strcmp(verb, RETR) == 0) {
            // Initial mark, to know the status
            // connectting to server
            if (has_build_conn) {
                if (!passive) {
                    if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
                        printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    }
                } else {
                    if (connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    } else {
                        printf("connect \n");
                    }
                }
            }

            recv_from_server(sockfd, buff, 0);
            printf("<<<Server: %s\n", buff);
            tmp_len = strlen(buff);
            if (tmp_len > 3) {
                for (i = 0; i < tmp_len-3; i++) {
                    if (buff[i] == '2' && buff[i+1] == '2' && buff[i+2] == '6') {
                        merge = 1;
                        break;
                    }
                }
            }


            parse_response(buff, &code, input_buff);

            if (code != 150)
                continue;


            has_build_conn = 0;
            // start writing

			if (rest_len == 0) {
				fp = fopen(info, "wb");
				while ((len_response = recv_from_server(connfd, buff, 1))) {
					fwrite(buff, sizeof(char), len_response, fp);
				}
				fclose(fp);
			}
			else {
				fp = fopen(info, "ab");
				while ((len_response = recv_from_server(connfd, buff, 1))) {
					fwrite(buff, sizeof(char), len_response, fp);
				}
				rest_len = 0;
				fclose(fp);
			}

            if (listenfd != -1) {
                close(listenfd);
                listenfd = -1;
            }
            if (connfd != -1) {
                close(connfd);
                connfd = -1;
            }

            
        } 
		else if(strcmp(verb, STOR) == 0) {
            // connectting to server
            if (has_build_conn) {
                if (!passive) {
                    if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
                        printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    }
                } else {
                    if (connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    }
                }
            }

            // Initial mark, to know the status
            recv_from_server(sockfd, buff, 0);
            printf("<<<Server: %s\n", buff);
            tmp_len = strlen(buff);
            if (tmp_len > 3) {
                for (i = 0; i < tmp_len-3; i++) {
                    if (buff[i] == '2' && buff[i+1] == '2' && buff[i+2] == '6') {
                        merge = 1;
                        break;
                    }
                }
            }

            parse_response(buff, &code, input_buff);

            if (code != 150)
                continue;

            has_build_conn = 0;
            // start writing

			if (rest_len == 0) {
				fp = fopen(info, "rb");
				if (fp == NULL) {
					printf("File doesn't exist, please check the file");
				}
				else {
					while ((read_len = fread(buff, sizeof(char), BUFF_SIZE, fp)))
						send_to_server(connfd, buff, read_len);
					fclose(fp);
				}
			}
			else {
				fp = fopen(info, "rb");
				if (fp == NULL) {
					printf("File doesn't exist, please check the file");
				}
				else {
					fseek(fp, rest_len, SEEK_SET);
					while ((read_len = fread(buff, sizeof(char), BUFF_SIZE, fp)))
						send_to_server(connfd, buff, read_len);
					fclose(fp);
				}
			}

            if (listenfd != -1) {
                close(listenfd);
                listenfd = -1;
            }
            if (connfd != -1) {
                close(connfd);
                connfd = -1;
            }

            
        } 
		else if(strcmp(verb, LIST) == 0) {
            // Initial mark, to know the status
            if (has_build_conn) {
                if (!passive) {
                    if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
                        printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    }
                } else {
                    if (connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
                        return 1;
                    }
                }
            }

            recv_from_server(connfd, buff, 0);
            printf("<<<Server: %s\n", buff);
            has_build_conn = 0;

            if (connfd != -1) {
                close(connfd);
                connfd = -1;
            }
            recv_from_server(sockfd, buff, 0);
            printf("<<<Server: %s\n", buff);
			merge = 1;
        }
		else if(strcmp(verb, REST) == 0){
			recv_from_server(sockfd, buff, 0);
			printf("<<<Server: %s\n", buff);
			tmp_len = strlen(buff);
			if (tmp_len > 3) {
				for (i = 0; i < tmp_len - 3; i++) {
					if (buff[i] == '3' && buff[i + 1] == '5' && buff[i + 2] == '0') {
						rest_len = atoi(info);
						merge = 1;
						break;
					}
				}
			}
		}

        if (!merge) {
            recv_from_server(sockfd, buff, 0);
            printf("<<<Server: %s\n", buff);
            merge = 0;
        }

        if (strcmp(verb, PASV) == 0) {
            has_build_conn = 1;

            // PASV
            if (connfd != -1) {
                close(connfd);
                connfd = -1;
            }
            get_addr_and_port(buff, input_buff);
            parse_addr_and_port(input_buff, info, &port);

            connfd = start_socket(port, info, &addr);
            passive = 1;
        }

        if (strcmp(verb, QUIT) == 0 || strcmp(verb, ABRT) == 0) break;
    }

    close(sockfd);
    if (listenfd != -1) close(listenfd);
    return 0;
}
