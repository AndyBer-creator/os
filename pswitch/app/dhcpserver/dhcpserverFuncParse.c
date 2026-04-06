/*******************************************************************************
Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
 File Name: dhcprelaysFuncParse.c
 Description: This file provides library functions for other processes to call
              using the parameter passing mechanism of the DHCP relay/server module.
              It parses the message stream passed from the library functions and 
              calls the corresponding processing functions.
Autor: Kniazkov Andrew
*******************************************************************************/
/***********************************HEADERS FILE*************************************/
#include <common/sys_def.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdCmdWrap.h>

//#include <h/switch/smd/smdDebug.h>
//#include <h/l3/dhcprelayserver/dhcp.h>
//#include <h/l3/dhcprelayserver/dhcprelayserverTask.h>
#include <libsal/sal_dhcp_server.h>

#include "dhcpserverFuncParse.h"
#include "dhcpSocket.h"
#include "dhcpr.h"
#include "ip_pool.h"
#include "dhcprelayserver.h"


/*********************************External Declaration*************************************/

/**************************************************************************/
SMD_WRAP_CMD dhcpserverWrap[DHCPS_FUNC_MAX];

/***********************************Function***************************************/

#if 0
/*******************************************************************************
Function: dhcprealyserverWrap_setzero_by_vlanif
Description: Command parsing function for dhcprelay_setzero_by_vlanif
Input: pMsg - message
       args - command parameters
       ulArgsNum - number of command parameters
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_setzero_by_vlanif(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelay_setzero_by_vlanif(*((ulong_t *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapCheckGroupUsed
Description: Command parsing function for dhcprealycheckGroupUsed
Input : pMsg - message
        args - command parameters
        ulArgsNum - number of command parameters
Output : None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapCheckGroupUsed(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelayCheckGroupUsed(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapCheckGroupUsed
Description: Command parsing function for dhcprelayIfGroupCfg
Input : pMsg, message
        args, command parameters
        ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, SYS_ERR_FAILED, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIfGroupCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelayIfGroupCfg(*((ulong_t *)args[0].pBuf),
        *((BOOL *)args[1].pBuf), *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIfGroupOp
Description: Command parsing function for dhcprelayIfGroupOp
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIfGroupOp(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 3 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelayIfGroupOp(*((DATA_OP_ENT *)args[0].pBuf),
        args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapPortlistEnableGet
Description: Command parsing function for dhcprelayPortlistEnableGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapPortlistEnableGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelayPortlistEnableGet(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapPortEnableSet
Description: Command parsing function for dhcprelayPortEnableSet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapPortEnableSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcprelayPortEnableSet(*((ulong_t *)args[0].pBuf),
        *((BOOL *)args[1].pBuf));
}
#endif

/*******************************************************************************
Function: dhcprealserverWrapHashDataGet
Description: Command parsing function for dhcpserverHashDataGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapHashDataGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
	DHCP_SERVER_CMD_DBG();

    if ( 4 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpserverHashDataGet(*((ushort_t *)args[0].pBuf), args[1].pBuf,
        args[2].pBuf, args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrapHashDataGetByKey
Description: Command parsing function for dhcpseverHashDataGetByKey
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapHashDataGetByKey(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpseverHashDataGetByKey(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrapHashNumGet
Description: Command parsing function for dhcpseverHashNumGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapHashNumGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpseverHashNumGet(args[0].pBuf);
}

#if 0
/*******************************************************************************
Function: dhcprealserverWrapTrapsCfg
Description: Command parsing function for dhcpseverTrapsCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapTrapsCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpseverTrapsCfg(*((BOOL *)args[0].pBuf));
}
#endif
#if 1
/*******************************************************************************
Function: dhcprealyserverWrapOption82Cfg
Description: Command parsing function for dhcpoption82Cfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82Cfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82Cfg(*((BOOL *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82Cfg
Description: Command parsing function for dhcpoption82DevCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82DevCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82DevCfg(*((BOOL *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82PortCfg
Description: Command parsing function for dhcpoption82PortCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82PortCfg(*((ulong_t *)args[0].pBuf), *((BOOL *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82PortStrategyCfg
Description: Command parsing function for dhcpoption82PortStrategyCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortStrategyCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82PortStrategyCfg(*((ulong_t *)args[0].pBuf), *((int *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82FormatCfg
Description: Command parsing function for dhcpoption82FormatCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82FormatCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82FormatCfg(*((int *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82InfoFormatCfg
Description: Command parsing function for dhcpoption82InfoFormatCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82InfoFormatCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82InfoFormatCfg(*((int *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82FormatVerbose
Description: Command parsing function for dhcpoption82FormatVerbose
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82FormatVerbose(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82FormatVerbose(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82PortCircuitid
Description: Command parsing function for dhcpoption82PortCircuitid
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortCircuitid(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82PortCircuitid(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrapOption82PortRemoteid
Description: Command parsing function for dhcpoption82PortRemoteid
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortRemoteid(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82PortRemoteid(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82PortSuboption9
Description: Command parsing function for dhcpooption82PortSuboption9
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortSuboption9(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpooption82PortSuboption9(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}
#endif
#if 0
/*******************************************************************************
Function: dhcprealyserverWrapOption82VlanlistCfg
Description: Command parsing function for dhcpoption82VlanlistCfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82VlanlistCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82VlanlistCfg(*((BOOL *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82VlanlistStrategy
Description: Command parsing function for dhcpoption82VlanlistStrategy
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82VlanlistStrategy(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82VlanlistStrategy(*((BOOL *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82VlanlistCircuitid
Description: Command parsing function for dhcpoption82VlanlistCircuitid
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82VlanlistCircuitid(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82VlanlistCircuitid(args[0].pBuf, args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrapOption82VlanlistRemoteid
Description: Command parsing function for the dhcpoption82VlanlistRemoteid function
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82VlanlistRemoteid(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82VlanlistRemoteid(args[0].pBuf, args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82GlobalGet
Description: Command parsing function for dhcpoption82GlobalGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82GlobalGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82GlobalGet(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82PortGet
Description: Command parsing function for dhcpoption82PortGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82PortGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82PortGet(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption82VlanGet
Description: Command parsing function for dhcpoption82VlanGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption82VlanGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 3 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption82VlanGet(*((DATA_OP_ENT *)args[0].pBuf),
        args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption60Cfg
Description: Command parsing function for dhcpoption60Cfg
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption60Cfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption60Cfg(*((ulong_t *)args[0].pBuf),
        *((BOOL *)args[1].pBuf), args[2].pBuf, args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapOption60CfgGet
Description: Command parsing function for dhcpoption60CfgGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapOption60CfgGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 3 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpoption60CfgGet(*((DATA_OP_ENT *)args[0].pBuf),
        args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
Function: dhcprelayserverWrapMaxHopsSet
Description: Command parsing function for dhcpRelayMaxHopsSet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapMaxHopsSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpRelayMaxHopsSet(*((ulong_t *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprelayserverWrapMaxHopsGet
Description: Command parsing function for dhcpRelayMaxHopsGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapMaxHopsGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpRelayMaxHopsGet(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolGetPoolIndexByName
Description: Command parsing function for ipPoolGetPoolIndexByName
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolGetPoolIndexByName(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolGetPoolIndexByName(args[0].pBuf, args[1].pBuf);
}

/*******************************************************************************
Function: dhcpreallyserverWrapIpPoolCheckIsIpPoolInUse
Description: Command parsing function for ipPoolCheckIsIpPoolInUse
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolCheckIsIpPoolInUse(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolCheckIsIpPoolInUse(*((ulong_t *)args[0].pBuf));
}


/*******************************************************************************
Function: dhcprealyserverWrapIpPoolSectionUsedGet
Description: Command parsing function for ipPoolSectionUsedGet
Input: pMsg, message
       args, command parameters
       ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, SYS_ERR_FAILED, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolSectionUsedGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolSectionUsedGet(*((ulong_t *)args[0].pBuf),
        *((ulong_t *)args[1].pBuf), args[2].pBuf, args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolSectionBitmapGet
Description: Command parsing function for ipPoolSectionBitmapGet
Input: pMsg, message
       args, command parameters
       ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, SYS_ERR_FAILED, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolSectionBitmapGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolSectionBitmapGet(*((ulong_t *)args[0].pBuf),
        *((ulong_t *)args[1].pBuf), args[2].pBuf, args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolSectionBitmapGet
Description: Command parsing function for ipPoolGatewayCheck
Input: pMsg, message
       args, command parameters
       ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, SYS_ERR_FAILED, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolGatewayCheck(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolGatewayCheck(*((ulong_t *)args[0].pBuf),
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolLeaseTimeSet
Description: Command parsing function for ipPoolLeaseTimeSet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolLeaseTimeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolLeaseTimeCfg(args[0].pBuf, *((ulong_t *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolIprangeCheck
Description: Command parsing function for ipPoolIprangeCheck
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolIprangeCheck(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolIprangeCheck(*((ulong_t *)args[0].pBuf),
        *((ulong_t *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolDisableIp
Description: Command parsing function for ipPoolDisableIp
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolDisableIp(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolDisableIp(*((BOOL *)args[0].pBuf), args[1].pBuf,
        *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIppool_set_debug_enable
Description: Command parsing function for ippool_set_debug_enable
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIppool_set_debug_enable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ippool_set_debug_enable(*((ulong_t *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealserverWrap_addTo_macList
Description: Command parsing function for dhcp_snooping_addTo_macList
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_addTo_macList(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcp_snooping_addTo_macList(args[0].pBuf,
        *((ULONG *)args[1].pBuf), *((USHORT *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrap_clear_macList
Description: Command parsing function for dhcp_snooping_clear_macList
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_clear_macList(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int                      iNum = 0;

    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    iNum = dhcp_snooping_clear_macList();
    *((int *)args[0].pBuf) = iNum;
    return VD_E_NONE;
}

/*******************************************************************************
Function: dhcprealyserverWrapIPPOOL_SetOpton43_ByIndex
Description: Command parsing function for IPPOOL_SetOpton43_ByIndex
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIPPOOL_SetOpton43_ByIndex(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return IPPOOL_SetOpton43_ByIndex(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIPPOOL_GetOpton43_ByIndex
Description: Command parsing function for IPPOOL_GetOpton43_ByIndex
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIPPOOL_GetOpton43_ByIndex(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return IPPOOL_GetOpton43_ByIndex(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}
#endif
/*******************************************************************************
Function: dhcpserverWrapEnable
Description: Command parsing function for IPPOOL_GetOpton43_ByIndex
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcpserverWrapEnable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 1 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelayEnableCfg(*((ulong_t*)args[0].pBuf));
}

int dhcpserverWrapServerIPSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelayServerIPCfg(*((ulong_t*)args[0].pBuf), *((sys_ipv4_t*)args[1].pBuf));
}

int dhcpserverWrapGroupSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 3 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelayServerIPCfg(*((ulong_t*)args[0].pBuf), *((sys_ipv4_t*)args[1].pBuf));
}

int dhcpserverWrapIfGroupCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 3 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelayIfGroupCfg(*((ulong_t *)args[0].pBuf),
        *((BOOL *)args[1].pBuf), *((ulong_t *)args[2].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrapIfGroupOp
Description: Command parsing function for dhcprelayIfGroupOp
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), VD_E_FAIL (failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIfGroupOp(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 4 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return dhcprelayIfGroupOp(*((DATA_OP_ENT *)args[0].pBuf), 
        args[1].pBuf, args[2].pBuf, args[3].pBuf);
}

int dhcpserverWrapIPPoolCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcpserverIPPoolCfg(args[0].pBuf, *((BOOL *)args[1].pBuf));
}

int dhcpserverWrapGatewayCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 3 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcpserverGatewayCfg(args[0].pBuf, *((sys_ipv4_t *)args[1].pBuf), *((sys_ipv4_t *)args[2].pBuf));
}

int dhcpserverWrapSectionCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 4 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcpserverSectionCfg(args[0].pBuf, *((uint32 *)args[1].pBuf), *((sys_ipv4_t *)args[2].pBuf), *((sys_ipv4_t *)args[3].pBuf));
}

int dhcprelayWrapPortEnableCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	if ( 2 != ulArgsNum )
	{
		return SYS_ERR_FAILED;
	}

	return dhcprelayPortEnableSet(*((ulong_t *)args[0].pBuf), *((ulong_t *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealserverWrapIpBindSwitch
Description: Command parsing function for dhcpserverIpBindSwitch
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpBindSwitch(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    DHCP_SERVER_CMD_DBG("%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpserverIpBindSwitch(*((BOOL *)args[0].pBuf));
}

/*******************************************************************************
Function: dhcprealserverWrapIpBindGet
Description: Command parsing function for dhcpseverIpBindGet
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpBindGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    DHCP_SERVER_CMD_DBG("%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpseverIpBindGet(args[0].pBuf, args[1].pBuf);
}
int dhcpserverWrapIPPoolDnsCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 3 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcpserverDnsCfg(args[0].pBuf, *((ulong_t *)args[1].pBuf), *((sys_ipv4_t *)args[2].pBuf));
}

int dhcpserverWrapIPPoolDnsSuffixCfg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	if ( 2 != ulArgsNum )
	{
		return SYS_ERR_FAILED;
	}

	return dhcpserverDnsSuffixCfg(args[0].pBuf, args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrap_addTo_macList
Description: Command parsing function for dhcp_snooping_addTo_macList
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_addTo_macList(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    DHCP_SERVER_CMD_DBG("%s:start ip=0x%x", __func__, *((ULONG *)args[1].pBuf));

    if ( 4 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcp_snooping_addTo_macList(args[0].pBuf,
        *((ULONG *)args[1].pBuf), *((USHORT *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrap_delfrom_macList
Description: Command parsing function for dhcp_snooping_delfrom_macList
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_delfrom_macList(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    DHCP_SERVER_CMD_DBG("%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcp_snooping_delfrom_macList(args[0].pBuf,
        *((USHORT *)args[1].pBuf));
}

/*******************************************************************************
Function: dhcprealyserverWrap_macList_get
Description: Command parsing function for dhcp_snooping_macList_get
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrap_macList_get(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //DHCP_SERVER_CMD_DBG("%s:start", __func__);

    if ( 4 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcp_snooping_macList_get(*((ushort_t *)args[1].pBuf),
        args[1].pBuf, args[2].pBuf, args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealserverWrapUnBoundAssign
Description: Command parsing function for dhcpseverUnBoundAssign
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapUnBoundAssign(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    DHCP_SERVER_CMD_DBG();

    if ( 1 != ulArgsNum )
    {
        printf("%s:ulArgsNum %d error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return dhcpseverUnBoundAssign(*((BOOL *)args[0].pBuf));
}

int dhcprealyserverWrapIpPoolLeaseTimeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return SYS_ERR_FAILED;
    }

    return ipPoolLeaseTimeCfg(args[0].pBuf, *((ulong_t *)args[1].pBuf));
}

int dhcpserverWrapSystemMacSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 1 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelaySystemMacCfg(args[0].pBuf);
}

int dhcpserverWrapSystemOidSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 1 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelaySystemOidCfg(*((ulong_t*)args[0].pBuf));
}

int dhcpserverWrapHostnameSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 1 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }

    return dhcprelayHostnameCfg(args[0].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolDisableIp
Description: Command parsing function for ipPoolDisableIp
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), VD_E_FAIL (failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolDisableIp(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{

    if ( 4 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return ipPoolDisableIp(*((BOOL *)args[0].pBuf), args[1].pBuf,
        *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIpPoolDisableIp
Description: Command parsing function for ipPoolDisableIpGet
Input: pMsg, message
       args, command parameters
       ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, VD_E_FAIL, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIpPoolDisableIpGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{

    if ( 2 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return ipPoolDisableIpGet(args[0].pBuf, args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapCheckGroupUsed
Description: Command parsing function for dhcprealycheckGroupUsed
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), VD_E_FAIL (failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapCheckGroupUsed(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    
    if ( 3 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return dhcprelayCheckGroupUsed(*((ulong_t *)args[0].pBuf), args[1].pBuf, args[2].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapCheckStaticIpUsed
Description: Command parsing function for dhcpprelaycheckGroupUsed
Input: pMsg, message
       args, command parameters
       ulArgsNum, number of command parameters
Output: None
Return: VD_E_NONE, success, VD_E_FAIL, execution failure
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapCheckStaticIpUsed(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    
    if ( 2 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return static_mac_ip_bind_check_pool(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
Function: dhcprealyserverWrapIPPOOL_SetOpton43_ByIndex
Description: Command parsing function for IPPOOL_SetOpton43_ByIndex
Input: pMsg (message)
       args (command parameters)
       ulArgsNum (number of command parameters)
Output: None
Return: VD_E_NONE (success), VD_E_FAIL (failure)
Other Notes: None
*******************************************************************************/
int dhcprealyserverWrapIPPOOL_SetOpton43_ByName(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        return SYS_ERR_FAILED;
    }
    
    return IPPOOL_SetOpton43_ByName(args[0].pBuf, args[1].pBuf);
}

/******************************************************************************
Function: dhcpserverAppWrapFunction
Description: Registers a library function and message stream parsing function
Input: None
Output: None
Return: VD_E_NONE (success), SYS_ERR_FAILED (execution failure), VD_E_PARAM (parameter error)
Additional Notes: Called before a process receives a message
******************************************************************************/
int dhcpserverAppWrapFunction(void)
{
    int iRet = SYS_ERR_OK;

    //iRet = smdAppWrapInit(DHCPS_FUNC_MAX, pDhcpserverWrap);
    memset(dhcpserverWrap, 0, (DHCPS_FUNC_MAX * sizeof(SMD_WRAP_CMD)));
	if(dhcpserverWrap == NULL)
	{
		osal_printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
	}
	iRet += smdAppWrapRegister(DHCPS_FUNC_ENABLE, 0, dhcpserverWrapEnable, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_SERVER_IP_SET, 0, dhcpserverWrapServerIPSet, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_GROUP_SET, 0, dhcpserverWrapIfGroupCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_CHECK_GROUPNUM_USED, 0, dhcprealyserverWrapCheckGroupUsed, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IF_GROUP_OP, 0, dhcprealyserverWrapIfGroupOp, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IP_POOL_SET, 0, dhcpserverWrapIPPoolCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_GATEWAY_SET, 0, dhcpserverWrapGatewayCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_SECTION_SET, 0, dhcpserverWrapSectionCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_PORT_RELAY_ENABLE, 0, dhcprelayWrapPortEnableCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_IP_BIND_SWITCH_SET, 0, dhcprealyserverWrapIpBindSwitch, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_IP_BIND_SWITCH_GET, 0, dhcprealyserverWrapIpBindGet, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_ADDTO_MACLIST, 0, dhcprealyserverWrap_addTo_macList, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_DELFROM_MACLIST, 0, dhcprealyserverWrap_delfrom_macList, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_MACLIST_GET, 0, dhcprealyserverWrap_macList_get, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_UNBIND_ASSIGN_SET, 0, dhcprealyserverWrapUnBoundAssign, dhcpserverWrap, DHCPS_FUNC_MAX);

	iRet += smdAppWrapRegister(DHCPS_FUNC_DNS_IP_SET, 0, dhcpserverWrapIPPoolDnsCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_DNS_SUFFIX_SET, 0, dhcpserverWrapIPPoolDnsSuffixCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_LEASETIME_SET, 0, dhcprealyserverWrapIpPoolLeaseTimeSet, dhcpserverWrap, DHCPS_FUNC_MAX);

    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_CFG, 0, dhcprealyserverWrapOption82Cfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_DEV_CFG, 0, dhcprealyserverWrapOption82DevCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_PORT_CFG, 0, dhcprealyserverWrapOption82PortCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_PORT_STRATEGY_CFG, 0, dhcprealyserverWrapOption82PortStrategyCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_FORMAT_CFG, 0, dhcprealyserverWrapOption82FormatCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_INFO_FORMAT_CFG, 0, dhcprealyserverWrapOption82InfoFormatCfg, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_FORMAT_VERBOSE, 0, dhcprealyserverWrapOption82FormatVerbose, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_PORT_CIRCUITID, 0, dhcprealyserverWrapOption82PortCircuitid, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_PORT_REMOTEID, 0, dhcprealyserverWrapOption82PortRemoteid, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_OPTION82_PORT_SUBOPTION9, 0, dhcprealyserverWrapOption82PortSuboption9, dhcpserverWrap, DHCPS_FUNC_MAX);

	iRet += smdAppWrapRegister(DHCPS_FUNC_SYSTEM_MAC_SET, 0, dhcpserverWrapSystemMacSet, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_SYSTEM_OID_SET, 0, dhcpserverWrapSystemOidSet, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_HOSTNAME_SET, 0, dhcpserverWrapHostnameSet, dhcpserverWrap, DHCPS_FUNC_MAX);

    iRet += smdAppWrapRegister(DHCPS_FUNC_HASH_DATA_GET, 0, dhcprealyserverWrapHashDataGet, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_HASH_DATA_GET_BY_KEY, 0, dhcprealyserverWrapHashDataGetByKey, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_HASH_NUM_GET, 0, dhcprealyserverWrapHashNumGet, dhcpserverWrap, DHCPS_FUNC_MAX);
    iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_DISABLE_IP, 0, dhcprealyserverWrapIpPoolDisableIp, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_DISABLE_IP_GET, 0, dhcprealyserverWrapIpPoolDisableIpGet, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_STATIC_IP_CHECK_GET, 0, dhcprealyserverWrapCheckStaticIpUsed, dhcpserverWrap, DHCPS_FUNC_MAX);
	iRet += smdAppWrapRegister(DHCPS_FUNC_IPPOOL_OPTION43_SET_BY_INDEX, 0, dhcprealyserverWrapIPPOOL_SetOpton43_ByName, dhcpserverWrap, DHCPS_FUNC_MAX);

#if 0
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_VLANLIST_ENABLE, 0, dhcprealyserverWrapOption82VlanlistCfg);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_VLANLIST_STRATEGY, 0, dhcprealyserverWrapOption82VlanlistStrategy);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_VLANLIST_CIRCUITID, 0, dhcprealyserverWrapOption82VlanlistCircuitid);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_VLANLIST_REMOTEID, 0, dhcprealyserverWrapOption82VlanlistRemoteid);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_GLOBAL_GET, 0, dhcprealyserverWrapOption82GlobalGet);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_PORT_GET, 0, dhcprealyserverWrapOption82PortGet);
    iRet += smdRmWrapRegister(DHCPS_FUNC_OPTION82_VLAN_GET, 0, dhcprealyserverWrapOption82VlanGet);


    iRet += smdRmWrapRegister(DHRS_FUNC_SETZERO_BY_VLANIF, 0, dhcprealyserverWrap_setzero_by_vlanif);
    iRet += smdRmWrapRegister(DHRS_FUNC_CHECK_GROUPNUM_USED, 0, dhcprealyserverWrapCheckGroupUsed);
    iRet += smdRmWrapRegister(DHRS_FUNC_IF_GROUP_CFG, 0, dhcprealyserverWrapIfGroupCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_IF_GROUP_OP, 0, dhcprealyserverWrapIfGroupOp);
    iRet += smdRmWrapRegister(DHRS_FUNC_PORTLIST_ENABLED_GET, 0, dhcprealyserverWrapPortlistEnableGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_PORT_ENABLE, 0, dhcprealyserverWrapPortEnableSet);
    iRet += smdRmWrapRegister(DHRS_FUNC_HASH_DATA_GET, 0, dhcprealyserverWrapHashDataGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_HASH_DATA_GET_BY_KEY, 0, dhcprealyserverWrapHashDataGetByKey);
    iRet += smdRmWrapRegister(DHRS_FUNC_HASH_NUM_GET, 0, dhcprealyserverWrapHashNumGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_TRAPS_CFG, 0, dhcprealyserverWrapTrapsCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_CFG, 0, dhcprealyserverWrapOption82Cfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_DEV_CFG, 0, dhcprealyserverWrapOption82DevCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_CFG, 0, dhcprealyserverWrapOption82PortCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_STRATEGY_CFG, 0, dhcprealyserverWrapOption82PortStrategyCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_FORMAT_CFG, 0, dhcprealyserverWrapOption82FormatCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_INFO_FORMAT_CFG, 0, dhcprealyserverWrapOption82InfoFormatCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_FORMAT_VERBOSE, 0, dhcprealyserverWrapOption82FormatVerbose);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_CIRCUITID, 0, dhcprealyserverWrapOption82PortCircuitid);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_REMOTEID, 0, dhcprealyserverWrapOption82PortRemoteid);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_SUBOPTION9, 0, dhcprealyserverWrapOption82PortSuboption9);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_VLANLIST_ENABLE, 0, dhcprealyserverWrapOption82VlanlistCfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_VLANLIST_STRATEGY, 0, dhcprealyserverWrapOption82VlanlistStrategy);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_VLANLIST_CIRCUITID, 0, dhcprealyserverWrapOption82VlanlistCircuitid);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_VLANLIST_REMOTEID, 0, dhcprealyserverWrapOption82VlanlistRemoteid);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_GLOBAL_GET, 0, dhcprealyserverWrapOption82GlobalGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_PORT_GET, 0, dhcprealyserverWrapOption82PortGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION82_VLAN_GET, 0, dhcprealyserverWrapOption82VlanGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION60_CFG, 0, dhcprealyserverWrapOption60Cfg);
    iRet += smdRmWrapRegister(DHRS_FUNC_OPTION60_CFG_GET, 0, dhcprealyserverWrapOption60CfgGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_DHCP_MAXHOPS_SET, 0, dhcprealyserverWrapMaxHopsSet);
    iRet += smdRmWrapRegister(DHRS_FUNC_DHCP_MAXHOPS_GET, 0, dhcprealyserverWrapMaxHopsGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_GET_INDEX_BY_NAME, 0, dhcprealyserverWrapIpPoolGetPoolIndexByName);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_CHECK_IN_USED, 0, dhcprealyserverWrapIpPoolCheckIsIpPoolInUse);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_DELETE_DISABLE_IP, 0, dhcprealyserverWrapDeleteDisableIp);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_SECTION_USED_GET, 0, dhcprealyserverWrapIpPoolSectionUsedGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_SECTION_BITMAP_GET, 0, dhcprealyserverWrapIpPoolSectionBitmapGet);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_GATEWAY_CHECK, 0, dhcprealyserverWrapIpPoolGatewayCheck);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_LEASETIME_SET, 0, dhcprealyserverWrapIpPoolLeaseTimeSet);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_IPRANGE_CHECK, 0, dhcprealyserverWrapIpPoolIprangeCheck);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_SET_DEBUG_ENABLE, 0, dhcprealyserverWrapIppool_set_debug_enable);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_CLEAR_MACLIST, 0, dhcprealyserverWrap_clear_macList);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_OPTION43_SET_BY_INDEX, 0, dhcprealyserverWrapIPPOOL_SetOpton43_ByIndex);
    iRet += smdRmWrapRegister(DHRS_FUNC_IPPOOL_OPTION43_GET_BY_INDEX, 0, dhcprealyserverWrapIPPOOL_GetOpton43_ByIndex);
#endif
    if ( SYS_ERR_OK != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int dhcpserverAppWrapProcess(SMD_MSG *pMsg)
{
	int iRet = SYS_ERR_OK;
	iRet = smdCfgAppWrapProcess(pMsg, dhcpserverWrap, DHCPS_FUNC_MAX);
	if ( SYS_ERR_OK != iRet )
    {
        osal_printf( "%s:register failed", __func__);
    }
    return iRet;
}

