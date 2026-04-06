
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>
#include <libipc/quaggaTask.h>

int32 ipc_l3_bgp_router(sys_enable_t enable, uint32 as_number)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(2);

	if(enable)
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_BGP_OPEN, SMD_GLOBAL, SMD_MSG_SET);
	}
	else 
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_BGP_CLOSE, SMD_GLOBAL, SMD_MSG_SET);
	}

	ARG_ADD(0, &enable, 0, sizeof(enable));
	ARG_ADD(1, &as_number, 0, sizeof(as_number));

	iRet = smdMsgRequestByName(BGP_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{
		printf("%s:request failed iRet = %d\n", __func__, iRet);
	}
	else
	{
		printf("\r config bgp ----ok\n");
	}

	return iRet;
}

int32 ipc_l3_bgp_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp, sys_ipv4_t ulMask, uint32 areaId)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_BGP_NETWORK_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &ulIp, 0, sizeof(ulIp));
    ARG_ADD(2, &ulMask, 0, sizeof(ulMask));
    ARG_ADD(3, &areaId, 0, sizeof(areaId));

    iRet = smdMsgRequestByName(BGP_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int32 sal_l3_routeV4_bgp_open(uint32 as_number)
{

	SYS_ERR_CHK(ipc_l3_bgp_router(ENABLED, as_number));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_bgp_cloce(uint32 as_number)
{

	SYS_ERR_CHK(ipc_l3_bgp_router(DISABLED, as_number));

    return SYS_ERR_OK;
}

int32 sal_l3_bgp_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp, sys_ipv4_t ulMask, uint32 areaId)
{

	SYS_ERR_CHK(ipc_l3_bgp_network_area_set(bNoFlag, ulIp, ulMask, areaId));

    return SYS_ERR_OK;
}
#if 0

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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
    else 
    {
		printf("\r %sconfig rip ----ok\n", __func__);
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
#endif
