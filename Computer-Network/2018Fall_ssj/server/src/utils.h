
#ifndef UTILS
#define UTILS

int is_valid_command(char* command);

void parse_request(char*, char*, char*);

void merge_code_response(char* buff, int code, char* response);

void get_port_and_root(char**, int, int*, char*);

int send_to_client(int connfd, char* command, int n);

int recv_from_client(int connfd, char* command, int file_conn);

int is_valid_pwd(const char* pwd);

int is_valid_user(const char* user);

int parse_addr_and_port(char* addr, char* formatted_addr, int* port);

int start_socket(int port, char* ip, struct sockaddr_in* addr);

int retrive_file(int connfd, char* filename, char* buff);

int retrive_file_rest(int connfd, char* filename, char* buff, int offset);

int store_file(int connfd, char* filename, char* buff);

int store_file_rest(int connfd, char* filename, char* buff, int offset);

int generate_random_port();

void get_local_addr(char* ip);

void merge_ip_and_port(char* buff, char* ip, int port);

void merge_path_and_file(char* buff, char* path, char* file);

int remove_dir(const char *dir);

void getList(int connfd,char* path);


#endif