/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelaysFuncParse.c
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#include <zebra.h>

#include "prefix.h"
#include "command.h"
#include "if.h"
#include "thread.h"
#include "stream.h"
#include "memory.h"
#include "zebra_memory.h"
#include "table.h"
#include "rib.h"
#include "network.h"
#include "sockunion.h"
#include "log.h"
#include "zclient.h"
#include "privs.h"
#include "network.h"
#include "buffer.h"
#include "nexthop.h"
#include "vrf.h"

#include "zebra/zserv.h"
#include "zebra/zebra_ns.h"
#include "zebra/zebra_vrf.h"
#include "zebra/router-id.h"
#include "zebra/redistribute.h"
#include "zebra/debug.h"
#include "zebra/ipforward.h"
#include "zebra/zebra_rnh.h"
#include "zebra/rt_netlink.h"
#include "zebra/interface.h"
#include "zebra/zebra_ptm.h"
#include "zebra/rtadv.h"
#include "zebra/zebra_mpls.h"
#include "zebra/zebra_fpm.h"


#include <libipc/vos.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdCmdWrap.h>
#include <libipc/quaggaTask.h>
#include "zebra/zebraFuncParse.h"


/*********************************外部声明*************************************/
//extern int zebraRouterIdAdd(char *id_str);

/***********************************数据***************************************/
SMD_WRAP_CMD zebraWrap[FRR_FUNC_ZEBRA_END];

/***********************************函数***************************************/

/*******************************************************************************
函数名称：zebraWrapRouterIdAdd
功能描述：zebraRouterIdAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapRouterIdAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    
    if ( 1 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraRouterIdAdd(args[0].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapRouterIdDelte
功能描述：zebraRouterIdDelete的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapRouterIdDelte(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
      
    if ( 0 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraRouterIdDelete();
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapRouterIdGet
功能描述：zebraRouterIdGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapRouterIdGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1; 
    
    if ( 1 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraRouterIdGet(args[0].pBuf);
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIfShutdown
功能描述：zebraIfShutdown的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfShutdown(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
     
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIfShutdown(args[0].pBuf, *((BOOL *)args[1].pBuf));
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIfIpAddressAdd
功能描述：zebraIfIpAddressAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfIpAddressAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
      
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIfIpAddressAdd(args[0].pBuf, args[1].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIfIpAddressDelete
功能描述：zebraIfIpAddressDelete的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfIpAddressDelete(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
       
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIfIpAddressDelete(args[0].pBuf, args[1].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIpRouteAdd
功能描述：zebraIpRouteAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpRouteAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
     
    if ( 5 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIpRouteAdd(args[0].pBuf, args[1].pBuf, args[2].pBuf,
        *((ulong_t *)args[3].pBuf), *((ulong_t *)args[4].pBuf));
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIpRouteDelete
功能描述：zebraIpRouteDelete的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpRouteDelete(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
        
    if ( 5 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIpRouteDelete(args[0].pBuf, args[1].pBuf, args[2].pBuf,
        *((ulong_t *)args[3].pBuf), *((ulong_t *)args[4].pBuf));
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrapIpRouteGet
功能描述：zebraIpRouteGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpRouteGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
      
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = zebraIpRouteGet(*((DATA_OP_ENT *)args[0].pBuf), NULL, SAFI_UNICAST, args[1].pBuf);
    
    return iMsgRet;
}

/*******************************************************************************
函数名称：zebraWrap_ipv6_forwarding
功能描述：zebra_ipv6_forwarding的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrap_ipv6_forwarding(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 1 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return zebra_ipv6_forwarding(*((BOOL *)args[0].pBuf));
    return 0;
}

/*******************************************************************************
函数名称：zebraWrapIpv6RouteGet
功能描述：zebraIpv6RouteGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpv6RouteGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{  
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    return zebraIpv6RouteGet(*((DATA_OP_ENT *)args[0].pBuf), NULL, args[1].pBuf);
}

/*******************************************************************************
函数名称：zebraWrapIfIpv6AddressAdd
功能描述：zebraIfIpv6AddressAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfIpv6AddressAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return zebraIfIpv6AddressAdd(args[0].pBuf, args[1].pBuf);
    return 0;
}

/*******************************************************************************
函数名称：zebraWrapIfIpv6AddressDelete
功能描述：zebraIfIpv6AddressDelete的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfIpv6AddressDelete(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return zebraIfIpv6AddressDelete(args[0].pBuf, args[1].pBuf);
    return 0;
}

/*******************************************************************************
函数名称：zebraWrapIpv6RouteAdd
功能描述：zebraIpv6RouteAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpv6RouteAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{ 
    if ( 3 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    return zebraIpv6RouteAdd(args[0].pBuf, args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
函数名称：zebraWrapIpv6RouteDelete
功能描述：zebraIpv6RouteDelete的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIpv6RouteDelete(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{ 
    if ( 3 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    return zebraIpv6RouteDelete(args[0].pBuf, args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
函数名称：zebraWrapIfIpv6LinkLocalAddrGet
功能描述：zebraIfIpv6LinkLocalAddrGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapIfIpv6LinkLocalAddrGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 3 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return zebraIfIpv6LinkLocalAddrGet(args[0].pBuf, args[1].pBuf, args[2].pBuf);
    return 0;
}

/*******************************************************************************
函数名称：zebraWrapRibStatGet
功能描述：zebraRibStatGet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapRibStatGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 1 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return ribStatGet(args[0].pBuf);
    return 0;
}

/*******************************************************************************
函数名称：zebraWrapRibStatLinklocalAddrCount
功能描述：ribStatLinklocalAddrCount的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int zebraWrapRibStatLinklocalAddrCount(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    printf("%s:start", __func__);
    
    if ( 1 != ulArgsNum )
    {
        printf( "%s:ulArgsNum %d error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    //return ribStatLinklocalAddrCount(*((BOOL *)args[0].pBuf));
    return 0;
}

/******************************************************************************
函数名称：zebraFrrWrapFunction
功能描述：注册库函数消息流解析函数
输入参数：无
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
          VD_E_PARAM，参数错误  
其它说明：在进程接收消息之前调用
******************************************************************************/
int zebraFrrWrapFunction(void)
{
    int iRet = VOS_ERR_NONE;
    memset(zebraWrap, 0, (FRR_FUNC_ZEBRA_END * sizeof(SMD_WRAP_CMD)));
	if(zebraWrap == NULL)
	{
		printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
	}
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_ROUTERID, 0, zebraWrapRouterIdAdd, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_ROUTERID_NO, 0, zebraWrapRouterIdDelte, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_ROUTERID_GET, 0, zebraWrapRouterIdGet, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IF_SHUTDOWN, 0, zebraWrapIfShutdown, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IF_IP_ADD, 0, zebraWrapIfIpAddressAdd, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IF_IP_DELETE, 0, zebraWrapIfIpAddressDelete, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IP_ROUTE_ADD, 0, zebraWrapIpRouteAdd, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IP_ROUTE_DELETE, 0, zebraWrapIpRouteDelete, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IP_ROUTE_GET, 0, zebraWrapIpRouteGet, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_FORWARDING_SET, 0, zebraWrap_ipv6_forwarding, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_ROUTE_GET, 0, zebraWrapIpv6RouteGet, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_ADDR_ADD, 0, zebraWrapIfIpv6AddressAdd, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_ADDR_DEL, 0, zebraWrapIfIpv6AddressDelete, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_ROUTE_ADD, 0, zebraWrapIpv6RouteAdd, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_ROUTE_DEL, 0, zebraWrapIpv6RouteDelete, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_IPV6_LINKLOCAL_ADDR_GET, 0, zebraWrapIfIpv6LinkLocalAddrGet, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_STATS_GET, 0, zebraWrapRibStatGet, zebraWrap, FRR_FUNC_ZEBRA_END);
	iRet += smdAppWrapRegister(FRR_FUNC_ZEBRA_STATS_LINKLOCAL_ADDR_COUNT, 0, zebraWrapRibStatLinklocalAddrCount, zebraWrap, FRR_FUNC_ZEBRA_END);

    if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int zebraFrrWrapProcess(SMD_MSG *pMsg)
{
	int iRet = VOS_ERR_NONE;
	iRet = smdCfgAppWrapProcess(pMsg, zebraWrap, FRR_FUNC_ZEBRA_END);
	if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }
    return iRet;
}

