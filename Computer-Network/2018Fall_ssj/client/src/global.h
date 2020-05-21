#ifndef GLOBAL
#define GLOBAL

// Baisc Default Config for server
#define DEFAULT_PORT 21
#define DEFAULT_DIR_PATH "./tmp"

#define BUFF_SIZE 8192
#define MAX_LISTEN 10

// Definition of Status
// (I use the thinking of finite state machine)
#define USER "USER"
#define PASS "PASS"
#define PORT "PORT"
#define PASV "PASV"
#define RETR "RETR"
#define REST "REST"
#define STOR "STOR"
#define SYST "SYST"
#define TYPE "TYPE"
#define QUIT "QUIT"
#define ABRT "ABRT"
#define MKD  "MKD"
#define CWD  "CWD"
#define PWD  "PWD"
#define LIST "LIST"
#define RMD  "RMD"
#define RNFR "RNFR"
#define RNTO "RNTO"


enum State {
    start,  // Start establish connection
    user,   // client send USER command
    pass,   // clinet send PASS command
    build,  // call PORT/PASV to specify IP and PORT
    tran,   // call RETR/STOR to retrive data
};

// For Response
#define PASV_INFO "Entering Passive Mode(%s)"

#endif
