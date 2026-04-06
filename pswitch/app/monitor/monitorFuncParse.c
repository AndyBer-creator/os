/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelaysFuncParse.c
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#include <common/sys_def.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdCmdWrap.h>
#include "monitorFuncParse.h"


/*********************************外部声明*************************************/

/***********************************数据***************************************/
SMD_WRAP_CMD monitorServerWrap[MONITOR_FUNC_MAX];


/***********************************函数***************************************/

int monitorWrapOperateMessage(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	//snmp_trapFormat_t Info;
	//snmp_log(LOG_DEBUG, "%s %d args1=%s args2=%s args3=%d args4=%s\n", __func__, __LINE__, (char*)args[0].pBuf ,(char*)args[1].pBuf, *(ulong_t*)args[2].pBuf, (char*)args[3].pBuf);

    if ( 4 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
   	/*操作类消息封装*/ 
	//snmpTrapOperatingMessageEncapsulation(&Info, (char *)args[0].pBuf, (char *)args[1].pBuf, *((ulong_t*)args[2].pBuf), (char*)args[3].pBuf);

	//netsnmp_cmTrap_send_mux_new(SNMP_CMTRAP_CODE_OPERATE, &Info);

    return 0;
}


/******************************************************************************
函数名称：monitorAppWrapFunction
功能描述：注册库函数消息流解析函数
输入参数：无
输出参数：无
函数返回：VD_E_NONE, 成功
          SYS_ERR_FAILED，执行失败
          VD_E_PARAM，参数错误
其它说明：在进程接收消息之前调用
******************************************************************************/
int monitorAppWrapFunction(void)
{
    int iRet = SYS_ERR_OK;

    memset(monitorServerWrap, 0, (MONITOR_FUNC_MAX * sizeof(SMD_WRAP_CMD)));
	if(monitorServerWrap == NULL)
	{
		printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
	}
	
	iRet += smdAppWrapRegister(MONITOR_FUNC_OPERATE, 0, monitorWrapOperateMessage, monitorServerWrap, MONITOR_FUNC_MAX);

    if ( SYS_ERR_OK != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int monitorAppWrapProcess(SMD_MSG *pMsg)
{
	int iRet = SYS_ERR_OK;
	iRet = smdCfgAppWrapProcess(pMsg, monitorServerWrap, MONITOR_FUNC_MAX);
	if ( SYS_ERR_OK != iRet )
    {
        printf( "%s:register failed", __func__);
    }
    return iRet;
}


