/*
    文件名：PathHandle.c
    描述：定义文件处理的函数，还有当前目录这个全局变量
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "PathHandle.h"
#include "SocketHandle.h"



/*
描述：初始化文件信息
参数：控制和数据连接ID， 开始位置，当前目录，文件名
返回：文件信息
*/
struct FileInfo* InitFileInfo(int ControlID, int ID, int Start, char* Current, char* FileName)
{
    struct FileInfo* Info = (struct FileInfo*)malloc(sizeof(struct FileInfo));
    if(Current == NULL)
    {
        Info -> WorkingPlace = NULL;
    }
    else
    {
        Info -> WorkingPlace = (char*)malloc(sizeof(char)*strlen(Current));
        strcpy(Info -> WorkingPlace, Current);
    }
    if(FileName == NULL)
    {
        Info -> FileName = NULL;
    }
    else
    {
        Info -> FileName = (char*)malloc(sizeof(char)*strlen(FileName));
        strcpy(Info -> FileName, FileName);
    }
    Info -> ControlID = ControlID;
    Info -> StartPoint = Start;
    Info -> ConnectionID = ID;
    return Info;
}

/*
描述：删除文件信息
参数：文件信息
返回：无
*/
void DeleteFileInfo(struct FileInfo Info)
{
    free(Info.WorkingPlace);
    free(Info.FileName);
}

/*
描述：新建一个文件目录，对应MKD
参数：连接，目录名
返回：成功1失败0
*/
bool MakePath(struct Connection* Link, char PathName[])
{
    int Result = 0;
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    }
    if(Result == 0)
    {
        Result = mkdir(PathName, 0);
    }
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
描述：获得全局目录
参数：目录名
返回：目录，失败是NULL
*/
char* GetFullPath(char* PathName)
{
    char* NewPath = NULL;
    if(Root == NULL)
    {
        return NULL;
    }
    if(PathName == NULL)
    {
        return NULL;
    }
    printf("%s\n%s\n",Root, PathName);
    if(PathName[0] != '/')
    {
        //相对目录不处理
        int NewLength = strlen(PathName);
        NewPath = (char*)malloc(sizeof(char) * NewLength);
        memset(NewPath, 0 ,sizeof(char) * NewLength);
        for(int i = 0; i < NewLength; i ++)
        {
            NewPath[i] = PathName[i];
        }
        for(int i = 0; i < strlen(NewPath); i ++)
        {
            if(i >= NewLength)
            {
                NewPath[i] = 0;
            }
        }
        return NewPath;
    }
    int LenRoot = strlen(Root);
    if(Root[LenRoot - 1] == '/')
    {
        LenRoot -= 1;
    }
    int LenPart = strlen(PathName);
    int NewLength = LenRoot + LenPart;
    NewPath = (char*)malloc(sizeof(char) * NewLength);
    memset(NewPath, 0 ,sizeof(char) * NewLength);
    for(int i = 0; i < LenRoot; i ++)
    {
        NewPath[i] = Root[i];
    }
    for(int i = 0; i < LenPart; i ++)
    {
        NewPath[i + LenRoot] = PathName[i];
    }
    for(int i = 0; i < strlen(NewPath); i ++)
    {
        if(i >= NewLength)
        {
            NewPath[i] = 0;
        }
    }
    return NewPath;
}

/*
描述：获得相对根目录的目录
参数：目录名
返回：目录，失败是NULL
*/
char* GetPartPath(char* PathName)
{
    char* NewPath = NULL;
    if(Root == NULL)
    {
        return NULL;
    }
    if(PathName == NULL)
    {
        return NULL;
    }
    printf("%s\n%s\n",Root, PathName);
    if(PathName[0] != '/')
    {
        //相对目录不处理
        int NewLength = strlen(PathName);
        NewPath = (char*)malloc(sizeof(char) * NewLength);
        memset(NewPath, 0 ,sizeof(char) * NewLength);
        for(int i = 0; i < NewLength; i ++)
        {
            NewPath[i] = PathName[i];
        }
        for(int i = 0; i < strlen(NewPath); i ++)
        {
            if(i >= NewLength)
            {
                NewPath[i] = 0;
            }
        }
        return NewPath;
    }
    int LenRoot = strlen(Root);
    if(Root[LenRoot - 1] == '/')
    {
        LenRoot -= 1;
    }
    int LenFull = strlen(PathName);
    if(LenFull < LenRoot)
    {
        return NULL;
    }
    for(int i = 0; i < LenRoot; i ++)
    {
        if(Root[i] != PathName[i])
        {
            return NULL;
        }
    }
    if(LenFull == LenRoot)
    {
        int NewLength = 1;
        NewPath = (char*)malloc(1);
        memset(NewPath, 0 , 1);
        NewPath[0] = '/';
        for(int i = 0; i < strlen(NewPath); i ++)
        {
            if(i >= NewLength)
            {
                NewPath[i] = 0;
            }
        }
        return NewPath;
    }

    int NewLength = LenFull - LenRoot;
    NewPath = (char*)malloc(sizeof(char) * NewLength);
    memset(NewPath, 0 ,sizeof(char) * NewLength);
    for(int i = 0; i < NewLength; i ++)
    {
        NewPath[i] = PathName[i + LenRoot];
    }
    for(int i = 0; i < strlen(NewPath); i ++)
    {
        if(i >= NewLength)
        {
            NewPath[i] = 0;
        }
    }
    return NewPath;
}

/*
描述：修改当前目录，对应CWD
参数：连接，目录名
返回：成功1失败0
*/
bool ChangePath(struct Connection* Link, char PathName[])
{
    int Result = 0;
    char* AbsolutePath = NULL;
    if(Result == 0)
    {
        AbsolutePath = GetFullPath(PathName);
        if(AbsolutePath == NULL)
        {
            Result = -1;
        }
    }
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    }
    if(Result == 0)
    {
        printf("%s\n",AbsolutePath);
        Result = chdir(AbsolutePath);
    }
    if(Result == 0)
    {
        free(AbsolutePath);
    }
    if(Link -> WorkingPlace != NULL)
    {
        free(Link -> WorkingPlace);
        Link -> WorkingPlace = NULL;
    }
    Link -> WorkingPlace = getcwd(NULL, 0);
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
描述：从路径中提取文件名
参数：连接，路径名
返回：文件名，如果没有返回NULL
*/
char* GetFileName(char PathName[])
{
    if(PathName == NULL)
    {
        return NULL;
    }
    int Length = strlen(PathName);
    int i = 0;
    for(i = Length - 1; i >= 0; i --)
    {
        if(PathName[i] == '/')
        {
            break;
        }
    }
    int NewLength = Length - i + 1;
    if(NewLength == 0)
    {
        return NULL;
    }
    char* Return = (char*)malloc(NewLength * sizeof(char));
    memset(Return, 0, sizeof(char) * NewLength);
    for(int j = 0; j < NewLength; j ++)
    {
        Return[j] = PathName[i + j + 1];
    }
    return Return;
}


/*
描述：修改当前目录到根目录，对应CUDP
参数：无
返回：成功1失败0
*/
bool ChangeToRoot(struct Connection* Link)
{
    char Path[100] = "/";
    return ChangePath(Link, Path);
}

/*
描述：删除当前目录，对应RWD
参数：连接，目录名
返回：成功1失败0
*/
bool DeletePath(struct Connection* Link, char PathName[])
{
    int Result = 0;
    char* AbsolutePath = NULL;
    if(Result == 0)
    {
        AbsolutePath = GetFullPath(PathName);
        if(AbsolutePath == NULL)
        {
            Result = -1;
        }
    }
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    }
    if(Result == 0)
    {
        Result = rmdir(AbsolutePath);
    }
    if(Result == 0)
    {
        free(AbsolutePath);
    }
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
描述：输出当前目录，对应PWD
参数：连接
返回：目录
*/
char* PrintPath(struct Connection* Link)
{
    char* RelativePath = GetPartPath(Link -> WorkingPlace);
    return RelativePath;
}

/*
描述：选择当前目录，并且把它重命名为原名，看是否能成功重命名
参数：连接，目录名
返回：成功1失败0
*/
bool SelectPath(struct Connection* Link, char PathName[])
{
    int Result = 0;
    char* AbsolutePath = NULL;
    if(Result == 0)
    {
        AbsolutePath = GetFullPath(PathName);
        if(AbsolutePath == NULL)
        {
            Result = -1;
        }
    }
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    }
    if(Result == 0)
    {
        Result = access(AbsolutePath, W_OK);
    }
    if(Result == 0)
    {
        if(Link -> CurrentPlaceToBeChanged != NULL)
        {
            free(Link -> CurrentPlaceToBeChanged);
            Link -> CurrentPlaceToBeChanged = NULL;
        }
        Link -> CurrentPlaceToBeChanged = (char*) malloc (sizeof(char) * strlen(AbsolutePath));
        strcpy(Link -> CurrentPlaceToBeChanged, AbsolutePath);
    }
    if(Result == 0)
    {
        free(AbsolutePath);
    }
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
描述：选择当前目录，并且把它重命名，看是否能成功重命名
参数：连接，新名字
返回：成功1失败0
*/
bool RenamePath(struct Connection* Link, char PathName[])
{
    int Result = 0;
    if(Link -> CurrentPlaceToBeChanged == NULL)
    {
        return 0;
    }
    char* AbsolutePath = NULL;
    if(Result == 0)
    {
        AbsolutePath = GetFullPath(PathName);
        if(AbsolutePath == NULL)
        {
            Result = -1;
        }
    }
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    } 
    if(Result == 0)
    {
        Result = rename(Link -> CurrentPlaceToBeChanged, AbsolutePath);
    }
    if(Result == 0)
    {
        free(AbsolutePath);
    }
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result == 0)
    {
        if(Link -> CurrentPlaceToBeChanged != NULL)
        {
            free(Link -> CurrentPlaceToBeChanged);
            Link -> CurrentPlaceToBeChanged = NULL;
        }
    }
    if(Result == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
描述：输出当前目录下所有文件或单一文件信息，对应LIST
参数：目录/文件名
返回：无
*/
char* PrintFileList(struct Connection* Link, char* PathName)
{
    int Result = 0;
    if(Result == 0)
    {
        Result = chdir(Link -> WorkingPlace);
    }
    char Command[300] = "ls -l ";
    if(PathName != NULL)
    {
        strcat(Command, PathName);
    }
    char* Buffer = NULL;

    FILE *Pointer;
    if( (Pointer = popen(Command, "r")) == NULL )
    {
        if(Result == 0)
        {
            Result = chdir(Root);
        }
        printf("popen() error!\n");
        return NULL;
    }
    if(Result == 0)
    {
        Result = chdir(Root);
    }
    if(Result != 0)
    {
        pclose(Pointer);
        return NULL;
    }
    
    Buffer = (char*)malloc(sizeof(char) * MAX_LENGTH_SOCKET);
    memset(Buffer, 0, MAX_LENGTH_SOCKET);
    fread(Buffer, sizeof(char), MAX_LENGTH_SOCKET, Pointer);
    pclose(Pointer);
    return Buffer;
}



/*
描述：打开文件
参数：要求的工作路径，文件名
返回：文件指针，错误返回NULL
*/
FILE* OpenFile(char* WorkingPlace, char* FileName)
{
    char* OriginalWorkingPlace = Root;
    bool Success = 1;
    FILE* TheFile = NULL;
    char* AbsolutePath = NULL;
    if(Success == 1)
    {
        AbsolutePath = GetFullPath(FileName);
        if(AbsolutePath == NULL)
        {
            Success = 0;
        }
    }
    if(Success)
    {
        int Result = chdir(WorkingPlace);
        if(Result != 0)
        {
            Success = 0;
        }
    }
    if(Success)
    {
        TheFile = fopen(AbsolutePath, "rb");
    }
    if(Success)
    {
        free(AbsolutePath);
    }
    chdir(OriginalWorkingPlace);
    return TheFile;
}

/*
描述：发送文件
参数：文件发送信息
返回：无
*/
void* SendFile(void* Info)
{
    struct FileInfo* RealInfo = (struct FileInfo*) Info;
    //判断文件是否为空
    printf("%s %s\n",RealInfo -> WorkingPlace, RealInfo -> FileName);
    FILE* TheFile = OpenFile(RealInfo -> WorkingPlace, RealInfo -> FileName);
    if(TheFile == NULL)
    {
        char ReturnString[300] = "451 Reading File Error\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //判断是否能找到开始位置
    int Return = fseek(TheFile, RealInfo -> StartPoint, SEEK_SET);
    if(Return != 0)
    {
        fclose(TheFile);
        char ReturnString[300] = "451 Cannot Find StartPlace\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //一个一个buffer读取文件
    char Buffer[MAX_LENGTH_SOCKET];
    memset(Buffer, 0 , MAX_LENGTH_SOCKET * sizeof(char));
    while(!feof(TheFile))
    {
        int Number = fread(Buffer, sizeof(char), MAX_LENGTH_SOCKET, TheFile);
        //printf("%d\n", Number);

        unsigned int Pointer = 0;
	    while (Pointer < Number) 
        {
		    int Len = write(RealInfo -> ConnectionID, Buffer + Pointer, Number - Pointer);
		    if (Len < 0) 
            {
			    fclose(TheFile);
                char ReturnString[300] = "426 Connection Broken\r\n";
                WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
                close(RealInfo -> ConnectionID);
                DeleteFileInfo(*RealInfo);
                free(RealInfo);
                pthread_exit(0);
	 	    } 
            else 
            {
			    Pointer += Len;
		    }			
	    }
        //printf("%d\n", Pointer);

    }


    fclose(TheFile);
    char ReturnString[300] = "226 Sending Data Success\r\n";
    WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
    close(RealInfo -> ConnectionID);
    DeleteFileInfo(*RealInfo);
    free(RealInfo);
    pthread_exit(0);
}


/*
描述：接收文件，保存在旧文件后面
参数：文件发送信息
返回：无
*/
void* ReceiveFileAppend(void* Info)
{

    struct FileInfo* RealInfo = (struct FileInfo*) Info;
    //寻找文件

    char* StoreFileName = GetFileName(RealInfo -> FileName);
    if(StoreFileName == NULL)
    {
        char ReturnString[300] = "451 Writing File Error\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }
    //打开文件
    char* OriginalWorkingPlace = Root;
    chdir(RealInfo -> WorkingPlace);
    FILE* TheFile = fopen(StoreFileName, "rb+");
    if(TheFile == NULL && RealInfo -> StartPoint == 0)
    {
        TheFile = fopen(StoreFileName, "wb+");
    }
    chdir(OriginalWorkingPlace);
    if(TheFile == NULL)
    {
        char ReturnString[300] = "451 Writing File Error\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //判断是否能找到开始位置
    int Return = fseek(TheFile, RealInfo -> StartPoint, SEEK_SET);
    if(Return != 0)
    {
        fclose(TheFile);
        char ReturnString[300] = "451 Cannot Find StartPlace\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //写入文件
    char Data[MAX_LENGTH_SOCKET];
    memset(Data, 0, sizeof(char) * MAX_LENGTH_SOCKET);
    while(1)
    {
        int Length = read(RealInfo -> ConnectionID, Data, MAX_LENGTH_SOCKET);
        printf("%d\n", Length);
        //连接断开
        if(Length < 0)
        {
            fclose(TheFile);
            char ReturnString[300] = "426 Connection Broken\r\n";
            WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
            close(RealInfo -> ConnectionID);
            DeleteFileInfo(*RealInfo);
            free(RealInfo);
            pthread_exit(0);
        
        }
        if(Length == 0)
        {
            break;
        }


        /*if(Data[Length - 1] == EOF || Data[Length - 1] == 0)
        {
            fwrite(Data, sizeof(char), Length - 1, TheFile);
            break;
        }*/
        fwrite(Data, sizeof(char), Length, TheFile);
    }

    fclose(TheFile);
    char ReturnString[300] = "226 Receiving Data Success\r\n";
    WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
    close(RealInfo -> ConnectionID);
    DeleteFileInfo(*RealInfo);
    free(RealInfo);
    pthread_exit(0);
}

/*
描述：接收文件，保存在新文件里
参数：文件发送信息
返回：无
*/
void* ReceiveFileNew(void* Info)
{
    struct FileInfo* RealInfo = (struct FileInfo*) Info;    
    //寻找文件
    char* StoreFileName = GetFileName(RealInfo -> FileName);
    if(StoreFileName == NULL)
    {
        char ReturnString[300] = "451 Writing File Error\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //打开文件
    FILE* TheFile = fopen(StoreFileName, "wb");
    if(TheFile == NULL)
    {
        char ReturnString[300] = "451 Writing File Error\r\n";
        WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
        close(RealInfo -> ConnectionID);
        DeleteFileInfo(*RealInfo);
        free(RealInfo);
        pthread_exit(0);
    }

    //写入文件
    char Data[MAX_LENGTH_SOCKET];
    memset(Data, 0, sizeof(char) * MAX_LENGTH_SOCKET);
    while(1)
    {
        int Length = read(RealInfo -> ConnectionID, Data, MAX_LENGTH_SOCKET);
        //printf("%d\n", Length);
        //连接断开
        if(Length < 0)
        {
            fclose(TheFile);
            char ReturnString[300] = "426 Connection Broken\r\n";
            WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
            close(RealInfo -> ConnectionID);
            DeleteFileInfo(*RealInfo);
            free(RealInfo);
            pthread_exit(0);
        
        }
        if(Length == 0)
        {
            break;
        }


        if(Data[Length - 1] == EOF || Data[Length - 1] == 0)
        {
            fwrite(Data, sizeof(char), Length - 1, TheFile);
            break;
        }
        fwrite(Data, sizeof(char), Length, TheFile);
    }

    fclose(TheFile);
    char ReturnString[300] = "226 Receiving Data Success\r\n";
    WriteMessage(RealInfo -> ControlID, ReturnString, strlen(ReturnString));
    close(RealInfo -> ConnectionID);
    DeleteFileInfo(*RealInfo);
    free(RealInfo);
    pthread_exit(0);
}
