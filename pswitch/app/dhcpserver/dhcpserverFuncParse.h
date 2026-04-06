/*******************************************************************************
 Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
 File Name: dhcprelayserverFuncParse.h
 Description: This file provides library functions for other processes to call 
              using the parameter passing mechanism of the DHCP relay/server module.
              This file parses the message stream passed from the library functions
              and calls the corresponding processing functions.
Autor: Kniazkov Andrew
*******************************************************************************/
/***********************************Headers Files*************************************/
#ifndef _INCLUDE_DHCPSERVERFUNCPARSE_H_
#define _INCLUDE_DHCPSERVERFUNCPARSE_H_

/***********************************Data***************************************/
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

/***********************************Function***************************************/

/******************************************************************************
Function: dhcprelayserverRmWrapFunction
Description: Registers a library function and message stream parsing function.
Input: None
Output: None
Return: VD_E_NONE (success), VD_E_FAIL (failure), VD_E_PARAM (parameter error)
Additional Notes: Called before a process receives a message.
******************************************************************************/
int dhcpserverAppWrapFunction(void);
int dhcpserverAppWrapProcess(SMD_MSG *pMsg);

#endif /*_INCLUDE_DHCPRELAYSERVERFUNCPARSE_H_*/

