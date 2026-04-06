/*******************************************************************************
 Copyright (C),2002-2014, NewGreenNet Tech.  Co . ,  Ltd.
 文件名称: ospfFrrFuncParse.h
 描述: 提供其它进程调用的库函数使用dhcp relay/server模块的参数传递机制进行，此文件完成
       解析从库函数传递过来的消息流并调用相应的处理函数。
 修订记录：
 修订1：
*******************************************************************************/
/***********************************头文件*************************************/
#ifndef _INCLUDE_OSPFFUNCPARSE_H_
#define _INCLUDE_OSPFFUNCPARSE_H_
#include <libipc/smdMsgRequest.h>

/***********************************数据***************************************/

/***********************************函数***************************************/

int ospfFrrWrapFunction(void);
int ospfFrrWrapProcess(SMD_MSG *pMsg);
int ospfFrrWrapEnable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrWrapDisable(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaNetworkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrRouterIdSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrPassiveIntfDefaultSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrPassiveIntfSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaRangeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaRangeSubstituteSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapVirLinkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaShortCutSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapStubAreaSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapNssaAreaSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaDefaultCostSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaExportListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaImportListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaFilterListSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaAuthTypeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAbrTypeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapLogAdjacencySet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRfc1583compatibilitySet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapSpfTimerSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapStaticNeighborSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRefreshTimerSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAutoCostSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpAuthModeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpAuthSimpleSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpAuthMd5Set(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpParamSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpDeadIntervalSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIpNetworkSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRedistributeSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDistrListOutSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDftInfoSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDefaultMetricSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDistanceSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDistanceOspfSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapDistancePrefixSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapMtuIgnoreSet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapIntfInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapVirIntfInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapLsaInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRedistInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRunInstanceGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaNetworkByGetAreaId(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapAreaRangeByGetAreaId(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRouteInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRouteNumGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfWrapRtrRouteInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrStatusGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrNbrInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrNbmaNbrInfoGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrPassiveIntfGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);
int ospfFrrDistanceIntfGet(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum);

#endif /*_INCLUDE_OSPFFUNCPARSE_H_*/

