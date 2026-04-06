/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: vrrpFuncParse.c
 描述: 提供其它进程调用的库函数使用vrrp模块的参数传递机制进行，此文件完成解析
     从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#include <libipc/vos.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/smdCmdWrap.h>
#include <libipc/quaggaTask.h>
#include <libsal/sal_l3_vrrp.h>

#include "vrrpd.h"
#include "vrrpFuncParse.h"

/*********************************外部声明*************************************/

/***********************************数据***************************************/
SMD_WRAP_CMD vrrpWrap[VRRP_FUNC_MAX];

/***********************************函数***************************************/

/*******************************************************************************
函数名称：vrrpWrap_VsrvConfig
功能描述：Vrrp_VsrvConfig的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrap_VsrvConfig(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    if ( 4 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    return Vrrp_VsrvConfig(*((int *)args[0].pBuf), *((int *)args[1].pBuf), 
        *((ulong_t *)args[2].pBuf), *((int *)args[3].pBuf));
}

/*******************************************************************************
函数名称：vrrpWrapVsrvDellAllByIfindex
功能描述：vrrpVsrvDellAllByIfindex的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvDellAllByIfindex(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvDellAllByIfindex(*((ulong_t *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvPrioritySet
功能描述：vrrpVsrvPrioritySet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvPrioritySet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 4 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvPrioritySet(*((ulong_t *)args[0].pBuf), 
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvPreemptSet
功能描述：vrrpVsrvPreemptSet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvPreemptSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 5 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvPreemptSet(*((ulong_t *)args[0].pBuf), 
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), 
        *((ulong_t *)args[3].pBuf), args[4].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvAdverTimeSet
功能描述：vrrpVsrvAdverTimeSet的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvAdverTimeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 4 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvAdverTimeSet(*((ulong_t *)args[0].pBuf), 
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvGetNext
功能描述：vrrpVsrvGetNext的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvGetNext(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvGetNext(*((int *)args[0].pBuf), args[1].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvIfTrackAdd
功能描述：vrrpVsrvIfTrackAdd的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvIfTrackAdd(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 5 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvIfTrackAdd(*((ulong_t *)args[0].pBuf), 
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), 
        *((ulong_t *)args[3].pBuf), args[4].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrapVsrvIfTrackDel
功能描述：vrrpVsrvIfTrackDel的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapVsrvIfTrackDel(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 4 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpVsrvIfTrackDel(*((ulong_t *)args[0].pBuf), 
        *((ulong_t *)args[1].pBuf), *((ulong_t *)args[2].pBuf), args[3].pBuf);
}

/*******************************************************************************
函数名称：vrrpWrap_routeProtoReg
功能描述：Vrrp_routeProtoReg的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrap_routeProtoReg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return Vrrp_routeProtoReg(args[0].pBuf, *((ulong_t *)args[1].pBuf));
}

/*******************************************************************************
函数名称：vrrpWrap_routeProtoDeReg
功能描述：Vrrp_routeProtoDeReg的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrap_routeProtoDeReg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return Vrrp_routeProtoDeReg(args[0].pBuf, *((ulong_t *)args[1].pBuf));
}

#ifdef GLN_INCLUDE_TRACK
/*******************************************************************************
函数名称：vrrpWrapTrackStateChg
功能描述：vrrpTrackStateChg的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int vrrpWrapTrackStateChg(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);
    
    if ( 3 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }
    
    return vrrpTrackStateChg(*((ulong_t *)args[0].pBuf),
        *((int *)args[1].pBuf), *((int *)args[2].pBuf));
}
#endif

/******************************************************************************
函数名称：vrrpAppWrapFunction
功能描述：注册库函数消息流解析函数
输入参数：无
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
          VD_E_PARAM，参数错误  
其它说明：在进程接收消息之前调用
******************************************************************************/
int vrrpAppWrapFunction(void)
{
    int iRet = VOS_ERR_NONE;

    memset(vrrpWrap, 0, (VRRP_FUNC_MAX * sizeof(SMD_WRAP_CMD)));
    if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:smdRMWrapInit failed", __func__);
        return iRet;
    }

    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_CONFIG_SET, 0, vrrpWrap_VsrvConfig, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_DEL_ALL_BY_IFINDEX, 0, vrrpWrapVsrvDellAllByIfindex, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_PRIO_SET, 0, vrrpWrapVsrvPrioritySet, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_PREEMPT_SET, 0, vrrpWrapVsrvPreemptSet, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_ADVER_TIME_SET, 0, vrrpWrapVsrvAdverTimeSet, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_GETNEXT, 0, vrrpWrapVsrvGetNext, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_IFTRACK_ADD, 0, vrrpWrapVsrvIfTrackAdd, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_VSRV_IFTRACK_DEL, 0, vrrpWrapVsrvIfTrackDel, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_ROUTE_PROTO_REG, 0, vrrpWrap_routeProtoReg, vrrpWrap, VRRP_FUNC_MAX);
    iRet += smdAppWrapRegister(VRRP_FUNC_ROUTE_PROTO_DEREG, 0, vrrpWrap_routeProtoDeReg, vrrpWrap, VRRP_FUNC_MAX);
#ifdef GLN_INCLUDE_TRACK
    iRet += smdAppWrapRegister(VRRP_FUNC_TRACK_STATE_CHG, 0, vrrpWrapTrackStateChg, vrrpWrap, VRRP_FUNC_MAX);
#endif

    if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int vrrpAppWrapProcess(SMD_MSG *pMsg)
{
	int iRet = SYS_ERR_OK;
	iRet = smdCfgAppWrapProcess(pMsg, vrrpWrap, VRRP_FUNC_MAX);
	if ( SYS_ERR_OK != iRet )
    {
        osal_printf( "%s:register failed", __func__);
    }
    return iRet;
}

