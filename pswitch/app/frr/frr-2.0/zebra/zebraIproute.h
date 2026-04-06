/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelayserverFuncParse.h
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#ifndef _INCLUDE_ZEBRAIPROUTE_H_
#define _INCLUDE_ZEBRAIPROUTE_H_
#include "zebra.h"

#include <libipc/smdMsgRequest.h>
#include <libipc/quaggaTask.h>

/***********************************数据***************************************/


/***********************************函数***************************************/
extern int zebraIpRouteAdd(char *strNetwork, char *strNetmask, char *strGateway, ulong_t ulDistance, ulong_t ulFlags);
extern int zebraIpRouteDelete(char *strNetwork, char *strNetmask, char *strGateway, ulong_t ulDistance, ulong_t ulFlags);
extern int zebraIpv6RouteAdd(char *strIpPrefixLen, char *strGateway, char *strIfName);
extern int zebraIpv6RouteDelete(char *strIpPrefixLen, char *strGateway, char *strIfName);
extern int zebraIpRouteGet (DATA_OP_ENT op, const char *vrf_name, safi_t safi, ZEBRA_IP_ROUTE_STRC *pRoute);
extern int zebraIpv6RouteGet(DATA_OP_ENT op, const char *vrf_name, ZEBRA_IPV6_ROUTE_STRC *pIpv6Route);
#endif /*_INCLUDE_DHCPRELAYSERVERFUNCPARSE_H_*/

