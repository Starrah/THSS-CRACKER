/*
    文件名：StatusChange.h
    描述：修改后端服务器状态的函数，每个函数的输入都是处理好的命令，负责状态转换。以及定义状态码
*/

#ifndef STATUSCHANGE_H
#define STATUSCHANGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "RequestHandle.h"
#include "SocketHandle.h"

/*
描述：获取控制台参数，来得到Root和端口信息
参数：参数个数，参数
返回：成功1失败0
*/
extern bool HandleArguments(int argc, char* argv[]);

/*
描述：初始化服务器
参数：无
返回：成功1失败0
*/
extern bool InitServer();

/*
描述：控制处理客户端请求的主函数
参数：ID指针
返回: 无
*/
extern void* HandleClient(void* ID);

/*
描述：处理登录请求
参数：连接
返回：无
*/
extern void HandleLogin(struct Connection* Link);

/*
描述：处理除了登录外的请求
参数：连接,类型
返回：无
*/
extern void HandleCommand(struct Connection* Link);
#endif
