
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>
#include <libipc/quaggaTask.h>

int32 ipc_l3_rip_router(sys_enable_t enable)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(1);

	if(enable)
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_OPEN, SMD_GLOBAL, SMD_MSG_SET);
	}
	else 
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_CLOSE, SMD_GLOBAL, SMD_MSG_SET);
	}

	ARG_ADD(0, &enable, 0, sizeof(enable));

	iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{
		printf("%s:request failed iRet = %d\n", __func__, iRet);
	}

	return iRet;
}

int32 sal_l3_routeV4_rip_open(void)
{
    SYS_ERR_CHK(rsd_rip_enable_set(ENABLED));

	SYS_ERR_CHK(ipc_l3_rip_router(ENABLED));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_rip_cloce(void)
{
    SYS_ERR_CHK(rsd_rip_enable_set(DISABLED));

	SYS_ERR_CHK(ipc_l3_rip_router(DISABLED));

    return SYS_ERR_OK;
}

int32 sal_l3_rip_network_set(sys_enable_t bNoFlag, uint32 ulIp, uint32 ulMask, char *ifname)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_NETWORK_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &ulIp, 0, sizeof(ulIp));
    ARG_ADD(2, &ulMask, 0, sizeof(ulMask));
    ARG_ADD(3, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0 );

    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int32 ipc_rip_version_rip_set(int32 tmpVersion, int32 isSet)
{
    int                       iRet;
    SMD_MSG                   msg;
	
	ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_VERSION_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &tmpVersion, 0, sizeof(tmpVersion));
	ARG_ADD(1, &isSet, 0, sizeof(isSet));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

int32 ipc_rip_route_config(int32 isSet, char *ipAddrStr)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == ipAddrStr), SYS_ERR_NULL_POINTER);
	ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_STATIC_ROUTE_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &isSet, 0, sizeof(isSet));
	ARG_ADD(1, ipAddrStr, SMD_ARG_POINTER, strlen(ipAddrStr)+1);
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

int32 ipc_rip_distance_config(int32 isSet, int32 distanceVal)
{
    int                       iRet;
    SMD_MSG                   msg;
	
	ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_DISTANCE_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &isSet, 0, sizeof(isSet));
	ARG_ADD(1, &distanceVal, 0, sizeof(distanceVal));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

int32 ipc_rip_distance_source_config(int32 isSet, char *distanceVal, char *sourceAddr)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == distanceVal), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == sourceAddr), SYS_ERR_NULL_POINTER);
	ARG_DEF(3);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_DISTANCE_SOURCE_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &isSet, 0, sizeof(isSet));
	ARG_ADD(1, distanceVal, SMD_ARG_POINTER, strlen(distanceVal)+1);
	ARG_ADD(2, sourceAddr, SMD_ARG_POINTER, strlen(sourceAddr)+1);
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

int32 ipc_rip_distribute_list_config(int32 isSet, char *alistName, char *type, char *ifName)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == alistName), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == type), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == ifName), SYS_ERR_NULL_POINTER);
	ARG_DEF(4);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_DISTRIBUTE_LIST_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &isSet, 0, sizeof(isSet));
	ARG_ADD(1, alistName, SMD_ARG_POINTER, strlen(alistName)+1);
	ARG_ADD(2, type, SMD_ARG_POINTER, strlen(type)+1);
	ARG_ADD(3, ifName, SMD_ARG_POINTER, strlen(ifName)+1);
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

int32 ipc_rip_access_list_config(int32 isSet, int32 afi,char *alistName, char *typestr, char *prefix_str, int32 exact_match)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == alistName), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == typestr), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == prefix_str), SYS_ERR_NULL_POINTER);
	
	ARG_DEF(6);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ACCESS_LIST_ZEBRA_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &isSet, 0, sizeof(isSet));
	ARG_ADD(1, &afi, 0, sizeof(afi));
	ARG_ADD(2, alistName, SMD_ARG_POINTER, strlen(alistName)+1);
	ARG_ADD(3, typestr, SMD_ARG_POINTER, strlen(typestr)+1);
	ARG_ADD(4, prefix_str, SMD_ARG_POINTER, strlen(prefix_str)+1);
	ARG_ADD(5, &exact_match, 0, sizeof(exact_match));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 6, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;
}

 int32 ipc_access_list_get_alist_info( DATA_OP_ENT op, int afi, ALIST_INFO_T *pAlistInfo)
 {

	 int					   iRet;
	 SMD_MSG				   msg;
	 
	 SYS_PARAM_CHK((NULL == pAlistInfo), SYS_ERR_NULL_POINTER);

	 ARG_DEF(3);
	 smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ACCESS_LIST_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);
	 ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
	 ARG_ADD(1, &afi, 0, sizeof(int));
	 ARG_ADD(2, pAlistInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(ALIST_INFO_T));
	 iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	 
	 return iRet;

 }
 
 int32 ipc_access_list_get_filter_info(DATA_OP_ENT op, int afi, FILTER_INFO_T *pFilterInfo)
 {
	 int					   iRet;
	 SMD_MSG				   msg;
	 
	 SYS_PARAM_CHK((NULL == pFilterInfo), SYS_ERR_NULL_POINTER);
	 
	 ARG_DEF(3);
	 smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ACCESS_LIST_FILTER_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);
	 ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
	 ARG_ADD(1, &afi, 0, sizeof(int));
	 ARG_ADD(2, pFilterInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(FILTER_INFO_T));
	 iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	 
	 return iRet;
 
 }

 int32 ipc_distribute_list_get_info(DATA_OP_ENT op, DISTRIBUTE_INFO_T *pDistributeInfo)
 {
	 int					   iRet;
	 SMD_MSG				   msg;
	 
	 SYS_PARAM_CHK((NULL == pDistributeInfo), SYS_ERR_NULL_POINTER);
	 
	 ARG_DEF(2);
	 smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_DISTRIBUTE_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);
	 ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
	 ARG_ADD(1, pDistributeInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(DISTRIBUTE_INFO_T));
	 iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	 
	 return iRet;
 
 }

int32 ipc_rip_debug(int bIsEnbale, char *pRipDebugStr)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == pRipDebugStr), SYS_ERR_NULL_POINTER);
	ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_DEBUG_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &bIsEnbale, 0, sizeof(bIsEnbale));
	ARG_ADD(1, pRipDebugStr, SMD_ARG_POINTER, strlen(pRipDebugStr)+1);
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;

}

int32 ipc_rip_log_to_fileName(int bIsEnbale)
{
	int                       iRet;
    SMD_MSG                   msg;

	ARG_DEF(1);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_LOG_TO_FILE_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &bIsEnbale, 0, sizeof(bIsEnbale));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    
    return iRet;
}

int32 ipc_rip_log_to_stdout(int bIsEnbale)
{
	int                       iRet;
    SMD_MSG                   msg;

	ARG_DEF(1);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_LOG_TO_STDOUT_SET, SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &bIsEnbale, 0, sizeof(bIsEnbale));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int32 ipc_rip_status_get(int32 *bEnable)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_STATUS_GET, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, bEnable, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int32));   
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    return iRet;

}

int32 ipc_rip_version_get(int32 *sendVersion, int32 *recvVersion)
{
	int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_VERSION_GET, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, sendVersion, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int32));
    ARG_ADD(1, recvVersion, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int32));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }
    
    return iRet;
}

int32 ipc_rip_enable_network_info_get(DATA_OP_ENT op, RIP_ENABLE_NW_INFO_T *pRipEnableNwInfo)
{
	int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_ENABLE_NW_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
    ARG_ADD(1, pRipEnableNwInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(RIP_ENABLE_NW_INFO_T));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    
    return iRet;
}

int32 ipc_rip_ip_route_get(DATA_OP_ENT op, RIP_IP_ROUTE_STRC *pRipRoute)
{
	int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_ROUTE_GET, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
    ARG_ADD(1, pRipRoute, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(RIP_IP_ROUTE_STRC));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    
    return iRet;
}

int32 ipc_rip_route_num_get(int32 *routeNum)
{
	int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_ROUTE_NUM_GET, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, &routeNum, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int32));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    
    return iRet;
}

int32 ipc_rip_information_info_get(RIP_INFO_T *ripInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

	SYS_PARAM_CHK((NULL == ripInfo), SYS_ERR_NULL_POINTER);
    ARG_DEF(1);
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_RIP_STATUS_SHOW, SMD_GLOBAL, SMD_MSG_GET);
    ARG_ADD(0, ripInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(RIP_INFO_T));
    iRet = smdMsgRequestByName(RIP_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("  rip is closed\n");
    }

    return iRet;
}

int32 sal_rip_version_rip_set(int32 tmpVersion, int32 isSet)
{
    int                       iRet;
	iRet = ipc_rip_version_rip_set(tmpVersion, isSet);
    return iRet;
}

int32 sal_rip_route_config(int32 isSet, char *ipAddrStr)
{
    int                       iRet;

	SYS_PARAM_CHK((NULL == ipAddrStr), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_route_config(isSet, ipAddrStr);

    return iRet;
}

int32 sal_rip_distance_config(int32 isSet, int32 distanceVal)
{
    int                       iRet;
	
	iRet = ipc_rip_distance_config(isSet, distanceVal);

    return iRet;
}

int32 sal_rip_distance_source_config(int32 isSet, char *distanceVal, char *sourceAddr)
{
    int                       iRet;

	SYS_PARAM_CHK((NULL == distanceVal), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == sourceAddr), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_distance_source_config(isSet, distanceVal, sourceAddr);
    return iRet;
}

int32 sal_rip_distribute_list_config(int32 isSet, char *alistName, char *type, char *ifName)
{

    int                       iRet;

	SYS_PARAM_CHK((NULL == alistName), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == type), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == ifName), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_distribute_list_config(isSet, alistName, type, ifName);

    return iRet;
}

int32 sal_rip_access_list_config(int32 isSet, int32 afi,char *alistName, char *typestr, char *prefix_str, int32 exact_match)
{

    int                       iRet;

	SYS_PARAM_CHK((NULL == alistName), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == typestr), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == prefix_str), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_access_list_config(isSet, afi,alistName, typestr, prefix_str, exact_match);
	
    return iRet;
}

 int32 sal_access_list_get_alist_info( DATA_OP_ENT op, int afi, ALIST_INFO_T *pAlistInfo)
 {

	 int32					   iRet;
	 
	 SYS_PARAM_CHK((NULL == pAlistInfo), SYS_ERR_NULL_POINTER);

	 iRet = ipc_access_list_get_alist_info(op, afi, pAlistInfo);
	 
	 return iRet;

 }

 int32 sal_access_list_get_filter_info(DATA_OP_ENT op, int afi, FILTER_INFO_T *pFilterInfo)
 {
	 int32					   iRet;
	 
	 SYS_PARAM_CHK((NULL == pFilterInfo), SYS_ERR_NULL_POINTER);
	 iRet = ipc_access_list_get_filter_info(op, afi, pFilterInfo);
	 
	 return iRet;
 
 }

 int32 sal_distribute_list_get_info(DATA_OP_ENT op, DISTRIBUTE_INFO_T *pDistributeInfo)
 {
	 int32					   iRet;
	 
	 SYS_PARAM_CHK((NULL == pDistributeInfo), SYS_ERR_NULL_POINTER);
	 iRet = ipc_distribute_list_get_info(op, pDistributeInfo);
	 
	 return iRet;
 }

int32 sal_rip_debug(int bIsEnbale, char *pRipDebugStr)
{

    int                       iRet;
    
	SYS_PARAM_CHK((NULL == pRipDebugStr), SYS_ERR_NULL_POINTER);
	
	iRet = ipc_rip_debug(bIsEnbale, pRipDebugStr);
	
    return iRet;
}

int32 sal_rip_log_to_fileName(int bIsEnbale)
{

    int                       iRet;
	
	iRet = ipc_rip_log_to_fileName(bIsEnbale);
	
    return iRet;
}

int32 sal_rip_log_to_stdout(int bIsEnbale)
{

    int                       iRet;
	
	iRet = ipc_rip_log_to_stdout(bIsEnbale);
	
    return iRet;
}

int32 sal_rip_status_get(int32 *bEnable)
{

    int                       iRet;
    
	SYS_PARAM_CHK((NULL == bEnable), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_status_get(bEnable);
	
    return iRet;
}

int32 sal_rip_version_get(int32 *sendVersion, int32 *recvVersion)
{

    int                       iRet;
    
	SYS_PARAM_CHK((NULL == sendVersion), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == recvVersion), SYS_ERR_NULL_POINTER);
	iRet = ipc_rip_version_get(sendVersion, recvVersion);
	
    return iRet;
}

int32 sal_rip_enable_network_info_get(DATA_OP_ENT op, RIP_ENABLE_NW_INFO_T *pRipEnableNwInfo)
{

    int                       iRet;
    
	SYS_PARAM_CHK((NULL == pRipEnableNwInfo), SYS_ERR_NULL_POINTER);
	if(DATA_OP_GETNEXT != op)
	{
		return VOS_ERR_PARAM;
	}
	iRet = ipc_rip_enable_network_info_get(op, pRipEnableNwInfo);
	
    return iRet;
}

int32 sal_rip_ip_route_get(DATA_OP_ENT op, RIP_IP_ROUTE_STRC *pRipRoute)
{
	int                       iRet;
    
	SYS_PARAM_CHK((NULL == pRipRoute), SYS_ERR_NULL_POINTER);
	if(DATA_OP_GETNEXT != op)
	{
		return VOS_ERR_PARAM;
	}
	iRet = ipc_rip_ip_route_get(op, pRipRoute);
	
    return iRet;
}

int32 sal_rip_route_num_get(int32 *routeNum)
{
	int                       iRet;
    
	SYS_PARAM_CHK((NULL == routeNum), SYS_ERR_NULL_POINTER);

	iRet = ipc_rip_route_num_get(routeNum);
	
    return iRet;
}

int32 sal_rip_information_info_get(RIP_INFO_T *ripInfo)
{

    int                       iRet;
    RIP_INFO_T	tmpRipInfo;
    
	SYS_PARAM_CHK((NULL == ripInfo), SYS_ERR_NULL_POINTER);
	SYS_MEM_CLEAR(tmpRipInfo);
	iRet = ipc_rip_information_info_get(&tmpRipInfo);
	if(SYS_ERR_OK == iRet)
	{
		ripInfo->update_time = tmpRipInfo.update_time;
	    ripInfo->remain_time = tmpRipInfo.remain_time;
	    ripInfo->timeout_time = tmpRipInfo.timeout_time;
	    ripInfo->garbage_time = tmpRipInfo.garbage_time;
	    ripInfo->default_metric= tmpRipInfo.default_metric;
	    ripInfo->distance = tmpRipInfo.distance;
	    osal_memcpy(ripInfo->version_send, tmpRipInfo.version_send, sizeof(tmpRipInfo.version_send));
	    osal_memcpy(ripInfo->version_recv, tmpRipInfo.version_recv, sizeof(tmpRipInfo.version_recv));
	}
	
    return iRet;
}

