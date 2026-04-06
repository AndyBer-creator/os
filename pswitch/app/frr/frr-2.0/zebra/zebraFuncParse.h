/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: dhcprelayserverFuncParse.h
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#ifndef _INCLUDE_ZEBRAFUNCPARSE_H_
#define _INCLUDE_ZEBRAFUNCPARSE_H_
#include <libipc/smdMsgRequest.h>

/***********************************数据***************************************/
/*vos功能函数名称*/
typedef enum {
    DHRS_FUNC_SETZERO_BY_VLANIF,
    DHRS_FUNC_CHECK_GROUPNUM_USED,
    DHRS_FUNC_IF_GROUP_CFG,
    DHRS_FUNC_IF_GROUP_OP,
    DHRS_FUNC_PORTLIST_ENABLED_GET,
    DHRS_FUNC_PORT_ENABLE,
    DHRS_FUNC_HASH_DATA_GET,
    DHRS_FUNC_HASH_DATA_GET_BY_KEY,
    DHRS_FUNC_HASH_NUM_GET,
    DHRS_FUNC_TRAPS_CFG,
    DHRS_FUNC_OPTION82_CFG,
    DHRS_FUNC_OPTION82_DEV_CFG,
    DHRS_FUNC_OPTION82_PORT_CFG,
    DHRS_FUNC_OPTION82_PORT_STRATEGY_CFG,
    DHRS_FUNC_OPTION82_FORMAT_CFG,
    DHRS_FUNC_OPTION82_INFO_FORMAT_CFG,
    DHRS_FUNC_OPTION82_FORMAT_VERBOSE,
    DHRS_FUNC_OPTION82_PORT_CIRCUITID,
    DHRS_FUNC_OPTION82_PORT_REMOTEID,
    DHRS_FUNC_OPTION82_PORT_SUBOPTION9,
    DHRS_FUNC_OPTION82_VLANLIST_ENABLE,
    DHRS_FUNC_OPTION82_VLANLIST_STRATEGY,
    DHRS_FUNC_OPTION82_VLANLIST_CIRCUITID,
    DHRS_FUNC_OPTION82_VLANLIST_REMOTEID,
    DHRS_FUNC_OPTION82_GLOBAL_GET,
    DHRS_FUNC_OPTION82_PORT_GET,
    DHRS_FUNC_OPTION82_VLAN_GET,
    DHRS_FUNC_OPTION60_CFG,
    DHRS_FUNC_OPTION60_CFG_GET,
    DHRS_FUNC_DHCP_MAXHOPS_SET,
    DHRS_FUNC_DHCP_MAXHOPS_GET,
    DHRS_FUNC_IPPOOL_GET_INDEX_BY_NAME,
    DHRS_FUNC_IPPOOL_CHECK_IN_USED,
    DHRS_FUNC_IPPOOL_DELETE_DISABLE_IP,
    DHRS_FUNC_IPPOOL_SECTION_USED_GET,
    DHRS_FUNC_IPPOOL_SECTION_BITMAP_GET,
    DHRS_FUNC_IPPOOL_GATEWAY_CHECK,
    DHRS_FUNC_IPPOOL_LEASETIME_SET,
    DHRS_FUNC_IPPOOL_IPRANGE_CHECK,
    DHRS_FUNC_IPPOOL_DISABLE_IP,
    DHRS_FUNC_IPPOOL_SET_DEBUG_ENABLE,
    DHRS_FUNC_IPPOOL_ADDTO_MACLIST,
    DHRS_FUNC_IPPOOL_DELFROM_MACLIST,
    DHRS_FUNC_IPPOOL_CLEAR_MACLIST,
    DHRS_FUNC_IPPOOL_MACLIST_GET,
    DHRS_FUNC_IP_BIND_SWITCH_SET,
    DHRS_FUNC_IP_BIND_SWITCH_GET,
    DHRS_FUNC_UNBIND_ASSIGN_SET,
    DHRS_FUNC_IPPOOL_OPTION43_SET_BY_INDEX,
    DHRS_FUNC_IPPOOL_OPTION43_GET_BY_INDEX,
    DHRS_FUNC_MAX
}DHRS_FUNC_ENT;

/***********************************函数***************************************/

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
int zebraFrrWrapFunction(void);
int zebraFrrWrapProcess(SMD_MSG *pMsg);


#endif /*_INCLUDE_DHCPRELAYSERVERFUNCPARSE_H_*/

