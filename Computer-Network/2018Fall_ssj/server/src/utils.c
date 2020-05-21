/*
sever: utils.py
-------------------------------------
Compact some functions for simplicity and reusability
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <sys/stat.h>

#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>

#include "utils.h"
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

    if(command[blank_idx] != ' ') blank_idx++;

    if (blank_idx < 3 || blank_idx > 4) return 0;

    for (i = 0; i < blank_idx; i++) {
        verb[i] = command[i];
    }

    verb[blank_idx] = '\0'; 
    printf("verb: %s\n", verb);

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

void get_code_and_info(char* response, int* code, char* info) {
    // Get code and info from response
    sscanf(response, "%d %s", code, info);
}

void merge_code_response(char* buff, int code, char* response) {
    /*
    Merge code and response with the form
        code response
    to buff
    */
    sprintf(buff, "%d %s\r\n", code, response);
}

void merge_ip_and_port(char* buff, char* ip, int port) {
    int prefix_port = port / 256;
    int postfix_port = port % 256;

    int h1, h2, h3, h4;
    sscanf(ip, "%d.%d.%d.%d", &h1, &h2, &h3, &h4);
    
    sprintf(buff, "%d,%d,%d,%d,%d,%d", h1, h2, h3, h4, prefix_port, postfix_port);
}

void get_port_and_root(char** argv, int argc, int *port, char* root) {
    /*
    Work on command input, request for port and dirpath,
    fill with default setting if there is None

    Parameters
    ---------------------------------------
    argc, argv: from main
    port, dirpath: extern variable for saving the input setting 
    */
    int i;
    *port = DEFAULT_PORT;
    strcpy(root, DEFAULT_DIR_PATH);

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0) {
            if (i + 1 < argc)
                sscanf(argv[i+1], "%d", port);
        } else if (strcmp(argv[i], "-root") == 0) {
            if (i + 1 < argc) 
                strcpy(root, argv[i+1]);
        }
    }
}

int send_to_client(int connfd, char* command, int len_command) {
    int len_write = write(connfd, command,  len_command);
    if (len_write < 0) {
        printf("Error write(): %s(%d)\n", strerror(errno), errno);
        return -1;
    } else {
        ;
    }
    
    return len_write;
}

int recv_from_client(int connfd, char* command, int fd_conn) {
    int len_read = read(connfd, command, BUFF_SIZE);
    if (len_read < 0) {
        printf("error read(): %s(%d)\n", strerror(errno), errno);
        close(connfd);
    } else {
        ;
    }
    if (!fd_conn)
        command[len_read] = '\0';

    return len_read;
}

int is_valid_pwd(const char* pwd) {
    /*
    Test if a pwd is valid, not requested in this program
    */
    return 1;
}

int is_valid_user(const char* user) {
    /*
    test if the user is valid
    */
    return strcmp(user, "anonymous") == 0;
}

int parse_addr_and_port(char* addr, char* formatted_addr, int* port) {
    /*
    Parse the form
        h1,h2,h3,h4,p1,p2

    Return -1 if the foramt is not consistent
            0 if the format is consistent
    */
    int h1 = -1; 
    int h2 = -1;
    int h3 = -1;
    int h4 = -1;
    int p1 = -1;
    int p2 = -1;
    
    sscanf(addr, "%d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2);

    if (h1 == -1 || h2 == -1 || h3 == -1 || h4 == -1 || p1 == -1 || p2 == -1 || \
        h1 < 0 || h2 < 0 || h3 < 0 || h4 < 0 || p1 < 0 || p2 <0) {
        // the format is not right        
        return -1;
    }

    sprintf(formatted_addr, "%d.%d.%d.%d", h1, h2, h3, h4);

    *port = p1 * 256 + p2;
    return 0;
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
	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	/*
    if (strcmp(ip, "") == 0)
        addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else {
        //addr->sin_addr.s_addr = inet_addr(ip);
        if (inet_pton(AF_INET, "127.0.0.1", &(addr->sin_addr)) <= 0) {
            printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
            return -1;
        }
    }
	*/

    return listenfd;
}

int retrive_file(int connfd, char* filename, char* buff) {
    /*
    Send file to client

    Returns
    -------------------------------
    code: 
        226:  finish writing
        425: invalid TCP
        426: TCP was established but broken by client
        451: file error
    */
    FILE* fp;
    int read_len;
    int send_len;

    fp = fopen(filename,"rb");

	if (fp == NULL) {
		printf("cannot open file");
		return 451; 
	}

    while(read_len = fread(buff, sizeof(char), BUFF_SIZE, fp)) {
        send_len = send_to_client(connfd, buff, read_len);
        printf("send len: %d, read_len: %d\n", send_len, read_len);
        if (send_len == -1) {
            fclose(fp);
            return 426;
        }
    }
    fclose(fp);

    return 226;
}

int retrive_file_rest(int connfd, char * filename, char * buff, int offset)
{
	FILE* fp;
	int read_len;
	int send_len;

	fp = fopen(filename, "rb");

	if (fp == NULL) {
		printf("cannot open file");
		return 451;
	}

	fseek(fp, offset, SEEK_SET);

	while (read_len = fread(buff, sizeof(char), BUFF_SIZE, fp)) {
		send_len = send_to_client(connfd, buff, read_len);
		printf("send len: %d, read_len: %d\n", send_len, read_len);
		if (send_len == -1) {
			fclose(fp);
			return 426;
		}
	}
	fclose(fp);

	return 226;
}

int store_file(int connfd, char* filename, char* buff) {
    /*
    Get file From client
    */
    char file_buff[MAX_LISTEN];
    FILE* fp;
    int n;
    int code = -1;

    fp = fopen(filename,"wb");
    if (fp == NULL) return 451;

    while(n = recv_from_client(connfd, buff, 1)) {
        fwrite(buff, sizeof(char), n, fp);
    }

    fclose(fp);
    
    return 226;

}

int store_file_rest(int connfd, char * filename, char * buff, int offset)
{
	char file_buff[MAX_LISTEN];
	FILE* fp;
	int n;
	int code = -1;

	fp = fopen(filename, "ab");
	if (fp == NULL) return 451;

	while (n = recv_from_client(connfd, buff, 1)) {
		fwrite(buff, sizeof(char), n, fp);
	}

	fclose(fp);

	return 226;
}

int generate_random_port() {
    // Generate port between [20000, 65535]
    int rand_result = rand() % 45535 + 20000;

    return rand_result;
}

void get_local_addr(char* ip) {;
    int fd, num;  
    struct ifreq ifq[16];  
    struct ifconf ifc;  
    int i;  
    char *ips, *tmp_ip;  
    char *delim = ".";  
    int val;  
      
    fd = socket(AF_INET, SOCK_DGRAM, 0);  
    if(fd < 0){  
        fprintf(stderr, "socket failed\n");  
        return;  
    }  
    ifc.ifc_len = sizeof(ifq);  
    ifc.ifc_buf = (caddr_t)ifq;  
    if(ioctl(fd, SIOCGIFCONF, (char *)&ifc)){  
        fprintf(stderr, "ioctl failed\n");  
        return;  
    }  
    num = ifc.ifc_len / sizeof(struct ifreq);  
    if(ioctl(fd, SIOCGIFADDR, (char *)&ifq[num-1])){  
        fprintf(stderr, "ioctl failed\n");  
        return;  
    }  
    close(fd);  
      
    val = 0;  
    for(i=0; i<num; i++){  
        tmp_ip = inet_ntoa(((struct sockaddr_in*)(&ifq[i].ifr_addr))-> sin_addr);  
        if(strcmp(tmp_ip, "127.0.0.1") != 0){  
            val++;  
        }  
    }  
      
    ips = (char *)malloc(val * 16 * sizeof(char));  
    if(ips == NULL){  
        fprintf(stderr, "malloc failed\n");  
        return;  
    }  
    memset(ips, 0, val * 16 * sizeof(char));  
    val = 0;  
    for(i=0; i<num; i++){  
        tmp_ip = inet_ntoa(((struct sockaddr_in*)(&ifq[i].ifr_addr))-> sin_addr);  
        if(strcmp(tmp_ip, "127.0.0.1") != 0){  
            if(val > 0){  
                strcat(ips, delim);  
            }  
            strcat(ips, tmp_ip);  
            val ++;  
        }  
    }  
      
    strcpy(ip, ips);
}

void merge_path_and_file(char* buff, char* path, char* file) {
    /*
    Merge path and file to a full path
    */
    int file_len = strlen(file);
    int path_len = strlen(path);


    if (file[0] == '/') {
        sprintf(buff, "%s%s", path, file);
    } else {
        sprintf(buff, "%s/%s", path, file);
    }
}

int remove_dir(const char *dir) {
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[128];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    if ( 0 != access(dir, F_OK) ) {
        return 0;
    }

    if ( 0 > stat(dir, &dir_stat) ) {
        perror("get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) {
        remove(dir);
    } else if ( S_ISDIR(dir_stat.st_mode) ) {
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) {
            if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
                continue;
            }

            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            remove_dir(dir_name);
        }
        closedir(dirp);

        rmdir(dir);
    } else {
        perror("unknow file type!");    
    }

    return 0;
}

void getList(int connfd, char* path)
{
	int read_len;
	int send_len;
	char control[100];
	sprintf(control, "ls -la %s | tail -n+2 > tmp.txt", path);
	int rs = system(control);
	if (rs < 0) {
		return;
	}
	
	FILE* fp = fopen("tmp.txt", "r");
	if (fp == NULL) {
		printf("cannot open file");
	}
	char buffer[8192];
	int n;

	while (read_len = fread(buffer, sizeof(char), BUFF_SIZE, fp)) {
		send_len = send_to_client(connfd, buffer, read_len);
		printf("buffer=%s",buffer);
		printf("send len: %d, read_len: %d\n", send_len, read_len);
		if (send_len == -1) {
			fclose(fp);
		}
	}

	fclose(fp);
}
