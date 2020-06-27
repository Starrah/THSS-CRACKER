/*
    文件名：RequestHandle.h
    描述：request切分函数
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "RequestHandle.h"


/*
功能：判断请求头是否合法
参数：请求头四个字母
返回：失败-1,其余返回请求码
*/
int JudgeRequestHeader(char Header0, char Header1, char Header2, char Header3)
{

    if (Header0 == 'U' && Header1 == 'S' && Header2 == 'E' && Header3 == 'R')
    {
        return USER;
    }
    if (Header0 == 'P' && Header1 == 'A' && Header2 == 'S' && Header3 == 'S')
    {
        return PASS;
    }
    if (Header0 == 'P' && Header1 == 'O' && Header2 == 'R' && Header3 == 'T')
    {
        return PORT;
    }
    if (Header0 == 'P' && Header1 == 'A' && Header2 == 'S' && Header3 == 'V')
    {
        return PASV;
    }
    if (Header0 == 'R' && Header1 == 'E' && Header2 == 'T' && Header3 == 'R')
    {
        return RETR;
    }
    if (Header0 == 'S' && Header1 == 'T' && Header2 == 'O' && Header3 == 'R')
    {
        return STOR;
    }
    if (Header0 == 'S' && Header1 == 'Y' && Header2 == 'S' && Header3 == 'T')
    {
        return SYST;
    }
    if (Header0 == 'T' && Header1 == 'Y' && Header2 == 'P' && Header3 == 'E')
    {
        return TYPE;
    }
    if (Header0 == 'Q' && Header1 == 'U' && Header2 == 'I' && Header3 == 'T')
    {
        return QUIT;
    }
    if (Header0 == 'A' && Header1 == 'B' && Header2 == 'O' && Header3 == 'R')
    {
        return ABOR;
    }
    if (Header0 == 'R' && Header1 == 'E' && Header2 == 'S' && Header3 == 'T')
    {
        return REST;
    }
    if (Header0 == 'C' && Header1 == 'D' && Header2 == 'U' && Header3 == 'P')
    {
        return CDUP;
    }
    if (Header0 == 'R' && Header1 == 'N' && Header2 == 'F' && Header3 == 'R')
    {
        return RNFR;
    }
    if (Header0 == 'R' && Header1 == 'N' && Header2 == 'T' && Header3 == 'O')
    {
        return RNTO;
    }
    if (Header0 == 'L' && Header1 == 'I' && Header2 == 'S' && Header3 == 'T')
    {
        return LIST;
    }
    return -1;
}

/*
功能：判断请求头是否合法（长度3）
参数：请求头3个字母
返回：失败-1,其余返回请求码
*/
int JudgeRequestHeaderLength3(char Header0, char Header1, char Header2)
{
    if (Header0 == 'M' && Header1 == 'K' && Header2 == 'D')
    {
        return MKD;
    }
    if (Header0 == 'C' && Header1 == 'W' && Header2 == 'D')
    {
        return CWD;
    }
    if (Header0 == 'P' && Header1 == 'W' && Header2 == 'D')
    {
        return PWD;
    }
    if (Header0 == 'R' && Header1 == 'M' && Header2 == 'D')
    {
        return RMD;
    }
    return -1;
}


/*
功能：初始化FTP请求
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
struct FTPRequest InitializeRequest(char* RawRequest)
{
    struct FTPRequest TheRequest = InitializeRequestLength4(RawRequest);
    if(TheRequest.RequestType == -1)
    {
        TheRequest = InitializeRequestLength3(RawRequest);
    }
    //printf("%d\n",TheRequest.RequestType);
    return TheRequest;
}

/*
功能：初始化FTP请求(长度4)
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
struct FTPRequest InitializeRequestLength4(char* RawRequest)
{
    //初始化请求
    bool Valid = 1;
    char RequestHeader[4] = {0};
    int Length = 0;
    int RequestHeaderEncoded = -1;
    char* RequestBody = NULL;
    struct FTPRequest ReturnBody;

    if(RawRequest == NULL)
    {            
        Valid = 0;
    }
    //判断请求头是否合法
    if(Valid)
    {

        Length = strlen(RawRequest);
        //printf("%d\n",Length);
        if(Length < 4)
        {
            Valid = 0;
        }
    }
    if(Valid)
    {
        for (int i = 0; i < 4; i ++)
        {
            RequestHeader[i] = RawRequest[i];
        }
        RequestHeaderEncoded = JudgeRequestHeader(RequestHeader[0], RequestHeader[1], RequestHeader[2], RequestHeader[3]);
        if(RequestHeaderEncoded == -1)
        {
            Valid = 0;
        }
    }
    //printf("%d",RequestHeaderEncoded);
    //判断空格
    if(Valid)
    {
        if(Length > 6 && RawRequest[4] != ' ')
        {
            Valid = 0;
        }
    }

    if(Valid)
    {
        //有参数
        int BodyLength = 0;

        int FlagSrc = 5;
        if(Length > 6)
        {
            while(FlagSrc < Length && RawRequest[FlagSrc] != '\r' && RawRequest[FlagSrc] != '\n')
            {
                FlagSrc ++;
                BodyLength ++;
            }
            //printf("%d\n",BodyLength);
            RequestBody = (char*)malloc(sizeof(char) * BodyLength);
            memset(RequestBody, 0, sizeof(char) * BodyLength);
            for(int i = 0; i < BodyLength; i ++)
            {
                RequestBody[i] = RawRequest[i + 5];
            }
            for(int i = BodyLength; i < strlen(RequestBody); i ++)
            {
                RequestBody[i] = 0;
            }
        }
    }
    
    if(Valid)
    {
        ReturnBody.RequestType = RequestHeaderEncoded;
        ReturnBody.Parameter = RequestBody; 
    }
    else
    {
        ReturnBody.RequestType = -1;
        ReturnBody.Parameter = NULL;
    }
    return ReturnBody;
}


/*
功能：初始化FTP请求(长度3)
参数：请求的字符串
返回：请求结构体。如果请求不合法返回Null
*/
struct FTPRequest InitializeRequestLength3(char* RawRequest)
{
    //初始化请求
    bool Valid = 1;
    char RequestHeader[3] = {0};
    int Length = 0;
    int RequestHeaderEncoded = -1;
    char* RequestBody = NULL;
    struct FTPRequest ReturnBody;

    if(RawRequest == NULL)
    {            
        Valid = 0;
    }
    //判断请求头是否合法
    if(Valid)
    {

        Length = strlen(RawRequest);
        //printf("%d\n",Length);
        if(Length < 3)
        {
            Valid = 0;
        }
    }
    if(Valid)
    {
        for (int i = 0; i < 3; i ++)
        {
            RequestHeader[i] = RawRequest[i];
        }
        RequestHeaderEncoded = JudgeRequestHeaderLength3(RequestHeader[0], RequestHeader[1], RequestHeader[2]);
        if(RequestHeaderEncoded == -1)
        {
            Valid = 0;
        }
    }
    //printf("%d",RequestHeaderEncoded);
    //判断空格
    if(Valid)
    {
        if(Length > 5 && RawRequest[3] != ' ')
        {
            Valid = 0;
        }
    }

    if(Valid)
    {
        //有参数
        int BodyLength = 0;

        int FlagSrc = 4;
        if(Length > 5)
        {
            while(FlagSrc < Length && RawRequest[FlagSrc] != '\r' && RawRequest[FlagSrc] != '\n')
            {
                FlagSrc ++;
                BodyLength ++;
            }
            //printf("%d\n",BodyLength);
            RequestBody = (char*)malloc(sizeof(char) * BodyLength);
            memset(RequestBody, 0, sizeof(char) * BodyLength);
            for(int i = 0; i < BodyLength; i ++)
            {
                RequestBody[i] = RawRequest[i + 4];
            }
            for(int i = BodyLength; i < strlen(RequestBody); i ++)
            {
                RequestBody[i] = 0;
            }
        }
    }
    
    if(Valid)
    {
        ReturnBody.RequestType = RequestHeaderEncoded;
        ReturnBody.Parameter = RequestBody; 
    }
    else
    {
        ReturnBody.RequestType = -1;
        ReturnBody.Parameter = NULL;
    }
    return ReturnBody;
}

/*
功能：删除FTP请求
返回：无
*/
void DeleteRequest(struct FTPRequest Request)
{
    if(Request.Parameter != NULL)
    {
        free(Request.Parameter);
    }
}

/*
功能：拷贝链接信息
返回：新信息
*/
struct ConnectPlace CopyPlace(const struct ConnectPlace Place)
{
    struct ConnectPlace NewPlace;
    NewPlace.Port = Place.Port;
    NewPlace.IP = (char*)malloc(sizeof(char)*20);
    memset(NewPlace.IP, 0 ,sizeof(char) * 20);
    memcpy(NewPlace.IP, Place.IP, strlen(Place.IP));
    return NewPlace;
}

/*
功能：删除链接信息
返回：无
*/
void DeletePlace(struct ConnectPlace Place)
{
    free(Place.IP);
}

/*
功能：解析IP端口
参数：请求的参数字符串
返回：IP端口(错误的话IP是NULL)
*/
struct ConnectPlace GetIPAndPort(char* Parameter)
{
    struct ConnectPlace Place;
    Place.IP = NULL;
    Place.Port = -1;
    bool Success = 1;
    int Length = -1;
    int StartPlace[6] = {0,-1,-1,-1,-1,-1};
    int EndPlace[6] = {-1,-1,-1,-1,-1,-1};
    int MiddlePlace[5] = {-1,-1,-1,-1,-1};
    int IP[6] = {-1, -1, -1, -1, -1, -1};
    if(Parameter == NULL)
    {
        Success = 0;
        return Place;
    }

    if(Success)
    {
        Length = strlen(Parameter);
        if(Length < 11)
        {
            Success = 0;
            return Place;
        }
    }
    
    //解析逗号
    if(Success)
    {
        EndPlace[5] = Length - 1;
        int MiddleNumber = 0;
        for(int i = 0; i < Length; i ++)
        {
            if(Parameter[i] == ',')
            {
                if(MiddleNumber >= 5)
                {
                    Success = 0;
                    break;
                }
                MiddlePlace[MiddleNumber] = i;
                MiddleNumber ++;
            }
        }
    }

    if(Success)
    {
        for(int i = 0; i < 5; i ++)
        {
            EndPlace[i] = MiddlePlace[i] - 1;
            StartPlace[i + 1] = MiddlePlace[i] + 1;
        }
        for(int i = 0; i < 6; i ++)
        {
            if(StartPlace[i] < 0 || StartPlace[i] >= Length || EndPlace[i] < 0 || EndPlace[i] >= Length)
            {
                Success = 0;
                break;
            }
        }
    }

    if(Success)
    {
        for(int i = 0; i < 6; i ++)
        {
            char Temp[100] = {0};
            for(int j = StartPlace[i]; j <= EndPlace[i]; j ++)
            {
                Temp[j - StartPlace[i]] = Parameter[j];
            }
            IP[i] = SwitchToIntIP(Temp);
            if(IP[i] == -1)
            {
                Success = 0;
                break;
            }
        }
    }

    if(Success)
    {
        Place.IP = IPListToString(IP[0], IP[1], IP[2], IP[3]);
        Place.Port = IP[4] * 256 + IP[5];
    }
    return Place;
}


/*
功能：判断一个字符串是不是非负整数
参数：字符串
返回：是1否0
*/
bool JudgeInt(char* Str)
{
    if(Str == NULL)
    {
        return 0;
    }

	int length = strlen(Str);
	if (Str[0] == '0' && Str[1] != 0)
	{
		return 0;
	}
	if (length > 10)
	{
		return 0;
	}
	else
	{
		int i;
		for (i = 0; i < length; i++)
		{
			if (Str[i] > '9' || Str[i] < '0')
			{
				return 0;
			}
		}

		if (length == 10)
		{
			if (Str[0] > '2')
			{
				return 0;
			}
			if (Str[0] == '2')
			{
				int k = 1, n = 0;
				for (i = 9; i >= 1; i--)
				{
					n += (Str[i] - '0')*k;
					k *= 10;
				}

				if (n > 147483647)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

/*
功能：将一个数字字符串转化为数字
参数：字符串
返回：数字，如果不是数字，返回-1
*/
int SwitchToInt(char* Str)
{
    if(JudgeInt(Str))
    {
	    int k = 1, n = 0,i;
	    int length = strlen(Str);
	    for (i = length - 1; i >= 0; i--)
	    {
		    n += (Str[i] - '0') * k;
		    k *= 10;
	    }
	    return n;
    }
    return -1;
}

/*
功能：将一个数字字符串转化为IP和端口的数字
参数：字符串
返回：数字，如果不合法，返回-1
*/
int SwitchToIntIP(char* Str)
{
    int a = SwitchToInt(Str);
    if(a >= 0 && a <= 255)
    {
        return a;
    }
    return -1;
}

/*
描述：把IP地址转化为字符串
参数：IP的4个正整数
返回：字符串
*/
char* IPListToString(int IP1, int IP2, int IP3, int IP4)
{
    //printf("%d %d %d %d\n",IP1,IP2,IP3,IP4);
    char* IPList = (char*)malloc(20 * sizeof(char));
    memset(IPList, 0 ,sizeof(char) * 20);
    int Len = 0;
    char* A1 = IntToString(IP1);   
    char* A2 = IntToString(IP2);
    char* A3 = IntToString(IP3);
    char* A4 = IntToString(IP4);
    int Len1 = 0, Len2 = 0, Len3 = 0, Len4 = 0;
    if(IP1 < 10)
    {
        Len1 = 1;
    }
    else if(IP1 < 100)
    {
        Len1 = 2;
    }
    else
    {
        Len1 = 3;
    }
    if(IP2 < 10)
    {
        Len2 = 1;
    }
    else if(IP2 < 100)
    {
        Len2 = 2;
    }
    else
    {
        Len2 = 3;
    }
    if(IP3 < 10)
    {
        Len3 = 1;
    }
    else if(IP3 < 100)
    {
        Len3 = 2;
    }
    else
    {
        Len3 = 3;
    }
    if(IP4 < 10)
    {
        Len4 = 1;
    }
    else if(IP4 < 100)
    {
        Len4 = 2;
    }
    else
    {
        Len4 = 3;
    }

    //printf("%s %s %s %s\n", A1, A2, A3, A4);
    //printf("%d %d %d %d\n", Len1, Len2, Len3, Len4);

    for(int i = 0; i < Len1; i ++)
    {
        IPList[Len] = A1[i];
        Len ++;
    }
    IPList[Len] = '.';
    Len ++;
    for(int i = 0; i < Len2; i ++)
    {
        IPList[Len] = A2[i];
        Len ++;
    }
    IPList[Len] = '.';
    Len ++;
    for(int i = 0; i < Len3; i ++)
    {
        IPList[Len] = A3[i];
        Len ++;
    }
    IPList[Len] = '.';
    Len ++;
    for(int i = 0; i < Len4; i ++)
    {
        IPList[Len] = A4[i];
        Len ++;
    }
    free(A1);
    free(A2);
    free(A3);
    free(A4);
    return IPList;
}

/*
描述：把IP地址的三位正整数转化为字符串
参数：正整数
返回：字符串
*/
char* IntToString(int a)
{
    char* String = (char*)malloc(sizeof(char) * 3);
    memset(String, 0, sizeof(char) * 3);
    if(a < 10)
    {
        String[0] = a + '0';
    }
    else if (a < 100)
    {
        String[1] = (a % 10) + '0';
        String[0] = (a / 10) + '0';
    }
    else
    {
        String[0] = (a / 100) + '0';
        int Most = a / 100;
        String[1] = ((a - Most * 100) / 10) + '0';
        String[2] = (a % 10) + '0';
    }
    return String;
}

/*
描述：把IP地址字符串和端口数转化为输出标准格式
参数：IP地址，端口
返回：字符串
*/
char* IPPortToReturnString(char IP[], int Port)
{
    char* Return = (char*)malloc(30 * sizeof(char));
	memset(Return, 0, 30 * sizeof(char));
	int Length = 0;
	for(int i = 0; i < strlen(IP); i ++)
	{
		if(IP[i] == '.')
		{
			Return[Length] = ',';
		}
		else
		{
			Return[Length] = IP[i];
		}
		Length ++;
	}
	int Port0 = (Port) / 256;
	int Port1 = (Port) % 256;
    //printf("%d %d\n", Port0, Port1);
	char* A0 = IntToString(Port0);   
    char* A1 = IntToString(Port1);
	int Len0 = 0, Len1 = 0;
	if(Port0 < 10)
	{
		Len0 = 1;
	}
	else if(Port0 < 100)
	{
		Len0 = 2;
	}
	else
	{
		Len0 = 3;
	}
	if(Port1 < 10)
	{
		Len1 = 1;
	}
	else if(Port1 < 100)
	{
		Len1 = 2;
	}
	else
	{
		Len1 = 3;
	}
	Return[Length] = ',';
	Length ++;
	for(int i = 0; i < Len0; i ++)
	{
		Return[Length] = A0[i];
		Length ++;
	}
	Return[Length] = ',';
	Length ++;
	for(int i = 0; i < Len1; i ++)
	{
		Return[Length] = A1[i];
		Length ++;
	}
    free(A0);
    free(A1);
    return Return;
}