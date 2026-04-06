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
#include "ospfFuncParse.h"
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
#include "ospf_vty.h"

/*********************************外部声明*************************************/

/***********************************数据***************************************/
SMD_WRAP_CMD ospfWrap[FRR_FUNC_OSPF_END];

/***********************************函数***************************************/

/*******************************************************************************
函数名称：ospfFrrWrapEnable
功能描述：使能ospf的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ospfFrrWrapEnable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	int ret;
    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	ret = ospf_open(*((ushort_t*)args[0].pBuf));

    return ret;
}

/*******************************************************************************
函数名称：ospfFrrWrapDisable
功能描述：使能ospf的命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ospfFrrWrapDisable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
	int ret;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }
	ret = ospf_close(*((ushort_t*)args[0].pBuf));

    return ret;
}

/*******************************************************************************
函数名称：ospfWrapNetworkSet
功能描述：ospf network命令解析函数
输入参数：pMsg，消息
          args，命令参数
          ulArgsNum，命令参数数目
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
其它说明：无
*******************************************************************************/
int ospfWrapAreaNetworkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospf_network_area_cfg(*((int *)args[0].pBuf), *((u_int32_t *)args[1].pBuf),
                            *((u_int32_t *)args[2].pBuf), *((struct in_addr *)args[3].pBuf), *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapAreaSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfFrrRouterIdSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;
    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRouterIdSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfFrrPassiveIntfDefaultSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfPassiveIntfDefaultSet(*((BOOL *)args[0].pBuf));

    return iMsgRet;
}

int ospfFrrPassiveIntfSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfPassiveIntfSet(*((BOOL *)args[0].pBuf), args[1].pBuf, *((struct in_addr *)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapAreaRangeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 6 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaRangeSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                               args[2].pBuf, *((int *)args[3].pBuf), *((ulong_t *)args[4].pBuf), *((ushort_t*)args[5].pBuf));

    return iMsgRet;
}


int ospfWrapAreaRangeSubstituteSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaRangeSubstituteSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                         args[2].pBuf, args[3].pBuf, *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapVirLinkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfVirLinkSet(*((BOOL *)args[0].pBuf), args[1].pBuf, *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}


int ospfWrapAreaShortCutSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaShortCutSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    args[2].pBuf, *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

 int ospfWrapStubAreaSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfStubAreaSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    *((int *)args[2].pBuf), *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}


int ospfWrapNssaAreaSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfNssaAreaSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                              args[2].pBuf, *((int *)args[3].pBuf), *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}
int ospfWrapAreaDefaultCostSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaDefaultCostSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    *((ulong_t *)args[2].pBuf), *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapAreaExportListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaExportListSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    args[2].pBuf, *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapAreaImportListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaImportListSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    args[2].pBuf, *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapAreaFilterListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaFilterListSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    args[2].pBuf, args[3].pBuf, *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapAreaAuthTypeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaAuthTypeSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                                    *((int *)args[2].pBuf),  *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapAbrTypeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAbrTypeSet(*((BOOL *)args[0].pBuf), args[1].pBuf, *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapLogAdjacencySet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfLogAdjacencySet(*((BOOL *)args[0].pBuf), *((int *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapRfc1583compatibilitySet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRfc1583compatibilitySet(*((BOOL *)args[0].pBuf), *((ushort_t*)args[1].pBuf));

    return iMsgRet;
}

int ospfWrapSpfTimerSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfSpfTimerSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf),
                              *((ulong_t *)args[2].pBuf), *((ulong_t *)args[3].pBuf), *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapStaticNeighborSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfStaticNeighborSet(*((BOOL *)args[0].pBuf), *((struct in_addr *)args[1].pBuf),
                              *((ulong_t *)args[2].pBuf), *((ulong_t *)args[3].pBuf), *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapRefreshTimerSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRefreshTimerSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapAutoCostSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAutoCostSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapIpAuthModeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpAuthModeSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                *((int *)args[2].pBuf), *((struct in_addr *)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapIpAuthSimpleSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpAuthSimpleSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                args[2].pBuf, *((struct in_addr *)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapIpAuthMd5Set(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpAuthMd5Set(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                *((ulong_t *)args[2].pBuf), args[3].pBuf, *((struct in_addr *)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapIpParamSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpParamSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                             *((ulong_t *)args[2].pBuf), *((ulong_t *)args[3].pBuf),
                             *((struct in_addr *)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapIpDeadIntervalSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpDeadIntervalSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                    args[2].pBuf, args[3].pBuf,
                                    *((struct in_addr *)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapIpNetworkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIpNetworkSet(*((BOOL *)args[0].pBuf), args[1].pBuf, *((int *)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapRedistributeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 6 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRedistributeSet(*((BOOL *)args[0].pBuf), args[1].pBuf, args[2].pBuf,
                                    args[3].pBuf, args[4].pBuf, *((ushort_t*)args[5].pBuf));

    return iMsgRet;
}

int ospfWrapDistrListOutSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 4 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDistrListOutSet(*((BOOL *)args[0].pBuf), args[1].pBuf, args[2].pBuf, *((ushort_t*)args[3].pBuf));

    return iMsgRet;
}

int ospfWrapDftInfoSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 6 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDftInfoSet(*((BOOL *)args[0].pBuf), *((BOOL *)args[1].pBuf),
                            args[2].pBuf, args[3].pBuf, args[4].pBuf, *((ushort_t*)args[5].pBuf));

    return iMsgRet;
}

int ospfWrapDefaultMetricSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDefaultMetricSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}


int ospfWrapDistanceSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDistanceSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf), *((ushort_t*)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapDistanceOspfSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDistanceOspfSet(*((BOOL *)args[0].pBuf), *((ulong_t *)args[1].pBuf),
                                    *((ulong_t *)args[2].pBuf), *((ulong_t *)args[3].pBuf), *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapDistancePrefixSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 5 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDistancePrefixSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                    args[2].pBuf, args[3].pBuf, *((ushort_t*)args[4].pBuf));

    return iMsgRet;
}

int ospfWrapMtuIgnoreSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 3 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfMtuIgnoreSet(*((BOOL *)args[0].pBuf), args[1].pBuf,
                                *((struct in_addr *)args[2].pBuf));

    return iMsgRet;
}

int ospfWrapIntfInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfIntfInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapVirIntfInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfVirIntfInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapLsaInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfLsaInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfInfoGet(args[0].pBuf);

    return iMsgRet;

}

int ospfWrapRedistInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRedistInfoGet(args[0].pBuf);

    return iMsgRet;

}

int ospfWrapAreaInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapRunInstanceGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRunInstanceGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapAreaNetworkByGetAreaId(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaNetworkByGetAreaId(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapAreaRangeByGetAreaId(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfAreaRangeByGetAreaId(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfWrapRouteInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;


    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRouteInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;


}

int ospfWrapRouteNumGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 1 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRouteNumGet((ulong_t *)args[0].pBuf);

    return iMsgRet;
}

int ospfWrapRtrRouteInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;


    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfRtrRouteInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfFrrStatusGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 1 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfStatusGet(args[0].pBuf);

    return iMsgRet;

}


int ospfFrrNbrInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfNbrInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfFrrNbmaNbrInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    //smdDebugPrintf(SMD_DEBUG_PROCESS, "%s:start", __func__);

    if ( 2 != ulArgsNum )
    {
        //smdDebugPrintf(SMD_DEBUG_ERR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfNbmaNbrInfoGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;

}

int ospfFrrPassiveIntfGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfPassiveIntfGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;
}

int ospfFrrDistanceIntfGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    int iMsgRet = -1;

    if ( 2 != ulArgsNum )
    {
        return VOS_ERR_FAIL;
    }

    iMsgRet = ospfDistanceIntfGet(*((DATA_OP_ENT *)args[0].pBuf), args[1].pBuf);

    return iMsgRet;
}

/******************************************************************************
函数名称：ospfFrrWrapFunction
功能描述：注册库函数消息流解析函数
输入参数：无
输出参数：无
函数返回：VD_E_NONE, 成功
          VD_E_FAIL，执行失败
          VD_E_PARAM，参数错误
其它说明：在进程接收消息之前调用
******************************************************************************/
int ospfFrrWrapFunction(void)
{
    int iRet = VOS_ERR_NONE;
    memset(ospfWrap, 0, (FRR_FUNC_OSPF_END * sizeof(SMD_WRAP_CMD)));
	if(ospfWrap == NULL)
	{
		printf("\r %s %d pSmdWrap == NULL\n", __func__, __LINE__);
	}
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_OPEN, 0, ospfFrrWrapEnable, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_CLOSE, 0, ospfFrrWrapDisable, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_SET, 0, ospfWrapAreaSet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_NETWORK_SET, 0, ospfWrapAreaNetworkSet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_ROUTER_ID_SET, 0, ospfFrrRouterIdSet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_PASSIVE_INTF_DEFAULT_SET, 0, ospfFrrPassiveIntfDefaultSet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_PASSIVE_INTF_SET, 0, ospfFrrPassiveIntfSet, ospfWrap, FRR_FUNC_OSPF_END);

    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_RANGE_SET, 0, ospfWrapAreaRangeSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_RANGE_SUB_SET, 0, ospfWrapAreaRangeSubstituteSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_VIRTUAL_LINK_SET, 0, ospfWrapVirLinkSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_SHORT_CUT_SET, 0, ospfWrapAreaShortCutSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_STUB_AREA_SET, 0, ospfWrapStubAreaSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_NSSA_AREA_SET, 0, ospfWrapNssaAreaSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_DEFAULT_COST_SET, 0, ospfWrapAreaDefaultCostSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_EXPORT_LIST_SET, 0, ospfWrapAreaExportListSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_IMPORT_LIST_SET, 0, ospfWrapAreaImportListSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_FILTER_LIST_SET, 0, ospfWrapAreaFilterListSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AREA_AUTH_TYPE_SET, 0, ospfWrapAreaAuthTypeSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_ABR_TYPE_SET, 0, ospfWrapAbrTypeSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_LOG_ADJACENCY_SET, 0, ospfWrapLogAdjacencySet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_RFC1583_SET, 0, ospfWrapRfc1583compatibilitySet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_SPF_TIMER_SET, 0, ospfWrapSpfTimerSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_STATIC_NEIGHBOR_SET, 0, ospfWrapStaticNeighborSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_REFRESH_TIMER_SET, 0, ospfWrapRefreshTimerSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_AUTO_COST_SET, 0, ospfWrapAutoCostSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AUTH_MODE_SET, 0, ospfWrapIpAuthModeSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AUTH_SIMPLE_SET, 0, ospfWrapIpAuthSimpleSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AUTH_MD5_SET, 0, ospfWrapIpAuthMd5Set, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_PARAM_SET, 0, ospfWrapIpParamSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_DEAD_INTERVAL_SET, 0, ospfWrapIpDeadIntervalSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_NETWORK_SET, 0, ospfWrapIpNetworkSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_REDISTRIBUTE_SET, 0, ospfWrapRedistributeSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DISTRI_LIST_OUT_SET, 0, ospfWrapDistrListOutSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DFT_INFO_SET, 0, ospfWrapDftInfoSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DEFAULT_METRIC_SET, 0, ospfWrapDefaultMetricSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DISTANCE_SET, 0, ospfWrapDistanceSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DISTANCE_OSPF_SET, 0, ospfWrapDistanceOspfSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DISTANCE_PERFIX_SET, 0, ospfWrapDistancePrefixSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_OSPF_MTU_IGNORE_SET, 0, ospfWrapMtuIgnoreSet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_INTF_INFO_GET, 0, ospfWrapIntfInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_VIR_INTF_INFO_GET, 0, ospfWrapVirIntfInfoGet, ospfWrap, FRR_FUNC_OSPF_END);

	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_NBR_INFO_GET, 0, ospfFrrNbrInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_NBMA_NBR_INFO_GET, 0, ospfFrrNbmaNbrInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_ROUTE_INFO_GET, 0, ospfWrapRouteInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_ROUTE_NUM_GET, 0, ospfWrapRouteNumGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_RTR_ROUTE_INFO_GET, 0, ospfWrapRtrRouteInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
    iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_LSA_INFO_GET, 0, ospfWrapLsaInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_INFO_GET, 0, ospfWrapInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_REDIST_INFO_GET, 0, ospfWrapRedistInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AREA_INFO_GET, 0, ospfWrapAreaInfoGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_STATUS_GET, 0, ospfFrrStatusGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_PROCEESS_GET, 0, ospfWrapRunInstanceGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AREA_NETWORK_GET, 0, ospfWrapAreaNetworkByGetAreaId, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_IP_OSPF_AREA_RANGE_GET, 0, ospfWrapAreaRangeByGetAreaId, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_PASSIVE_INTF_GET, 0, ospfFrrPassiveIntfGet, ospfWrap, FRR_FUNC_OSPF_END);
	iRet += smdAppWrapRegister(FRR_FUNC_OSPF_DISTANCE_GET, 0, ospfFrrDistanceIntfGet, ospfWrap, FRR_FUNC_OSPF_END);



    if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }

    return iRet;
}

int ospfFrrWrapProcess(SMD_MSG *pMsg)
{
	int iRet = VOS_ERR_NONE;
	iRet = smdCfgAppWrapProcess(pMsg, ospfWrap, FRR_FUNC_OSPF_END);
	if ( VOS_ERR_NONE != iRet )
    {
        //smdDebugPrintf(SMD_DEBUG_WRAP_ERR, "%s:register failed", __func__);
    }
    return iRet;
}

