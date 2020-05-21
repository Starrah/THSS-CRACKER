#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include <unistd.h>
#include <errno.h>

#include <ctype.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <dirent.h>

#include "global.h"
#include "utils.h"
#include "client_info.h"

#define DEBUG

// For socket connection
struct sockaddr_in addr;
int listenfd = -1;
int connfd = -1;
int port;

// For saving data
char root[512];
char buff[BUFF_SIZE];
char tmp_buff[BUFF_SIZE];
char selected_file[100];
DIR* dir;
struct ClientInfo cur_client;
struct ClientInfo client_set[MAX_LISTEN];

int ret;
int rest_len;

int i; // For loop
enum State cur_state, to_state;

// Tmp Data saving
int code;
char command[512];
char verb[5];
char type[3] = DEFAULT_TYPE;
int cur_connfd;

char ip[15];

char control[100];
int rs = 0;

void* threadRetr(void* th);

void* threadStor(void* th);

typedef struct
{
	char filename[256];
	int connfd;
	int filefd;
	int num;
	int rest;
}transfer;



int main(int argc, char **argv) {
    

    // Compatible with select for multi client
    fd_set read_fds, test_fds;

    srand((unsigned) time(0));

    // Getting port and root
    get_port_and_root(argv, argc, &port, root);

    // init client set
    init_client_infos(client_set, MAX_LISTEN);

    // Start socket
    listenfd = start_socket(port, "", &addr);
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

    printf("Server opened on port: %d\nwith root: %s\n", port, root);

    // set the server socket to fd_set for multi-client connection
    FD_ZERO(&read_fds);
    FD_SET(listenfd, &read_fds);
    while (1) {
        test_fds = read_fds; // Need to save read fds because 
                             // the `select` will change the fd_set

        if(select(FD_SETSIZE, &test_fds, (fd_set*)0, (fd_set*)0, (struct timeval *)0) < -1) {
            printf("Error getting select\n");
        }

        // Whether new client connect to the server
        if (FD_ISSET(listenfd, &test_fds)) {
            if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
                printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                continue;
            }

            // Still test if the line is full here, although not necessary
            FD_SET(connfd, &read_fds);
            ret = save_client_infos(client_set, MAX_LISTEN, connfd);
            if (ret == -1) {
                printf("Error save(): the line is full");
            }

            // Send welcome msg to client
            merge_code_response(buff, 220, WELCOME);
            send_to_client(connfd, buff, strlen(buff));
        }

        // Find the client who send msg
        for (i = 0; i < MAX_LISTEN; i++) {
            cur_connfd = client_set[i].connfd;
            cur_client = client_set[i];
            if (cur_client.active && \
                    FD_ISSET(cur_connfd, &test_fds)) {
                ret = 0;
                memset(buff, 0, sizeof(buff));
                memset(tmp_buff, 0, sizeof(tmp_buff));
                memset(command, 0, sizeof(command));
                memset(verb, 0, sizeof(verb));
                //current client if readible
                recv_from_client(cur_connfd, buff, 0);

                // make sure the command are valid
                if (is_valid_command(buff) == 0) {
                    merge_code_response(buff, 500, COMMAND_ERROR);
                    send_to_client(cur_client.connfd, buff, strlen(buff));
                    continue;
                }

                parse_request(buff, verb, command);

                cur_state = cur_client.state;
                if (strcmp(verb, ABRT) == 0 || strcmp(verb, QUIT) == 0) {
                    // ABOR and QUIT command have the highest priority
                    merge_code_response(buff, 221, QUIT_INFO);
                    send_to_client(cur_connfd, buff, strlen(buff));

                    log_out_client(client_set, i);
                    FD_CLR(cur_client.connfd, &read_fds);
                }
                else if (cur_state == start) {
                    // user command
                    if (strcmp(verb, USER) == 0) {
                        if (is_valid_user(command)) {
                            merge_code_response(buff, 331, LOGIN);
                            send_to_client(cur_connfd, buff, strlen(buff));
                            client_set[i].state = user;
                        } else {
                            merge_code_response(buff, 430, USER_ERROR);
                            send_to_client(cur_connfd, buff, strlen(buff));
                        }
                    }
                    else {
                        merge_code_response(buff, 530, LOGIN_REQUIRE);
                        send_to_client(cur_connfd, buff, strlen(buff));
                    }
                }
                else if(cur_state == user) {
                    // client send password
                    if (strcmp(verb, PASS) == 0) {
                        if (is_valid_pwd(command)) {
                            merge_code_response(buff, 230, VERIFIED);
                            send_to_client(cur_connfd, buff, strlen(buff));
                            client_set[i].state = pass;
                        } else {
                            merge_code_response(buff, 430, PWD_ERROR);
                            send_to_client(cur_connfd, buff, strlen(buff));
                            client_set[i].state = start;
                        }
                    } else if (strcmp(verb, USER) == 0) {
                        if (is_valid_user(command)) {
                            merge_code_response(buff, 331, LOGIN);
                            send_to_client(cur_connfd, buff, strlen(buff));
                            client_set[i].state = user;
                        } else {
                            merge_code_response(buff, 430, USER_ERROR);
                            send_to_client(cur_connfd, buff, strlen(buff));
                        }
                    } else {
                        merge_code_response(buff, 530, LOGIN_REQUIRE);
                        send_to_client(cur_connfd, buff, strlen(buff));
                    }
                }
                else if(cur_state == pass || cur_state == build) {
                    if (strcmp(verb, SYST) == 0) {
                        sprintf(command, SYS_INFO, type); // tmp use command for storing
                        merge_code_response(buff, 215, command);
                        send_to_client(cur_connfd, buff, strlen(buff));

                    } else if (strcmp(verb, TYPE) == 0) {
                        if (strcmp(command, "I") == 0) {
                            strcpy(type, command);
                            sprintf(command, SET_TYPE, type);
                            merge_code_response(buff, 200, command);
                            send_to_client(cur_connfd, buff, strlen(buff));

                        } else {
                            sprintf(tmp_buff, TYPE_ERROR, command);
                            merge_code_response(buff, 501, tmp_buff);
                            send_to_client(cur_connfd, buff, strlen(buff));
                        }
                    } else if (strcmp(verb, MKD) == 0) {
                        merge_path_and_file(tmp_buff, root, command);
                        ret = mkdir(tmp_buff, 0700);
                        strcpy(command, tmp_buff);
                        if (ret != 0) {
                            sprintf(tmp_buff, MKD_ERROR, command);
                            merge_code_response(buff, 200, tmp_buff);
                        } else {
                            sprintf(tmp_buff, MKD_INFO, command);
                            merge_code_response(buff, 200, tmp_buff);
                        }
                        send_to_client(cur_connfd, buff, strlen(buff));
                    } else if(strcmp(verb, CWD) == 0) {
                        // change working directory
                        dir = opendir(command);
                        if (dir) {
                            closedir(dir);
                            strcpy(root, command);
                            sprintf(tmp_buff, CWD_INFO, command);
                            merge_code_response(buff, 200, tmp_buff);
                            send_to_client(cur_connfd, buff, strlen(buff));
                        } else {
                            merge_code_response(buff, 550, FILE_UNA);
                            send_to_client(cur_connfd, buff, strlen(buff));
                        }

                    } else if(strcmp(verb, PWD) == 0) {
                        // return the name of current directory
                        sprintf(tmp_buff, DIR_INFO, root);
                        merge_code_response(buff, 200, tmp_buff);
                        send_to_client(cur_connfd, buff, strlen(buff));
                        
                    } else if(strcmp(verb, LIST) == 0) {
						if (cur_state == build) {
							// send initial mark
							sprintf(tmp_buff, START_TRANS, command);
							merge_code_response(buff, 150, tmp_buff);
							send_to_client(cur_client.connfd, buff, strlen(buff));

							if (!cur_client.passive) {
								if (connect(cur_client.file_fd, (struct sockaddr*)&cur_client.addr, \
									sizeof(cur_client.addr)) < 0) {
									printf("Error connect(): %s(%d)\n", strerror(errno), errno);
									ret = -1;
								}
								connfd = cur_client.file_fd;
							}
							else {
								if ((connfd = accept(cur_client.file_fd, NULL, NULL)) == -1) {
									printf("Error accept(): %s(%d)\n", strerror(errno), errno);
									ret = -1;
								}
							}
							if (ret == -1) {
								// If the connection doesn't establish successfully
								merge_code_response(buff, 425, CONNECTION_ERROR);
								send_to_client(cur_client.connfd, buff, strlen(buff));
								close_file_fd(client_set, i);
							}
							else {
								getList(cur_client.connfd, root);
								merge_code_response(buff, 226, "LIST all the files.");
								send_to_client(cur_client.connfd, buff, strlen(buff));
								close(connfd);
								connfd = -1;
							}
							if (connfd != -1) {
								close(connfd);
								connfd = -1;
							}
							close_file_fd(client_set, i);
						}
						else {
							merge_code_response(buff, 503, SEQUENCE_ERRRO);
							send_to_client(cur_client.connfd, buff, strlen(buff));
						}

                        
                    } else if(strcmp(verb, RMD) == 0) {
                        // remove a specified dir
                        merge_path_and_file(tmp_buff, root, command);
                        strcpy(command, tmp_buff);
                        rmdir(command);
                        sprintf(tmp_buff, RMD_INFO, command);
                        merge_code_response(buff, 200, tmp_buff);
                        send_to_client(cur_connfd, buff, strlen(buff));

                    } else if(strcmp(verb, RNFR) == 0) {
                        // select a file to rename
                        // should be followed by RNTO
                        strcpy(selected_file, command);
                        merge_path_and_file(command, root, selected_file);
						printf("file: %s\n", command);
                        if (access(command, F_OK) != 0) {
                            merge_code_response(buff, 550, FILE_UNA);
                        } else {
                            sprintf(tmp_buff, RNFR_INFO, selected_file);
                            merge_code_response(buff, 200, tmp_buff);
                            client_set[i].state = rnfr;
                        }
                        send_to_client(cur_connfd, buff, strlen(buff));

                    } else if (strcmp(verb, PORT) == 0) {
                        if (client_set[i].file_fd != -1)
                            close_file_fd(client_set, i);
                        ret = parse_addr_and_port(command, buff, &port);
                        if (ret == -1) {
                            // the format error
                            merge_code_response(buff, 501, ARGS_ERROR);
                            send_to_client(cur_client.connfd, buff, strlen(buff));
                            continue;
                        }
                        
                        // saving connection info
                        client_set[i].file_fd = start_socket(port, buff, &addr);
                        client_set[i].addr = addr;
                        client_set[i].port = port;
                        client_set[i].state = build;
                        client_set[i].passive = 0;

                        if (client_set[i].file_fd == -1)
                            merge_code_response(buff, 425, CONNECTION_ERROR);
                        else
                            merge_code_response(buff, 200, PORT_INFO);

                        send_to_client(cur_client.connfd, buff, strlen(buff));
                    } else if (strcmp(verb, PASV) == 0) {
                        if (cur_client.file_fd != -1)
                            close_file_fd(client_set, i);

                        // generate port and ip for connecting
                        port = generate_random_port();
                        get_local_addr(ip);

                        client_set[i].file_fd = start_socket(port, ip, &addr);
                        client_set[i].addr = addr;
                        client_set[i].port = port;
                        client_set[i].passive = 1;

                        ret = 0;
                        // listen on the port for connection
                        if (bind(client_set[i].file_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                            printf("Error bind(): %s(%d)\n", strerror(errno), errno);
                            ret = -1;
                        }
                        if (listen(client_set[i].file_fd, MAX_LISTEN) == -1) {
                            printf("Error listen(): %s(%d)\n", strerror(errno), errno);
                            ret = -1;
                        }

                        if (ret == -1) {
                            merge_code_response(buff, 501, ARGS_ERROR);
                            if (client_set[i].file_fd != -1)
                                close_file_fd(client_set, i);
                            send_to_client(cur_client.connfd, buff, strlen(buff));
                            continue;
                        }
                        
                        merge_ip_and_port(command, ip, port);
                        sprintf(tmp_buff, PASV_INFO, command);
                        merge_code_response(buff, 227, tmp_buff);
                        send_to_client(cur_client.connfd, buff, strlen(buff));

                        // only change the state of client if there are no problems
                        client_set[i].state = build;
                    } else if (strcmp(verb, RETR) == 0) {
                        // command record the command name now
                        if (cur_state == build) {
                            // send initial mark
                            sprintf(tmp_buff, START_TRANS, command);
                            merge_code_response(buff, 150, tmp_buff);
                            send_to_client(cur_client.connfd, buff, strlen(buff));

                            if (!cur_client.passive) {
                                if (connect(cur_client.file_fd, (struct sockaddr*)&cur_client.addr,\
                                        sizeof(cur_client.addr)) < 0) {
                                    printf("Error connect(): %s(%d)\n", strerror(errno), errno);
                                    ret = -1;
                                }
                                connfd = cur_client.file_fd;
                            } else {
                                if ((connfd = accept(cur_client.file_fd, NULL, NULL)) == -1) {
                                    printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                                    ret = -1;
                                }
                            }
                            if (ret == -1) {
                                // If the connection doesn't establish successfully
                                merge_code_response(buff, 425, CONNECTION_ERROR);
                                send_to_client(cur_client.connfd, buff, strlen(buff));
								close_file_fd(client_set, i);
                            } else {

                                merge_path_and_file(buff, root, command);
                                strcpy(command, buff);
								transfer* td = (transfer*)malloc(sizeof(transfer));
								strcpy(td->filename, command);
								td->connfd = cur_client.connfd;
								td->filefd = connfd;
								td->num = i;
								td->rest = cur_client.rest_len;

								pthread_t id;
								pthread_create(&id, 0, threadRetr, (void*)td);
								pthread_detach(id);
                                //code = retrive_file(connfd, command, buff);

                                //// send ending mark
                                //if (code == 226) {
                                //    merge_code_response(buff, code, END_TRANS);
                                //} else if(code == 426) {
                                //    merge_code_response(buff, code, CONNECTION_ERROR);
                                //} else if (code == 451) {
                                //    merge_code_response(buff, code, FIEL_ERROR);
                                //}
                                //close(connfd);
                                //connfd = -1;

                                //send_to_client(cur_client.connfd, buff, strlen(buff));
                            }
                            /*if (connfd != -1) {
                                close(connfd);
                                connfd = -1;
                            }*/
                            //close_file_fd(client_set, i);
                        }
                        else {
                            merge_code_response(buff, 503, SEQUENCE_ERRRO);
                            send_to_client(cur_client.connfd, buff, strlen(buff));
                        }
                    } else if (strcmp(verb, STOR) == 0) {
                        if (cur_state == build) {
                            sprintf(tmp_buff, START_TRANS, command);
                            merge_code_response(buff, 150, tmp_buff);
                            // send initial mark
                            send_to_client(cur_client.connfd, buff, strlen(buff));

                            if (!cur_client.passive) {
                                if (connect(cur_client.file_fd, (struct sockaddr*)&cur_client.addr,\
                                        sizeof(cur_client.addr)) < 0) {
                                    printf("Error connect(): %s(%d)\n", strerror(errno), errno);
                                    ret = -1;
                                }
                                connfd = cur_client.file_fd;
                            } else {
                                if ((connfd = accept(cur_client.file_fd, NULL, NULL)) == -1) {
                                    printf("Error accept(): %s(%d)\n", strerror(errno), errno);
                                    ret = -1;
                                }
                            }
                            if (ret == -1) {
                                // If the connection doesn't establish successfully
                                merge_code_response(buff, 425, CONNECTION_ERROR);
                                send_to_client(cur_client.connfd, buff, strlen(buff));
                            } else {
                                /*merge_path_and_file(buff, root, command);
                                strcpy(command, buff);
                                code = store_file(connfd, command, buff);*/

								merge_path_and_file(buff, root, command);
								strcpy(command, buff);
								transfer* td = (transfer*)malloc(sizeof(transfer));
								strcpy(td->filename, command);
								td->connfd = cur_client.connfd;
								td->filefd = connfd;
								td->num = i;

								pthread_t id;
								pthread_create(&id, 0, threadStor, (void*)td);
								pthread_detach(id);

                                // send ending mark
                                /*if (code == 226) {
                                    merge_code_response(buff, code, END_TRANS);
                                } else if(code == 426) {
                                    merge_code_response(buff, code, CONNECTION_ERROR);
                                } else if (code == 451) {
                                    merge_code_response(buff, code, FIEL_ERROR);
                                }
                                close(connfd);
                                connfd = -1;

                                send_to_client(cur_client.connfd, buff, strlen(buff));*/
                            }
                            /*if (connfd != -1) {
                                close(connfd);
                                connfd = -1;
                            }
                            close_file_fd(client_set, i);*/
                        }
                        else {
                            merge_code_response(buff, 503, SEQUENCE_ERRRO);
                            send_to_client(cur_client.connfd, buff, strlen(buff));
                        }
                    } 
					else if (strcmp(verb, REST) == 0) {
						if (cur_state == build) {
							sprintf(tmp_buff, REST_INFO, command);
							merge_code_response(buff, 350, tmp_buff);
							send_to_client(cur_client.connfd, buff, strlen(buff));
							rest_len = atoi(command);
							client_set[i].rest_len = rest_len;
						}
						else {
							merge_code_response(buff, 503, SEQUENCE_ERRRO);
							send_to_client(cur_client.connfd, buff, strlen(buff));
						}
					}
					else {
                        merge_code_response(buff, 503, SEQUENCE_ERRRO);
                        send_to_client(cur_connfd, buff ,strlen(buff));
                    }
                }
                else if(cur_state == rnfr) {
                    if (strcmp(verb, RNTO) == 0) {
                        merge_path_and_file(tmp_buff, root, selected_file);
                        strcpy(selected_file, tmp_buff);
                        merge_path_and_file(tmp_buff, root, command);
                        strcpy(command, tmp_buff);

                        rename(selected_file, command);
                        sprintf(tmp_buff, RNTO_INFO, selected_file, command);
                        merge_code_response(buff, 200, tmp_buff);
                        send_to_client(cur_connfd, buff, strlen(buff));

                        client_set[i].state = pass;
                    } else {
                        merge_code_response(buff, 503, RNTO_SEQ_ERROR);
                        send_to_client(cur_connfd, buff, strlen(buff));
                    }
                }
            }
        }

        // Find the client who has a passive answer
    }

    close(listenfd);
}


void * threadRetr(void * th)
{
	char text[8191] = "\0";
	transfer* t = (transfer*)th;
	int code = 0;

	if (t->rest == 0)
		code = retrive_file(t->filefd, t->filename, text);
	else
	{
		code = retrive_file_rest(t->filefd, t->filename, text, t->rest);
		client_set[t->num].rest_len = 0;
	}

	printf("code=%d", code);
	//send ending mark
	if (code == 226) {
		merge_code_response(text, code, END_TRANS);
	}
	else if (code == 426) {
		merge_code_response(text, code, CONNECTION_ERROR);
	}
	else if (code == 451) {
		merge_code_response(text, code, FIEL_ERROR);
	}
	close(t->filefd);
	t->filefd = -1;

	send_to_client(t->connfd, text, strlen(text));

	close_file_fd(client_set, t->num);
	pthread_exit(0);
}

void * threadStor(void * th)
{
	char text[8191] = "\0";
	transfer* t = (transfer*)th;
	int code = 0;

	if (t->rest == 0) {
		code = store_file(t->filefd, t->filename, text);
	}
	else {
		code = store_file_rest(t->filefd, t->filename, text, t->rest);
		client_set[t->num].rest_len = 0;
	}
	// send ending mark
	if (code == 226) {
		merge_code_response(buff, code, END_TRANS);
	}
	else if (code == 426) {
		merge_code_response(buff, code, CONNECTION_ERROR);
	}
	else if (code == 451) {
		merge_code_response(buff, code, FIEL_ERROR);
	}
	close(t->filefd);
	t->filefd = -1;

	send_to_client(t->connfd, buff, strlen(buff));

	close_file_fd(client_set, t->num);
	pthread_exit(0);
}
