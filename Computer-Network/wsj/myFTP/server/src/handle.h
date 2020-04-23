//
// Created by 王世杰 on 2019/10/21.
//

#ifndef FTP_SERVER_HANDLE_H
#define FTP_SERVER_HANDLE_H
#include "util.h"

void USER(State* state, char* parameter, int client)
{
    state->status = STATUS_NEED_PWD;
    send(client, "331 Guest login ok, send your complete e-mail address as password.\r\n", 68, 0);
}

void PASS(State* state, char* parameter, int client)
{
    state->status = STATUS_LOGIN;
    send(client, "230 Guest login ok, access restrictions apply.\r\n", 48, 0);
}

void SYST(State* state, char* parameter, int client)
{
    send(client, "215 UNIX Type: L8\r\n", 19, 0);
}

void TYPE(State* state, char* parameter, int client)
{
    if(!strcmp(parameter, "I"))
    {
        send(client, "200 Type set to I.\r\n", 20, 0);
    }else
    {
        send(client, "200 Unkown type.\r\n",18,0);
    }
}

void PASV(State* state, char* parameter, int client)
{
    state->tranport_mode = PASV_MODE;
    char msg[100];
    int i, sockfd;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    for (i = 8192; i < 60000; i++)
    {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(i);
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == 0)
        {
            sscanf(getIP(),"%d,%d,%d,%d",&state->addr[0],&state->addr[1],&state->addr[2],&state->addr[3]);
            state->addr[4] = i / 256;
            state->addr[5] = i % 256;
            sprintf(msg,"227 Entering Passive Mode (%s,%d,%d)\r\n", getIP(), state->addr[4], state->addr[5]);
            if (listen(sockfd, 10) == -1)
            {
                printf("listen failed\n");
                return;
            }
            state->psv_socket = sockfd;
            break;
        }
    }
    send(client, msg, strlen(msg), 0);
}

void PORT(State* state, char* parameter, int client)
{
    state->tranport_mode = PORT_MODE;
    sscanf(parameter, "%d,%d,%d,%d,%d,%d", &state->addr[0], &state->addr[1], &state->addr[2], &state->addr[3], &state->addr[4], &state->addr[5]);
    send(client, "200 Port command successful.\r\n", 30, 0);
}

void STOR(State* state, char* parameter, int client)
{
    char path_copy[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    char msg[BUFFER_SIZE];

    FILE* fp;
    int data_connection = -2;

    if(state->tranport_mode == NO_MODE)
    {
        send(client, "425 Use PASV or PORT first.\r\n", 29, 0);
        return;
    }else if(state->tranport_mode == PASV_MODE)
    {
        data_connection = accept(state->psv_socket, NULL, NULL);  //wait for a connection from the client
        close(state->psv_socket);
    }else if(state->tranport_mode == PORT_MODE)
    {
        data_connection = connectClient(state);
    }

    if(data_connection == -1)
    {
        send(client, "425 No TCP connection was established.\r\n", 40, 0);
    }
    state->tranport_mode = NO_MODE;
    if(data_connection < 0) return;

    sprintf(msg, "150 Opening BINARY mode data connection for %s.\r\n", parameter);
    send(client, msg, strlen(msg),0);

    char file_name[BUFFER_SIZE];
    memset(file_name, 0 , BUFFER_SIZE);
    getFileName(parameter, file_name);

    chdir(state->root);
    chdir(state->remote_path+1);

    if((fp = fopen(file_name, "wb")))
    {
        int receive_count = recvFile(fp, data_connection);
        printf("recv stop\n");
        if(receive_count < 0)
        {
            send(client, "426 TCP connection was broken by the client or by network failure.\r\n",68,0);
        }else
        {   
            send(client, "226 Transfer complete.\r\n",24,0);
        }
        fclose(fp);
    }else
    {
        fprintf(stderr, "Can not write the file");
        send(client, "451 Cannot write file to the disk.\r\n",36,0);
    }
    chdir(path_copy);
    close(data_connection);
}

void RETR(State* state, char* parameter, int client)
{
    char path_copy[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    char msg[BUFFER_SIZE];
    sprintf(msg, "150 Opening BINARY mode data connection for %s.\r\n", parameter);
    send(client, msg, strlen(msg),0);
    FILE* fp;
    int data_connection = -2;

    if(state->tranport_mode == NO_MODE)
    {
        send(client, "425 Use PASV or PORT first.\r\n", 29, 0);
    }else if(state->tranport_mode == PASV_MODE)
    {
        data_connection = accept(state->psv_socket, NULL, NULL);  //wait for a connection from the client
        close(state->psv_socket);
    }else if(state->tranport_mode == PORT_MODE)
    {
        data_connection = connectClient(state);
    }
    if(data_connection == -1)
    {
        send(client, "425 No TCP connection was established.\r\n", 40, 0);
    }
    state->tranport_mode = NO_MODE;

    if(data_connection < 0) return;

    chdir(state->root);
    chdir(state->remote_path+1);
    char file_name[BUFFER_SIZE];
    if(parameter[0] == '/')
    {
        sprintf(file_name, "%s%s", state->root, parameter);
    }else
    {
        sprintf(file_name, "%s", parameter);
    }
    long file_size = getFileSize(file_name);
    printf("rest_size: %d\n",state->rest_start);
    if((access(file_name, R_OK) == 0) && (fp = fopen(file_name, "rb")))
    {
        if(file_size < state->rest_start)
        {  //如果定位位置比文件大小还大
            send(client,"551 The remote file is smaller than that local file.\r\n",54,0);
        }else
        {
            fseek(fp, state->rest_start, SEEK_SET); //定位位置
            int send_count = sendFile(fp, data_connection);
            file_size = getFileSize(file_name);  //新的文件大小
            if(send_count<0)
            {
                send(client, "426 TCP connection was broken by the client or by network failure.\r\n",68,0);
            }else
            {
                send(client, "226 Transfer complete.\r\n",24,0);
            }
        }
        fclose(fp);
    }else
    {
        fprintf(stderr, "Can not read the file\n");
        send(client,"551 Cannot read file from the disk.\r\n",37,0);
    }
    close(data_connection);
    chdir(path_copy);
    state->rest_start = 0;
}

void REST(State* state, char* parameter, int client)
{
    if(sscanf(parameter, "%d", &state->rest_start) != -1)
    {
        char msg[BUFFER_SIZE];
        sprintf(msg,"350 Set the start position of file to %d\r\n",state->rest_start);
        send(client,msg,strlen(msg),0);
    }else
    {
        send(client,"500 The argument is invalid.\r\n",38,0);
    }
}

void QUIT(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    sprintf(msg,"221 Goodbye.\r\n");
    send(client,msg,strlen(msg),0);
    close(state->psv_socket);
    close(client);
    exit(0);
}

void MKD(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    char path_copy[BUFFER_SIZE], path[BUFFER_SIZE],cmd[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    chdir(state->root);
    char root[BUFFER_SIZE];
    getcwd(root,BUFFER_SIZE);

    chdir(state->remote_path+1);
    char cur_path[BUFFER_SIZE];
    getcwd(cur_path,BUFFER_SIZE);

    if(parameter[0] == '.' && parameter[1] == '.')
    {
        sprintf(msg,"550 Create directory failed\r\n");
        send(client,msg,strlen(msg),0);
        return;
    }else if(parameter[0] == '/')
    {
        sprintf(path, "%s%s", state->root, parameter);
    }else
    {
        sprintf(path, "%s", parameter);
    }
    sprintf(cmd, "mkdir %s",path);

    if(system(cmd) == 0)
    {
        sprintf(msg,"257 Create directory \"%s\" successfully\r\n",parameter);
    }else
    {
        sprintf(msg,"550 Create directory failed\r\n");
    }
    chdir(path_copy);
    send(client,msg,strlen(msg),0);
}

void CWD(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    char path_copy[BUFFER_SIZE], path[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    chdir(state->root);
    char root[BUFFER_SIZE];
    getcwd(root,BUFFER_SIZE);

    chdir(state->remote_path+1);
    char cur_path[BUFFER_SIZE];
    getcwd(cur_path,BUFFER_SIZE);

    if(parameter[0] == '/')
    {
        sprintf(path, "%s%s", state->root, parameter);
    }else
    {
        sprintf(path, "%s", parameter);
    }

    if (chdir(path) == 0)
    {
        char temp_path[BUFFER_SIZE];
        getcwd(temp_path,BUFFER_SIZE);
        if(isPrefix(root, temp_path))
        {
            if(!strcmp(temp_path,root))
            {
                sprintf(state->remote_path, "/");
            }else
            {
                sprintf(state->remote_path, "%s", temp_path+strlen(root));
            }
            sprintf(msg,"250 OK. Current dir: \"%s\"\r\n",state->remote_path);
        }else
        {
            sprintf(msg,"550 Can't change directory to: \"%s\"\r\n",path);
        }
    }else
    {
        sprintf(msg,"550 Can't change directory to: \"%s\"\r\n",path);
    }
    chdir(path_copy);
    send(client,msg,strlen(msg),0);

}

void PWD(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    sprintf(msg, "%s\"%s\"\r\n", "257 current path is: ", state->remote_path);
    send(client, msg, strlen(msg), 0);
}

void RMD(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    char path_copy[BUFFER_SIZE], path[BUFFER_SIZE],cmd[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    chdir(state->root);
    char root[BUFFER_SIZE];
    getcwd(root,BUFFER_SIZE);

    chdir(state->remote_path+1);
    char cur_path[BUFFER_SIZE];
    getcwd(cur_path,BUFFER_SIZE);

    if(parameter[0] == '.' && parameter[1] == '.')
    {
        sprintf(msg,"550 Remove directory failed\r\n");
        send(client,msg,strlen(msg),0);
        return;
    }else if(parameter[0] == '/')
    {
        sprintf(path, "%s%s", state->root, parameter);
    }else
    {
        sprintf(path, "%s", parameter);
    }
    sprintf(cmd, "rm -r %s", path);
    if(system(cmd) == 0)
    {
        sprintf(msg,"250 The directory was successfully removed\r\n");
    }else
    {
        sprintf(msg,"550 Remove directory failed\r\n");
    }
    chdir(path_copy);
    send(client,msg,strlen(msg),0);
}

void CDUP(State* state, char* parameter, int client)
{
    CWD(state, "..", client);
}

void DELE(State* state, char* parameter, int client)
{
    RMD(state, parameter, client);
}

void LIST(State* state, char* parameter, int client)
{
    char path_copy[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    char msg[BUFFER_SIZE];
    sprintf(msg, "150 Opening BINARY mode data connection for %s.\r\n", parameter);
    send(client, msg, strlen(msg),0);
    int data_connection = -2;

    if(state->tranport_mode == NO_MODE)
    {
        send(client, "425 Use PASV or PORT first.\r\n", 29, 0);
    }else if(state->tranport_mode == PASV_MODE)
    {
        data_connection = accept(state->psv_socket, NULL, NULL);  //wait for a connection from the client
        close(state->psv_socket);
    }else if(state->tranport_mode == PORT_MODE)
    {
        data_connection = connectClient(state);
    }
    if(data_connection == -1)
    {
        send(client, "425 No TCP connection was established.\r\n", 40, 0);
    }
    state->tranport_mode = NO_MODE;
    if(data_connection < 0) return;

    chdir(state->root);
    chdir(state->remote_path+1);
    char target_path[BUFFER_SIZE];
    if(strlen(parameter)!=0)
    {
        if(parameter[0] == '/')
        {
            sprintf(target_path, "%s%s", state->root, parameter);
        }else if(parameter[0]=='.'&&parameter[1]=='.'&& (!strcmp(state->remote_path,"/")))
        {
            sprintf(target_path, "./");
        }else
        {
            sprintf(target_path, "%s", parameter);
        }
    } else
    {
        sprintf(target_path, "./");
    }

    int valid_path = 1;
    if(chdir(target_path)!=0)
    {
        valid_path=0;
    }

    char cmd[BUFFER_SIZE];
    if(valid_path)
    {
        sprintf(cmd, "ls -l");
    } else
    {
        sprintf(cmd, "ls -l %s",target_path);
    }
    FILE *fp = popen(cmd, "r");

    int count, p;
    char buf[4096];
    while (1)
    {
        count = fread(buf, 1, 4096, fp);
        if (count == 0) break;
        p = 0;
        while (p < count)
        {
            int n = write(data_connection, buf + p, count - p);
            if (n < 0)
            {
                printf("write wrong\n");
                return;
            }
            else p += n;
        }
        if (count != 4096) break;
    }
    pclose(fp);
    close(data_connection);
    chdir(path_copy);
    if(valid_path)
    {
        sprintf(msg,"226 List finished.\r\n");
        send(client,msg,strlen(msg),0);
    } else
    {
        sprintf(msg,"451 Invalid path \"%s\".\r\n",parameter);
        send(client,msg,strlen(msg),0);
    }

}

void RNFR(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    char path_copy[BUFFER_SIZE], path[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    chdir(state->root);
    char root[BUFFER_SIZE];
    getcwd(root,BUFFER_SIZE);

    chdir(state->remote_path+1);
    char cur_path[BUFFER_SIZE];
    getcwd(cur_path,BUFFER_SIZE);

    if(parameter[0]=='.'&&parameter[1]=='.'&& (!strcmp(state->remote_path,"/")))
    {
        sprintf(msg,"550 File not found or invalid path.\r\n");
        send(client,msg,strlen(msg),0);
        return;
    }else if(parameter[0] == '/')
    {
        sprintf(path, "%s%s", state->root, parameter);
    }else
    {
        sprintf(path, "%s", parameter);
    }
    FILE *fin = fopen(path, "r");
    if (fin)
    {
        fclose(fin);
        strcpy(state->rn_file, path);
        sprintf(msg,"350 RNFR accepted - file exists, ready for destination.\r\n");
    }else
    {
        sprintf(msg,"550 RNFR rejected - file doesn't exist.\r\n");
    }
    chdir(path_copy);
    send(client,msg,strlen(msg),0);
}

void RNTO(State* state, char* parameter, int client)
{
    char msg[BUFFER_SIZE];
    if(strcmp(state->last_cmd,"RNFR"))  //RNTO命令没有紧跟RNFR
    {
        sprintf(msg,"503 RNTO does not come immediately after RNFR.\r\n");
        send(client,msg,strlen(msg),0);
        return;
    }

    char path_copy[BUFFER_SIZE], cmd[BUFFER_SIZE], path[BUFFER_SIZE];
    getcwd(path_copy,BUFFER_SIZE);

    chdir(state->root);
    char root[BUFFER_SIZE];
    getcwd(root,BUFFER_SIZE);

    chdir(state->remote_path+1);
    char cur_path[BUFFER_SIZE];
    getcwd(cur_path,BUFFER_SIZE);
    if(parameter[0]=='.'&&parameter[1]=='.'&& (!strcmp(state->remote_path,"/")))
    {
        sprintf(msg,"550 Invalid path.\r\n");
        send(client,msg,strlen(msg),0);
        return;
    }else if(parameter[0] == '/')
    {
        sprintf(path, "%s%s", state->root, parameter);
    }else
    {
        sprintf(path, "%s", parameter);
    }

    sprintf(cmd, "mv %s %s", state->rn_file, path);
    if(system(cmd) == 0)
    {
        sprintf(msg,"200 File successfully renamed.\r\n");
    } else
    {
        sprintf(msg,"550 Fail to rename file.\r\n");
    }
    strcpy(state->rn_file, "");
    chdir(path_copy);
    send(client,msg,strlen(msg),0);
}

void NONE(int client)
{
    char msg[BUFFER_SIZE];
    sprintf(msg,"550 Invalid command.\r\n");
    send(client,msg,strlen(msg),0);
}


void handle(char* buffer, int client, State* state)
{
    buffer[strlen(buffer)-2] = '\0';     //去掉buffer行位的\r\n
    printf("buffer: %s len: %d \n", buffer,(int)strlen(buffer));

    char *command;
    char parameter[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    strcpy(temp, buffer);

    command = strtok(temp, " ");
    int index = strlen(command)+1;
    int operand_len = strlen(buffer)-index;
    if(operand_len>0)
    {
        strncpy(parameter, buffer + index, operand_len);
        parameter[operand_len] = '\0';
    }else
    {
        parameter[0] = '\0';
    }
    if(state->status == STATUS_NEED_USER)
    {
        char msg[BUFFER_SIZE];
        if(!strcmp(command,"USER")) USER(state, parameter, client);
        else if(!strcmp(command,"PASS"))
        {
            sprintf(msg,"530 Please tell me who you are.\r\n");
            send(client, msg, strlen(msg), 0);
        }else
        {   sprintf(msg,"530 You aren't logged in.\r\n");
            send(client, msg, strlen(msg), 0);
        }
    }else if(state->status == STATUS_NEED_PWD)
    {
        char msg[BUFFER_SIZE];
        if(!strcmp(command,"PASS")) PASS(state, parameter, client);
        else if(!strcmp(command,"USER")) USER(state, parameter, client);
        else
        {
            sprintf(msg,"530 You aren't logged in.\r\n");
            send(client, msg, strlen(msg), 0);
        }
    }else
    {   char msg[BUFFER_SIZE];
        if(!strcmp(command,"USER"))
        {
            sprintf(msg,"530 You're already logged in.\r\n");
            send(client, msg, strlen(msg), 0);
        }else if(!strcmp(command,"PASS"))
        {
            sprintf(msg,"530 We can't do that in the current session.\r\n");
            send(client, msg, strlen(msg), 0);
        }else
        {
            if(!strcmp(command,"RETR")) RETR(state, parameter, client);
            else if(!strcmp(command,"STOR")) STOR(state, parameter, client);
            else if(!strcmp(command,"QUIT")) QUIT(state, parameter, client);
            else if(!strcmp(command,"SYST")) SYST(state, parameter, client);
            else if(!strcmp(command,"TYPE")) TYPE(state, parameter, client);
            else if(!strcmp(command,"PORT")) PORT(state, parameter, client);
            else if(!strcmp(command,"PASV")) PASV(state, parameter, client);
            else if(!strcmp(command,"LIST")) LIST(state, parameter, client);
            else if(!strcmp(command,"REST")) REST(state, parameter, client);
            else if(!strcmp(command,"RNFR")) RNFR(state, parameter, client);
            else if(!strcmp(command,"RNTO")) RNTO(state, parameter, client);            
            else if(!strcmp(command,"CDUP")) CDUP(state, parameter, client);
            else if(!strcmp(command,"DELE")) DELE(state, parameter, client);
            else if(!strcmp(command,"MKD"))  MKD(state, parameter, client);
            else if(!strcmp(command,"CWD"))  CWD(state, parameter, client);
            else if(!strcmp(command,"PWD"))  PWD(state, parameter, client);
            else if(!strcmp(command,"RMD"))  RMD(state, parameter, client);
            else NONE(client);
        }
        strcpy(state->last_cmd,command);
    }
}

#endif //FTP_SERVER_HANDLE_H

