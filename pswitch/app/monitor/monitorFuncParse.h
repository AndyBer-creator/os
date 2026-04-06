/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelayserverFuncParse.h
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#ifndef _INCLUDE_MONITORFUNCPARSE_H_
#define _INCLUDE_MONITORFUNCPARSE_H_

/***********************************数据***************************************/
/*vos功能函数名称*/
typedef enum {
	MONITOR_FUNC_OPERATE,
    MONITOR_FUNC_MAX
}MONITOR_FUNC_ENT;

/***********************************函数***************************************/

int monitorAppWrapFunction(void);
int monitorAppWrapProcess(SMD_MSG *pMsg);

#endif /*_INCLUDE_DHCPRELAYSERVERFUNCPARSE_H_*/

