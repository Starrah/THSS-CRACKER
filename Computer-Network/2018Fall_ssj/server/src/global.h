#ifndef GLOBAL
#define GLOBAL

// Baisc Default Config for server
#define DEFAULT_PORT 21
#define DEFAULT_DIR_PATH "/tmp"

#define BUFF_SIZE 8192
#define MAX_LISTEN 10

#define DEFAULT_TYPE "L8"

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

// using enum here to avoid the problem of shadow copy
enum State {
    start,  // Start establish connection
    user,   // client send USER command
    pass,   // clinet send PASS command
    build,  // call PORT/PASV to specify IP and PORT
    tran,   // call RETR/STOR to retrive data
    rnfr    // rename file
};

// For Response
#define WELCOME "ftp.tsinghua.edu.cn FTP ready"
#define LOGIN "Guest login ok, send your complete e-mail address as password"
#define VERIFIED "Verified, Welcome!"
#define SYS_INFO "UNIX Type: %s"
#define SET_TYPE "Type set to %s."
#define QUIT_INFO "You have already quit, thanks for using"
#define ABRT_INFO "You have already abort, thanks for using"
#define PORT_INFO "PORT command successful"

#define REST_INFO "Restarting at %s. Send STORE or RETR to initiate transfer."

#define START_TRANS "Opening BINARY mode data connection for %s"
#define END_TRANS "Transfer Complete"

#define PASV_INFO "Entering Passive Mode(%s)"

#define DIR_INFO "Current Directory: %s"
#define CWD_INFO "Working Directory Changed to: %s"
#define MKD_INFO "Make dir: %s"
#define RMD_INFO "Successfully Remove dir: %s"

#define RNFR_INFO "Selcted file: %s"
#define RNTO_INFO "Rename file: %s -> %s"

// For Error Response
#define COMMAND_ERROR "The Command is invlalid"
#define LOGIN_REQUIRE "You Must Login In before such operation"
#define USER_ERROR "User not exist"
#define PWD_ERROR "Invalid password, Please try again"
#define TYPE_ERROR "Invalid Type: %s, I is the only choise"
#define CONNECTION_ERROR "Data Connection Failed to connect"
#define SEQUENCE_ERRRO "Need To Call PASV/PORT Firstly"
#define RN_SEQ_ERROR "Need To Call RNFR firstly"
#define RNTO_SEQ_ERROR "Need To Call RNTO to confirm the final name"
#define ARGS_ERROR "Invalid command arguments. Please Check"
#define FIEL_ERROR "File Error: file doesn't exist or permission denied"
#define CONNECT_FAIL "Connect Failed: Closed by client"

#define MKD_ERROR "Failed to make dir: %s"
#define FILE_UNA "File Not Available: file doesn't exist or permission deny"


#endif