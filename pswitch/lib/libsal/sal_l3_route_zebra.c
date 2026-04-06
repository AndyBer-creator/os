
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>
#include <libipc/quaggaTask.h>

int ipc_zebraInterfaceIpAdd(char *ifname, char *addr_str)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IF_IP_ADD, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, ifname, SMD_ARG_POINTER, strlen(ifname) + 1);
    ARG_ADD(1, addr_str, SMD_ARG_POINTER, strlen(addr_str) + 1);
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config router-id  failed iRet = %d\n", __func__, iRet);
    }
    
    return iRet;
}

int ipc_zebraInterfaceIpDel(char *ifname, char *addr_str)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IF_IP_DELETE, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, ifname, SMD_ARG_POINTER, strlen(ifname) + 1);
    ARG_ADD(1, addr_str, SMD_ARG_POINTER, strlen(addr_str) + 1);
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config router-id  failed iRet = %d\n", __func__, iRet);
    }
    
    return iRet;
}

int ipc_zebraRouterIdAdd(char *id_str)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_ROUTERID, SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, id_str, SMD_ARG_POINTER, strlen(id_str) + 1);
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config router-id  failed iRet = %d\n", __func__, iRet);
    }
    
    return iRet;
}

int ipc_zebraRouterIdDelete(void)
{
    int                       iRet;
    SMD_MSG                   msg;

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_ROUTERID_NO, SMD_GLOBAL, SMD_MSG_INFO);

    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, NULL, 0, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config router-id  failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int ipc_zebraRouterIdGet(char szRouteId[64])
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_ROUTERID_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, szRouteId, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), 64);
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_zebraIpRouterAdd(char *strNetwork, char *strNetmask, char *strGateway,
    	ulong_t ulDistance, ulong_t ulFlags)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IP_ROUTE_ADD, SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, strNetwork, SMD_ARG_POINTER, strlen(strNetwork) + 1);
    ARG_ADD(1, strNetmask, SMD_ARG_POINTER, strlen(strNetmask) + 1);
    ARG_ADD(2, strGateway, SMD_ARG_POINTER, strlen(strGateway) + 1);
    ARG_ADD(3, &ulDistance, 0, sizeof(ulong_t));
    ARG_ADD(4, &ulFlags, 0, sizeof(ulong_t));
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config ip route add failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int ipc_zebraIpRouterDelete(char *strNetwork, char *strNetmask, char *strGateway,
    	ulong_t ulDistance, ulong_t ulFlags)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IP_ROUTE_DELETE, SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, strNetwork, SMD_ARG_POINTER, strlen(strNetwork) + 1);
    ARG_ADD(1, strNetmask, SMD_ARG_POINTER, strlen(strNetmask) + 1);
    ARG_ADD(2, strGateway, SMD_ARG_POINTER, strlen(strGateway) + 1);
    ARG_ADD(3, &ulDistance, 0, sizeof(ulong_t));
    ARG_ADD(4, &ulFlags, 0, sizeof(ulong_t));
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config ip route delete failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int ipc_zebraIpRouteGet(DATA_OP_ENT op, ZEBRA_IP_ROUTE_STRC *pIpRoute)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    if ( (DATA_OP_GET != op) && (DATA_OP_GETNEXT != op) )
    {
        return VOS_ERR_FAIL;
    }
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IP_ROUTE_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
    ARG_ADD(1, pIpRoute, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(ZEBRA_IP_ROUTE_STRC));
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_zebraIpv6RouterAdd(char *strNetwork, char *strGateway, char* intfName)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);
  
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IPV6_ROUTE_ADD, SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, strNetwork, SMD_ARG_POINTER, strlen(strNetwork) + 1);
    ARG_ADD(1, strGateway, SMD_ARG_POINTER, strlen(strGateway) + 1);
    if ( NULL == intfName )
    {
        ARG_ADD(2, NULL, SMD_ARG_POINTER, 0);
    }
    else
    {
        ARG_ADD(2, intfName, SMD_ARG_POINTER, strlen(intfName) + 1);
    }
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config ipv6 route add failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int ipc_zebraIpv6RouterDelete(char *strNetwork, char *strGateway, char* intfName)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IPV6_ROUTE_DEL, SMD_GLOBAL, SMD_MSG_INFO);

    ARG_ADD(0, strNetwork, SMD_ARG_POINTER, strlen(strNetwork) + 1);
    ARG_ADD(1, strGateway, SMD_ARG_POINTER, strlen(strGateway) + 1);
    if ( NULL == intfName )
    {
        ARG_ADD(2, NULL, SMD_ARG_POINTER, 0);
    }
    else
    {
        ARG_ADD(2, intfName, SMD_ARG_POINTER, strlen(intfName) + 1);
    }

    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:config ipv6 route delete failed iRet = %d\n", __func__, iRet);
    }

    return iRet;
}

int ipc_zebraIpv6RouteGet(DATA_OP_ENT op, ZEBRA_IPV6_ROUTE_STRC *pIpv6Route)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);
    
    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_ZEBRA_IPV6_ROUTE_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
    ARG_ADD(1, pIpv6Route, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(ZEBRA_IPV6_ROUTE_STRC));
    
    iRet = smdMsgRequestByName(ZEBRA_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 sal_l3_route_id_add(char *id_str)
{

    int                       iRet = 0;

	iRet = ipc_zebraRouterIdAdd(id_str);
    return iRet;
}

int32 sal_l3_route_id_del(void)
{
    int                       iRet = 0;

	iRet = ipc_zebraRouterIdDelete();
    return iRet;
}

int32 sal_l3_route_id_get(char *id_str)
{
    int                       iRet = 0;
	SYS_PARAM_CHK((NULL == id_str), SYS_ERR_NULL_POINTER);
	
	iRet = ipc_zebraRouterIdGet(id_str);
    return iRet;
}

int32 sal_l3_ip_route_add(sys_ipv4_t dIp, sys_ipv4_t Netmask, sys_ipv4_t Gateway, uint32 ulDistance, uint32 ulFlags)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV4_STR_LEN];
    char strGateway[CAPA_IPV4_STR_LEN];
    char strNetmask[CAPA_IPV4_STR_LEN];

    SYS_MEM_CLEAR(strNetwork);
    SYS_MEM_CLEAR(strGateway);
	SYS_MEM_CLEAR(strNetmask);

    sys_util_ip2Str(dIp, strNetwork);
    sys_util_ip2Str(Gateway, strGateway);
    sys_util_ip2Str(Netmask, strNetmask);

	iRet = ipc_zebraIpRouterAdd(strNetwork, strNetmask, strGateway, ulDistance, ulFlags);
    return iRet;
}

int32 sal_l3_ip_route_delete(sys_ipv4_t dIp, sys_ipv4_t Netmask, sys_ipv4_t Gateway, uint32 ulDistance, uint32 ulFlags)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV4_STR_LEN];
    char strGateway[CAPA_IPV4_STR_LEN];
    char strNetmask[CAPA_IPV4_STR_LEN];

    SYS_MEM_CLEAR(strNetwork);
    SYS_MEM_CLEAR(strGateway);
	SYS_MEM_CLEAR(strNetmask);

    sys_util_ip2Str(dIp, strNetwork);
    sys_util_ip2Str(Gateway, strGateway);
    sys_util_ip2Str(Netmask, strNetmask);

	iRet = ipc_zebraIpRouterDelete(strNetwork, strNetmask, strGateway, ulDistance, ulFlags);
    return iRet;
}

int32 sal_l3_ip_route_getnext(sys_l3_routeV4Info_t *pRouteEntry)
{
    int                       iRet = 0;
    ZEBRA_IP_ROUTE_STRC      zebraRoute;
    
	SYS_PARAM_CHK((NULL == pRouteEntry), SYS_ERR_NULL_POINTER);
    SYS_MEM_CLEAR(zebraRoute);
	
	zebraRoute.ulDestIp = pRouteEntry->route.dst;
	zebraRoute.ulpen = pRouteEntry->route.dstLen;

	iRet = ipc_zebraIpRouteGet(DATA_OP_GETNEXT, &zebraRoute);
	if(iRet == SYS_ERR_OK)
	{
		pRouteEntry->select = zebraRoute.select;
		pRouteEntry->fib = zebraRoute.fib;
		pRouteEntry->active = zebraRoute.active;
		pRouteEntry->metric = zebraRoute.ulMetric;
		pRouteEntry->distance = zebraRoute.ulDistance;
		pRouteEntry->type = zebraRoute.type;
		pRouteEntry->route.dst = zebraRoute.ulDestIp;
		pRouteEntry->route.dstLen = zebraRoute.ulpen;
		pRouteEntry->route.gw = zebraRoute.ulNexthop[0];
		STR2INTFID(zebraRoute.devStr, &pRouteEntry->route.ointf);
	}
    return iRet;
}

int32 sal_l3_ip_route_get(sys_l3_routeV4Info_t *pRouteEntry)
{
    int                       iRet = 0;
    ZEBRA_IP_ROUTE_STRC      zebraRoute;
    
	SYS_PARAM_CHK((NULL == pRouteEntry), SYS_ERR_NULL_POINTER);
    SYS_MEM_CLEAR(zebraRoute);
	
	zebraRoute.ulDestIp = pRouteEntry->route.dst;
	zebraRoute.ulpen = pRouteEntry->route.dstLen;

	iRet = ipc_zebraIpRouteGet(DATA_OP_GET, &zebraRoute);
	if(iRet == SYS_ERR_OK)
	{
		pRouteEntry->select = zebraRoute.select;
		pRouteEntry->fib = zebraRoute.fib;
		pRouteEntry->active = zebraRoute.active;
		pRouteEntry->metric = zebraRoute.ulMetric;
		pRouteEntry->distance = zebraRoute.ulDistance;
		pRouteEntry->type = zebraRoute.type;
		pRouteEntry->route.dst = zebraRoute.ulDestIp;
		pRouteEntry->route.dstLen = zebraRoute.ulpen;
		pRouteEntry->route.gw = zebraRoute.ulNexthop[0];
		STR2INTFID(zebraRoute.devStr, &pRouteEntry->route.ointf);
	}
    return iRet;
}

int32 sal_l3_ipv6_route_add(sys_ipv6_t dIp, uint32 len, sys_ipv6_t Gateway, sys_l3_intfId_t intf)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV6_STR_LEN+3];
    char strGateway[CAPA_IPV6_STR_LEN];
    char str[CAPA_IPV6_STR_LEN+3];

    SYS_MEM_CLEAR(strNetwork);
    SYS_MEM_CLEAR(strGateway);
    SYS_MEM_CLEAR(str);

    IPV62STR(dIp, str);
    IPV62STR(Gateway, strGateway);

    osal_sprintf(strNetwork, "%s/%d", str, len);

    if (SYS_L3_INTFTYPE_NULL == intf.type)
    {
        iRet = ipc_zebraIpv6RouterAdd(strNetwork, strGateway, "lo");
    }
    else if (SYS_L3_INTFTYPE_VLAN == intf.type)
    {
        char intfStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(intfStr);

        INTFID2STR(&intf, intfStr);

        iRet = ipc_zebraIpv6RouterAdd(strNetwork, strGateway, intfStr);
    }
    else
    {
        iRet = ipc_zebraIpv6RouterAdd(strNetwork, strGateway, NULL);
    }

    return iRet;
}

int32 sal_l3_ipv6_route_delete(sys_ipv6_t dIp, uint32 len, sys_ipv6_t Gateway, sys_l3_intfId_t intf)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV6_STR_LEN+3];
    char strGateway[CAPA_IPV6_STR_LEN];
    char str[CAPA_IPV6_STR_LEN+3];

    SYS_MEM_CLEAR(strNetwork);
    SYS_MEM_CLEAR(strGateway);
    SYS_MEM_CLEAR(str);

    IPV62STR(dIp, str);
    IPV62STR(Gateway, strGateway);

    osal_sprintf(strNetwork, "%s/%d", str, len);
    
    if (SYS_L3_INTFTYPE_NULL == intf.type)
    {

        iRet = ipc_zebraIpv6RouterDelete(strNetwork, strGateway, "lo");
    }
    else if (SYS_L3_INTFTYPE_VLAN == intf.type)
    {
        char intfStr[CAPA_L3_INTFNAME_LEN];

        SYS_MEM_CLEAR(intfStr);

        INTFID2STR(&intf, intfStr);

        iRet = ipc_zebraIpv6RouterDelete(strNetwork, strGateway, intfStr);
    }
    else
    {
        iRet = ipc_zebraIpv6RouterDelete(strNetwork, strGateway, NULL);
    }
    
    return iRet;
}

int32 sal_l3_ipv6_route_getnext(sys_l3_routeV6Info_t *pRouteEntry)
{
    int                       iRet = 0;
    ZEBRA_IPV6_ROUTE_STRC     zebraRoute;
    
	SYS_PARAM_CHK((NULL == pRouteEntry), SYS_ERR_NULL_POINTER);
    SYS_MEM_CLEAR(zebraRoute);
	
	osal_memcpy(&zebraRoute.ucDestIp, &pRouteEntry->route.dst, sizeof(sys_ipv6_t));
	zebraRoute.ulPrefixLen = pRouteEntry->route.dstLen;

	iRet = ipc_zebraIpv6RouteGet(DATA_OP_GETNEXT, &zebraRoute);
	if(iRet == SYS_ERR_OK)
	{
		pRouteEntry->select = zebraRoute.select;
		pRouteEntry->fib = zebraRoute.fib;
		pRouteEntry->active = zebraRoute.active;
		pRouteEntry->metric = zebraRoute.ulMetric;
		pRouteEntry->distance = zebraRoute.ulDistance;
		pRouteEntry->type = zebraRoute.type;
		osal_memcpy(&pRouteEntry->route.dst, &zebraRoute.ucDestIp, sizeof(sys_ipv6_t));
		pRouteEntry->route.dstLen = zebraRoute.ulPrefixLen;
		osal_memcpy(&pRouteEntry->route.gw, &zebraRoute.ucNexthop[0], sizeof(sys_ipv6_t));
		STR2INTFID(zebraRoute.devStr, &pRouteEntry->route.ointf);
	}
    return iRet;
}

int32 sal_l3_interface_ip_add(char *ifname, sys_ipv4_t dIp, sys_ipv4_t Netmask)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV4_STR_LEN];
    char strNetmask[CAPA_IPV4_STR_LEN];
    int plen = 0;

    SYS_MEM_CLEAR(strNetwork);
	SYS_MEM_CLEAR(strNetmask);

    sys_util_ip2Str(dIp, strNetmask);
    plen = IPV4_MASK2PFX(Netmask);
    osal_sprintf(strNetwork, "%s/%d", strNetmask, plen);

	iRet = ipc_zebraInterfaceIpAdd(ifname, strNetwork);
	
    return iRet;
}

int32 sal_l3_interface_ip_del(char *ifname, sys_ipv4_t dIp, sys_ipv4_t Netmask)
{
    int                       iRet = 0;
    char strNetwork[CAPA_IPV4_STR_LEN];
    char strNetmask[CAPA_IPV4_STR_LEN];
    int plen = 0;

    SYS_MEM_CLEAR(strNetwork);
	SYS_MEM_CLEAR(strNetmask);

    sys_util_ip2Str(dIp, strNetmask);
    plen = IPV4_MASK2PFX(Netmask);
    osal_sprintf(strNetwork, "%s/%d", strNetmask, plen);

	iRet = ipc_zebraInterfaceIpDel(ifname, strNetwork);
	
    return iRet;
}

