/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelaysFuncParse.c
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#include <libipc/vos.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdCmdWrap.h>
#include <libipc/quaggaTask.h>
#include "ripFuncParse.h"
#include <zebra.h>
#include "vrf.h"
#include "if.h"
#include "command.h"
#include "prefix.h"
#include "table.h"
#include "thread.h"
#include "memory.h"
#include "log.h"
#include "stream.h"
#include "filter.h"
#include "sockunion.h"
#include "sockopt.h"
#include "routemap.h"
#include "if_rmap.h"
#include "plist.h"
#include "distribute.h"
#include "md5.h"
#include "keychain.h"
#include "privs.h"
#include "ripd/ripd.h"
#include "ripd/rip_debug.h"


/*********************************外部声明*************************************/

/***********************************数据***************************************/
SMD_WRAP_CMD ripWrap[FRR_FUNC_RIP_END];

/***********************************函数***************************************/

/*******************************************************************************
函数名称：ripFrrWrapEnable
功能描述：使能rip的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrWrapEnable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	int ret;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

	ret = rip_enable();

    return ret;
}

/*******************************************************************************
函数名称：ripFrrWrapEnable
功能描述：使能rip的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrWrapDisenable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	int ret;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

	ret = rip_disable();

    return ret;
}

/*******************************************************************************
函数名称：ripWrapNetworkSet
功能描述：rip network命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripWrapNetworkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ripNetworkSet(*((int *)args[0].pBuf), *((u_int32_t *)args[1].pBuf),
                            *((u_int32_t *)args[2].pBuf), args[3].pBuf);
    return iMsgRet;
}
/*******************************************************************************
函数名称：ripWrapVersionSet
功能描述：sal_rip_version_rip_set的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripWrapVersionSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = ripVersionSet(*((int *)args[1].pBuf), *((int *)args[0].pBuf));  
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrRouteSet
功能描述：sal_rip_route_config的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrRouteSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;   
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = ripStaticRouteSet(*((int *)args[0].pBuf), (char *)args[1].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrDistanceSet
功能描述：sal_rip_distance_config的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrDistanceSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
     
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = ripDistanceSet(*((int *)args[0].pBuf), *((int *)args[1].pBuf));  
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrDistanceSet
功能描述：sal_rip_distance_source_config的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrDistanceSourceSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1; 
    
    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = ripDistanceSourceSet(*((int *)args[0].pBuf), (char *)args[1].pBuf, (char *)args[2].pBuf);   
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrDistanceSet
功能描述：sal_rip_distribute_list_config的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrDistributeListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;  
    
    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = rip_frr_distribute_list_set(*((int *)args[0].pBuf), (char *)args[1].pBuf, (char *)args[2].pBuf, (char *)args[3].pBuf);   
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrAccesslistZebraSet
功能描述：sal_rip_access_list_config的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrAccesslistZebraSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 6 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
    iMsgRet = rip_frr_access_list_zebra_set(*((int *)args[0].pBuf), *((int *)args[1].pBuf), (char *)args[2].pBuf, (char *)args[3].pBuf, (char *)args[4].pBuf, *((int *)args[5].pBuf));  
    return iMsgRet;
}
/*******************************************************************************
函数名称：frrAccesslistWrapGetAlistInfo
功能描述：frrAccesslistWrapGetAlistInfo的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/

int frrAccesslistWrapGetAlistInfo(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = access_list_get_alist_info(*((DATA_OP_ENT *)args[0].pBuf), *((int *)args[1].pBuf),
                                args[2].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：frrAccesslistWrapGetFilterInfo
功能描述：frrAccesslistWrapGetFilterInfo的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/

int frrAccesslistWrapGetFilterInfo(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    
    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = access_list_get_filter_info(*((DATA_OP_ENT *)args[0].pBuf), *((int *)args[1].pBuf), 
                                args[2].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：frrDistributeWrapGetInfo
功能描述：frrDistributeWrapGetInfo的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/

int frrDistributeWrapGetInfo(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = frr_distribute_info_get(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);
    
    return iMsgRet;
}
//int frr_distribute_info_get(DATA_OP_ENT op,  DISTRIBUTE_INFO_T *pDistribute)
/*******************************************************************************
函数名称：ripFrrDebug
功能描述：ipc_rip_debug的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrDebug(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {	
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrRipDebugSet(*((int *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrLogToFile
功能描述：ipc_rip_log_to_fileName的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrLogToFile(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if (1!= ulArgsNum)
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrLogFileNameSet(*((int *)args[0].pBuf));

    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrDebug
功能描述：ipc_rip_log_to_stdout的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrLogToStdout(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrLogStdoutSet(*((int *)args[0].pBuf));

    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrStatusGet
功能描述：ipc_rip_status_get的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrStatusGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = ripdFrrStatusGet((int *)args[0].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrVersionGet
功能描述：ipc_rip_version_get的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrVersionGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrRipVersionGet((int *)args[0].pBuf, (int *)args[1].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrVersionGet
功能描述：ipc_rip_version_get的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int frrRipNetworkInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrRipEnableNwInfoGet(*((DATA_OP_ENT *)args[0].pBuf), (RIP_ENABLE_NW_INFO_T *)args[1].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrVersionGet
功能描述：ripWrapIpRouteGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrIpRouteGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = frrRipIpRouteGet(*((DATA_OP_ENT *)args[0].pBuf), (RIP_IP_ROUTE_STRC *)args[1].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrIpRouteNumGet
功能描述：ripRouteNumGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrIpRouteNumGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = frrRipRouteNumGet((int *)args[0].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：ripFrrStatusGet
功能描述：ipc_rip_status_get的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ripFrrInformationInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	iMsgRet = frrRipInfoGet((RIP_INFO_T *)args[0].pBuf);
    return iMsgRet;
}

/******************************************************************************
函数名称：ripFrrWrapFunction
功能描述：注册库函数消息流解析函数
输入参数：无
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
          VD_E_PARAM，参数错误  
其它说明：在进程接收消息之前调用
******************************************************************************/
int ripFrrWrapFunction(void)
{
    int iRet = VOS_ERR_NONE;
	
    memset(ripWrap, 0, (FRR_FUNC_RIP_END * sizeof(SMD_WRAP_CMD)));
	if(ripWrap == NULL)
	{
		printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
	}
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_OPEN, 0, ripFrrWrapEnable, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_CLOSE, 0, ripFrrWrapDisenable, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_NETWORK_SET, 0, ripWrapNetworkSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_VERSION_SET, 0, ripWrapVersionSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_STATIC_ROUTE_SET, 0, ripFrrRouteSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_DISTANCE_SET, 0, ripFrrDistanceSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_DISTANCE_SOURCE_SET, 0, ripFrrDistanceSourceSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_DISTRIBUTE_LIST_SET, 0, ripFrrDistributeListSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ACCESS_LIST_ZEBRA_SET, 0, ripFrrAccesslistZebraSet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_DEBUG_SET, 0, ripFrrDebug, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_LOG_TO_FILE_SET, 0, ripFrrLogToFile, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_LOG_TO_STDOUT_SET, 0, ripFrrLogToStdout, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_STATUS_GET, 0, ripFrrStatusGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_VERSION_GET, 0, ripFrrVersionGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_ENABLE_NW_INFO_GET, 0, frrRipNetworkInfoGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_ROUTE_GET, 0, ripFrrIpRouteGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_ROUTE_NUM_GET, 0, ripFrrIpRouteNumGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_RIP_STATUS_SHOW, 0, ripFrrInformationInfoGet, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ACCESS_LIST_INFO_GET, 0, frrAccesslistWrapGetAlistInfo, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ACCESS_LIST_FILTER_INFO_GET, 0, frrAccesslistWrapGetFilterInfo, ripWrap, FRR_FUNC_RIP_END);
	iRet += smdAppWrapRegister(FRR_FUNC_DISTRIBUTE_INFO_GET, 0, frrDistributeWrapGetInfo, ripWrap, FRR_FUNC_RIP_END);

    if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int ripFrrWrapProcess(SMD_MSG *pMsg)
{
	int iRet = VOS_ERR_NONE;
	iRet = smdCfgAppWrapProcess(pMsg, ripWrap, FRR_FUNC_RIP_END);
	if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }
    return iRet;
}

