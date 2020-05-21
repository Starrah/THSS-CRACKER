#ifndef UTILS
#define UTILS

int is_valid_command(char* command);

void parse_request(char* request, char* verb, char* info);
void parse_response(char* response, int* code, char* info);

void merge_verb_param(char* buff, char* verb, char* param);

int send_to_server(int sockfd, char* buff, int len);

int recv_from_server(int sockfd, char* buff, int file_cnn);

void parse_addr_and_port(char* addr, char* formatted_addr, int* port);

int start_socket(int port, char* ip, struct sockaddr_in* addr);

void get_addr_and_port(char* buff, char* info);

#endif

