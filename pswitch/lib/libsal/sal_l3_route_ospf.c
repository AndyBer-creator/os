
#include <common/sys_log.h>
#include <libsal/sal_util.h>
#include <libsal/sal_l3.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>
#include <libipc/quaggaTask.h>

int32 ipc_l3_ospf_router(sys_enable_t enable, uint16 ProcessID)
{

	int 					  iRet;
	SMD_MSG 				  msg;
	ARG_DEF(1);

	if(enable)
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_OPEN, SMD_GLOBAL, SMD_MSG_SET);
	}
	else
	{
		smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_CLOSE, SMD_GLOBAL, SMD_MSG_SET);
	}

	ARG_ADD(0, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{
		printf("%s:request failed iRet = %d\n", __func__, iRet);
	}

	return iRet;
}

int ipc_l3_ospf_status_get(sys_enable_t *pBEnable)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_STATUS_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, pBEnable, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {
        printf("%s:request failed iRet = %d\n", __func__, iRet);
    }

    return iRet;

}

int ipc_l3_ospf_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbrInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_NBR_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pNbrInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_NBR_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_l3_ospf_nbma_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbmaNbrInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR,FRR_FUNC_IP_OSPF_NBMA_NBR_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pNbmaNbrInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_NBR_INFO_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_l3_ospf_virtual_link_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pVirIntfInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_VIR_INTF_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pVirIntfInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_NBR_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_l3_ospf_intf_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pIntfInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_INTF_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pIntfInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_INTF_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_l3_ospf_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AREA_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pAreaInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_AREA_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_l3_ospf_lsa_info_get(DATA_OP_ENT op, OSPF_LSA_INFO_T *pLsaInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_LSA_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pLsaInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_LSA_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int32 ipc_l3_ospf_area_set(sys_enable_t bNoFlag, uint32 areaId, uint16 ProcessID)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_l3_ospf_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp,
	sys_ipv4_t ulMask, uint32 areaId, uint16 ProcessID)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_NETWORK_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &ulIp, 0, sizeof(ulIp));
    ARG_ADD(2, &ulMask, 0, sizeof(ulMask));
    ARG_ADD(3, &areaId, 0, sizeof(areaId));
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_l3_ospf_router_id_set(sys_enable_t bNoFlag, sys_ipv4_t router_id, uint16 ProcessID)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_ROUTER_ID_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &router_id, 0, sizeof(router_id));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_l3_ospf_passive_interface_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_PASSIVE_INTF_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
    ARG_ADD(2, &addr, 0, sizeof(addr));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_l3_ospf_passive_interface_default(sys_enable_t bNoFlag)
{

    int                       iRet;
    SMD_MSG                   msg;
	ARG_DEF(1);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_PASSIVE_INTF_DEFAULT_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_ospf_info_get(OSPF_INFO_T *pOspfInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(1);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, pOspfInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_INFO_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_ospf_redist_info_get(OSPF_REDIST_INFO_T *pRedistInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_REDIST_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

	ARG_ADD(0, pRedistInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_REDIST_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{
		printf("%s:request failed ospf info iRet = %d\n", __func__, iRet);
	}

	return iRet;

}

int ipc_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AREA_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pAreaInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_AREA_INFO_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_ospf_passive_interface_get(DATA_OP_ENT op, OSPF_PASSIVE_INTF_INFO_T *pIntfInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_PASSIVE_INTF_GET, SMD_GLOBAL, SMD_MSG_GET);

	ARG_ADD(0, &op, 0, sizeof(op));
	ARG_ADD(1, pIntfInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_PASSIVE_INTF_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_process_status_get(DATA_OP_ENT op, uint16 *pProcess)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_PROCEESS_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pProcess, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(pProcess));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_area_network_get(DATA_OP_ENT op, OSPF_AREA_NETWORK_T *network)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AREA_NETWORK_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, network, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_AREA_NETWORK_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_area_range_get(DATA_OP_ENT op, OSPF_AREA_RANGE_T *range)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AREA_RANGE_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, range, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_AREA_RANGE_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_ospf_route_info_get(DATA_OP_ENT op, OSPF_ROUTE_INFO_T *pRouteInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_ROUTE_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pRouteInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_ROUTE_INFO_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_ospf_route_border_info_get(DATA_OP_ENT op, OSPF_RTR_ROUTE_INFO_T *pRouteInfo)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_RTR_ROUTE_INFO_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(op));
    ARG_ADD(1, pRouteInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_RTR_ROUTE_INFO_T));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_ospf_distance_get(DATA_OP_ENT op, OSPF_DISTANCE_INFO_T *pDistInfo)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DISTANCE_GET, SMD_GLOBAL, SMD_MSG_GET);

	ARG_ADD(0, &op, 0, sizeof(op));
	ARG_ADD(1, pDistInfo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(OSPF_DISTANCE_INFO_T));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);

	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int32 ipc_l3_ospf_area_range_set(sys_enable_t bNoFlag, uint32 areaId,
	char *prefix_str, uint32 advertise, uint32 cost, uint16 ProcessID)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(6);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_RANGE_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, &areaId, 0, sizeof(areaId));
	ARG_ADD(2, prefix_str, SMD_ARG_POINTER, (NULL != prefix_str) ? (strlen(prefix_str)+1) : 0);
	ARG_ADD(3, &advertise, 0, sizeof(advertise));
	ARG_ADD(4, &cost, 0, sizeof(cost));
	ARG_ADD(5, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 6, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_l3_ospf_area_range_substitute_set(sys_enable_t bNoFlag, uint32 areaId, char *prefix_src,
                                        char *prefix_dst, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_RANGE_SUB_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, prefix_src, SMD_ARG_POINTER, (NULL != prefix_src) ? (strlen(prefix_src)+1) : 0);
    ARG_ADD(3, prefix_dst, SMD_ARG_POINTER, (NULL != prefix_dst) ? (strlen(prefix_dst)+1) : 0);
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_l3_ospf_area_vlink_set(sys_enable_t bNoFlag, struct ospf_vllink_config_data *vl_config, uint16 ProcessID)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_VIRTUAL_LINK_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, vl_config, SMD_ARG_POINTER, sizeof(struct ospf_vllink_config_data));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

	return iRet;

}

int ipc_l3_ospf_area_stub_set(sys_enable_t bNoFlag, uint32 areaId, int AreaSummaryStatus, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_STUB_AREA_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, &AreaSummaryStatus, 0, sizeof(AreaSummaryStatus));
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_l3_ospf_area_nssa_set(sys_enable_t bNoFlag, uint32 areaId, char *translator_role,
                          int AreaSummaryStatus, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_NSSA_AREA_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, translator_role, SMD_ARG_POINTER, (NULL != translator_role) ? (strlen(translator_role)+1) : 0);
    ARG_ADD(3, &AreaSummaryStatus, 0, sizeof(AreaSummaryStatus));
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;

}

int ipc_l3_ospf_area_default_cost_set(sys_enable_t bNoFlag, uint32 areaId, ulong_t cost, uint16 ProcessID)
{
  int						iRet;
  SMD_MSG					msg;

  ARG_DEF(4);

  smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_DEFAULT_COST_SET, SMD_GLOBAL, SMD_MSG_SET);

  ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
  ARG_ADD(1, &areaId, 0, sizeof(areaId));
  ARG_ADD(2, &cost, 0, sizeof(cost));
  ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

  iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
  if ( VOS_ERR_NONE != iRet )
  {

  }

  return iRet;

}

int ipc_l3_ospf_area_export_list_set(sys_enable_t bNoFlag, uint32 areaId, char *alistName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_EXPORT_LIST_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, alistName, SMD_ARG_POINTER, (NULL != alistName) ? (strlen(alistName)+1) : 0);
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_area_import_list_set(sys_enable_t bNoFlag, uint32 areaId, char *alistName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_IMPORT_LIST_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, alistName, SMD_ARG_POINTER, (NULL != alistName) ? (strlen(alistName)+1) : 0);
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_area_filter_list_set(sys_enable_t bNoFlag, uint32 areaId,
	char *plistName, char *type_str, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_FILTER_LIST_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, plistName, SMD_ARG_POINTER, (NULL != plistName) ? (strlen(plistName)+1) : 0);
    ARG_ADD(3, type_str, SMD_ARG_POINTER, (NULL != type_str) ? (strlen(type_str)+1) : 0);
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_area_authentication_set(sys_enable_t bNoFlag, uint32 areaId, int auth_type, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_AUTH_TYPE_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, &auth_type, 0, sizeof(auth_type));
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_area_shortcut_set(sys_enable_t bNoFlag, uint32 areaId, char *type_str, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AREA_SHORT_CUT_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &areaId, 0, sizeof(areaId));
    ARG_ADD(2, type_str, SMD_ARG_POINTER, (NULL != type_str) ? (strlen(type_str)+1) : 0);
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_abr_type_set(sys_enable_t bNoFlag, char *abr_type_str, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_ABR_TYPE_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, abr_type_str, SMD_ARG_POINTER, (NULL != abr_type_str) ? (strlen(abr_type_str)+1) : 0);
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_log_adjacency_set(sys_enable_t bNoFlag, int detail, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_LOG_ADJACENCY_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &detail, 0, sizeof(detail));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_rfc1583compatibility_set(sys_enable_t bNoFlag, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_RFC1583_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_spf_timer_set(sys_enable_t bNoFlag, ulong_t delay,
	ulong_t hold, ulong_t max, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_SPF_TIMER_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &delay, 0, sizeof(delay));
    ARG_ADD(2, &hold, 0, sizeof(hold));
    ARG_ADD(3, &max, 0, sizeof(max));
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_static_neighbor_set(sys_enable_t bNoFlag,
	sys_ipv4_t nbr_addr, ulong_t priority, ulong_t interval, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_STATIC_NEIGHBOR_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &nbr_addr, 0, sizeof(nbr_addr));
    ARG_ADD(2, &priority, 0, sizeof(priority));
    ARG_ADD(3, &interval, 0, sizeof(interval));
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_refresh_timer_set(sys_enable_t bNoFlag, ulong_t time, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_REFRESH_TIMER_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &time, 0, sizeof(time));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_auto_cost_set(sys_enable_t bNoFlag, ulong_t refbw, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_AUTO_COST_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &refbw, 0, sizeof(refbw));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_redistribute_set(sys_enable_t bNoFlag, char *protoName, char *metric_str,
                              char *type_str, char *rmapName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(6);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_REDISTRIBUTE_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, protoName, SMD_ARG_POINTER, (NULL != protoName) ? (strlen(protoName)+1) : 0);
    ARG_ADD(2, metric_str, SMD_ARG_POINTER, (NULL != metric_str) ? (strlen(metric_str)+1) : 0);
    ARG_ADD(3, type_str, SMD_ARG_POINTER, (NULL != type_str) ? (strlen(type_str)+1) : 0);
    ARG_ADD(4, rmapName, SMD_ARG_POINTER, (NULL != rmapName) ? (strlen(rmapName)+1) : 0);
	ARG_ADD(5, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 6, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_distri_list_out_set(sys_enable_t bNoFlag, char *alistName, char *protoName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DISTRI_LIST_OUT_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, alistName, SMD_ARG_POINTER, (NULL != alistName) ? (strlen(alistName)+1) : 0);
    ARG_ADD(2, protoName, SMD_ARG_POINTER, (NULL != protoName) ? (strlen(protoName)+1) : 0);
	ARG_ADD(3, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_dft_info_set(sys_enable_t bNoFlag, BOOL always,
	char *metric_str, char *type_str, char *rmapName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(6);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DFT_INFO_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &always, 0, sizeof(always));
    ARG_ADD(2, metric_str, SMD_ARG_POINTER, (NULL != metric_str) ? (strlen(metric_str)+1) : 0);
    ARG_ADD(3, type_str, SMD_ARG_POINTER, (NULL != type_str) ? (strlen(type_str)+1) : 0);
    ARG_ADD(4, rmapName, SMD_ARG_POINTER, (NULL != rmapName) ? (strlen(rmapName)+1) : 0);
	ARG_ADD(5, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 6, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_default_metric_set(sys_enable_t bNoFlag, ulong_t metric, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DEFAULT_METRIC_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &metric, 0, sizeof(metric));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_distance_set(sys_enable_t bNoFlag, ulong_t distance, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DISTANCE_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &distance, 0, sizeof(distance));
	ARG_ADD(2, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_distance_ospf_set(sys_enable_t bNoFlag, ulong_t intra_dis,
	ulong_t inter_dis, ulong_t external_dis, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DISTANCE_OSPF_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, &intra_dis, 0, sizeof(intra_dis));
    ARG_ADD(2, &inter_dis, 0, sizeof(inter_dis));
    ARG_ADD(3, &external_dis, 0, sizeof(external_dis));
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_l3_ospf_distance_prefix_set(sys_enable_t bNoFlag, char *dis_str,
	char *prefix_str, char *alistName, uint16 ProcessID)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(5);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_DISTANCE_PERFIX_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, dis_str, SMD_ARG_POINTER, (NULL != dis_str) ? (strlen(dis_str)+1) : 0);
    ARG_ADD(2, prefix_str, SMD_ARG_POINTER, (NULL != prefix_str) ? (strlen(prefix_str)+1) : 0);
    ARG_ADD(3, alistName, SMD_ARG_POINTER, (NULL != alistName) ? (strlen(alistName)+1) : 0);
	ARG_ADD(4, &ProcessID, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int ipc_ip_ospf_authentication_mode_set(sys_enable_t bNoFlag, char *ifname, int auth_type, sys_ipv4_t addr)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AUTH_MODE_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, &auth_type, 0, sizeof(auth_type));
	ARG_ADD(3, &addr, 0, sizeof(addr));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_ip_ospf_authentication_simple_set(sys_enable_t bNoFlag, char *ifname, char *passwdStr, sys_ipv4_t addr)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AUTH_SIMPLE_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, passwdStr, SMD_ARG_POINTER, (NULL != passwdStr) ? (strlen(passwdStr)+1) : 0);
	ARG_ADD(3, &addr, 0, sizeof(addr));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_ip_ospf_authentication_md5_set(sys_enable_t bNoFlag, char *ifname, ulong_t keyId, char *keyStr, sys_ipv4_t addr)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(5);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_AUTH_MD5_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, &keyId, 0, sizeof(keyId));
	ARG_ADD(3, keyStr, SMD_ARG_POINTER, (NULL != keyStr) ? (strlen(keyStr)+1) : 0);
	ARG_ADD(4, &addr, 0, sizeof(addr));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_ip_ospf_param_set(sys_enable_t bNoFlag, char *ifname, ulong_t type, ulong_t value, sys_ipv4_t addr)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(5);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_PARAM_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, &type, 0, sizeof(type));
	ARG_ADD(3, &value, 0, sizeof(value));
	ARG_ADD(4, &addr, 0, sizeof(addr));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}
int ipc_ip_ospf_dead_set(sys_enable_t bNoFlag, char *ifname, char *interval_str, char *fast_hello_str,
								sys_ipv4_t addr)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(5);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_DEAD_INTERVAL_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, interval_str, SMD_ARG_POINTER, (NULL != interval_str) ? (strlen(interval_str)+1) : 0);
	ARG_ADD(3, fast_hello_str, SMD_ARG_POINTER, (NULL != fast_hello_str) ? (strlen(fast_hello_str)+1) : 0);
	ARG_ADD(4, &addr, 0, sizeof(addr));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 5, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_ip_ospf_network_set(sys_enable_t bNoFlag, char *ifname, int type)
{
	int 					  iRet;
	SMD_MSG 				  msg;

	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_IP_OSPF_NETWORK_SET, SMD_GLOBAL, SMD_MSG_SET);

	ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
	ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
	ARG_ADD(2, &type, 0, sizeof(type));

	iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;

}

int ipc_ip_ospf_mtu_ignore_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr)
{
    int                       iRet;
    SMD_MSG                   msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_FRR, FRR_FUNC_OSPF_MTU_IGNORE_SET, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bNoFlag, 0, sizeof(bNoFlag));
    ARG_ADD(1, ifname, SMD_ARG_POINTER, (NULL != ifname) ? (strlen(ifname)+1) : 0);
    ARG_ADD(2, &addr, 0, sizeof(addr));

    iRet = smdMsgRequestByName(OSPF_SOCK_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

    return iRet;
}

int32 sal_l3_routeV4_ospf_open(uint16 ProcessID)
{
    SYS_ERR_CHK(rsd_ospf_enable_set(ENABLED));

	SYS_ERR_CHK(ipc_l3_ospf_router(ENABLED, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_ospf_cloce(uint16 ProcessID)
{
    SYS_ERR_CHK(rsd_ospf_enable_set(DISABLED));

	SYS_ERR_CHK(ipc_l3_ospf_router(DISABLED, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_ospf_get(sys_enable_t *pBEnable)
{
	SYS_ERR_CHK(ipc_l3_ospf_status_get(pBEnable));

    return SYS_ERR_OK;
}

int32 sal_l3_routeV4_ospf_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbrInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_nbr_info_get(op, pNbrInfo));

	return SYS_ERR_OK;
}

int32 sal_l3_routeV4_ospf_nbma_nbr_info_get(DATA_OP_ENT op, OSPF_NBR_INFO_T *pNbmaNbrInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_nbma_nbr_info_get(op, pNbmaNbrInfo));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_virtual_link_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pVirIntfInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_virtual_link_info_get(op, pVirIntfInfo));

	return SYS_ERR_OK;
}
int32 sal_l3_ospf_intf_info_get(DATA_OP_ENT op, OSPF_INTF_INFO_T *pIntfInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_intf_info_get(op, pIntfInfo));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_info_get(op, pAreaInfo));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_lsa_info_get(DATA_OP_ENT op, OSPF_LSA_INFO_T *pLsaInfo)
{
	SYS_ERR_CHK(ipc_l3_ospf_lsa_info_get(op, pLsaInfo));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_set(sys_enable_t bNoFlag, uint32 areaId, uint16 ProcessID)
{

	SYS_ERR_CHK(ipc_l3_ospf_area_set(bNoFlag, areaId, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_network_area_set(sys_enable_t bNoFlag, sys_ipv4_t ulIp, sys_ipv4_t ulMask, uint32 areaId, uint16 ProcessID)
{

	SYS_ERR_CHK(ipc_l3_ospf_network_area_set(bNoFlag, ulIp, ulMask, areaId, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_range_set(sys_enable_t bNoFlag, uint32 areaId, char *prefix_str, uint32 advertise, uint32 cost, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_range_set(bNoFlag, areaId, prefix_str, advertise, cost, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_range_substitute_set(sys_enable_t bNoFlag, uint32 areaId, char *prefix_src, char *prefix_dst, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_range_substitute_set(bNoFlag, areaId, prefix_src, prefix_dst, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_vlink_set(sys_enable_t bNoFlag, struct ospf_vllink_config_data *vl_config, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_vlink_set(bNoFlag, vl_config, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_stub_set(sys_enable_t bNoFlag, uint32 areaId, int AreaSummaryStatus, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_stub_set(bNoFlag, areaId, AreaSummaryStatus, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_nssa_set(sys_enable_t bNoFlag, uint32 areaId, char *role, int AreaSummaryStatus, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_nssa_set(bNoFlag, areaId, role, AreaSummaryStatus, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_default_cost_set(sys_enable_t bNoFlag, uint32 areaId, uint32 cost, uint16 ProcessID)
{
	int32 rc = SYS_ERR_OK;
	rc = ipc_l3_ospf_area_default_cost_set(bNoFlag, areaId, cost, ProcessID);
	if (rc != SYS_ERR_OK)
	{
		rc = SYS_ERR_FAILED;
	}

    return rc;
}

int32 sal_l3_ospf_area_export_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_export_list_set(bNoFlag, areaId, list_name, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_import_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_import_list_set(bNoFlag, areaId, list_name, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_filter_list_set(sys_enable_t bNoFlag, uint32 areaId, char *list_name, char *type_str, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_filter_list_set(bNoFlag, areaId, list_name, type_str, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_authentication_set(sys_enable_t bNoFlag, uint32 areaId, int auth_type, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_authentication_set(bNoFlag, areaId, auth_type, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_area_shortcut_set(sys_enable_t bNoFlag, uint32 areaId, char *type_str, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_area_shortcut_set(bNoFlag, areaId, type_str, ProcessID));

    return SYS_ERR_OK;
}

int32 sal_l3_ospf_router_id_set(sys_enable_t bNoFlag, sys_ipv4_t router_id, uint16 ProcessID)
{

	SYS_ERR_CHK(ipc_l3_ospf_router_id_set(bNoFlag, router_id, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_abr_type_set(sys_enable_t bNoFlag, char *abr_type_str, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_abr_type_set(bNoFlag, abr_type_str, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_log_adjacency_set(sys_enable_t bNoFlag, int detail, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_log_adjacency_set(bNoFlag, detail, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_rfc1583compatibility_set(sys_enable_t bNoFlag, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_rfc1583compatibility_set(bNoFlag, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_spf_timer_set(sys_enable_t bNoFlag, uint32 delay, uint32 hold, uint32 max, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_spf_timer_set(bNoFlag, delay, hold, max, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_static_neighbor_set(sys_enable_t bNoFlag, uint32 nbrId, uint32 priority, uint32 interval, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_static_neighbor_set(bNoFlag, nbrId, priority, interval, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_refresh_timer_set(sys_enable_t bNoFlag, uint32 time, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_refresh_timer_set(bNoFlag, time, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_auto_cost_set(sys_enable_t bNoFlag, uint32 refbw, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_auto_cost_set(bNoFlag, refbw, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_redistribute_set(sys_enable_t bNoFlag, char *protoName,
	char *metric_str, char *type_str, char *rmapName, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_redistribute_set(bNoFlag, protoName, metric_str, type_str, rmapName, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_dft_info_set(sys_enable_t bNoFlag, sys_enable_t always,
	char *metric_str, char *type_str, char *rmapName, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_dft_info_set(bNoFlag, always, metric_str, type_str, rmapName, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_distri_list_out_set(sys_enable_t bNoFlag, char *alistName, char *protoName, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_distri_list_out_set(bNoFlag, alistName, protoName, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_default_metric_set(sys_enable_t bNoFlag, uint32 default_metric, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_default_metric_set(bNoFlag, default_metric, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_distance_ospf_set(sys_enable_t bNoFlag, uint32 intra_dis,
	uint32 inter_dis, uint32 external_dis, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_distance_ospf_set(bNoFlag, intra_dis, inter_dis, external_dis, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_distance_prefix_set(sys_enable_t bNoFlag, char *dis_str,
	char *prefix_str, char *alistName, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_distance_prefix_set(bNoFlag, dis_str, prefix_str, alistName, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_distance_set(sys_enable_t bNoFlag, uint32 distance, uint16 ProcessID)
{
	SYS_ERR_CHK(ipc_l3_ospf_distance_set(bNoFlag, distance, ProcessID));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_passive_interface_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_l3_ospf_passive_interface_set(bNoFlag, ifname, addr));

	return SYS_ERR_OK;
}

int32 sal_l3_ospf_passive_interface_default(sys_enable_t bNoFlag)
{
	SYS_ERR_CHK(ipc_l3_ospf_passive_interface_default(bNoFlag));

	return SYS_ERR_OK;
}

int32 sal_ip_ospf_authentication_mode_set(sys_enable_t bNoFlag, char *ifname, int auth_type, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_ip_ospf_authentication_mode_set(bNoFlag, ifname, auth_type, addr));

	return SYS_ERR_OK;
}

int32 sal_ip_ospf_authentication_simple_set(sys_enable_t bNoFlag, char *ifname, char *passwdStr, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_ip_ospf_authentication_simple_set(bNoFlag, ifname, passwdStr, addr));

	return SYS_ERR_OK;
}

int32 sal_ip_ospf_authentication_md5_set(sys_enable_t bNoFlag, char *ifname, ulong_t keyId, char *keyStr, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_ip_ospf_authentication_md5_set(bNoFlag, ifname, keyId, keyStr, addr));

	return SYS_ERR_OK;
}

int32 sal_ip_ospf_cost_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_ip_ospf_param_set(bNoFlag, ifname, OSPF_CONFIG_COST, value, addr));

	return SYS_ERR_OK;
}

 int32 sal_ip_ospf_priority_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_param_set(bNoFlag, ifname, OSPF_CONFIG_PRIORITY, value, addr));

	 return SYS_ERR_OK;
 }

 int32 sal_ip_ospf_hello_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_param_set(bNoFlag, ifname, OSPF_CONFIG_HELLO, value, addr));

	 return SYS_ERR_OK;
 }
 int32 sal_ip_ospf_dead_set(sys_enable_t bNoFlag, char *ifname, char *interval_str, char *fast_hello_str, sys_ipv4_t addr)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_dead_set(bNoFlag, ifname, interval_str, fast_hello_str, addr));

	 return SYS_ERR_OK;
 }

 int32 sal_ip_ospf_transmit_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_param_set(bNoFlag, ifname, OSPF_CONFIG_TRANSMIT, value, addr));

	 return SYS_ERR_OK;
 }
  int32 sal_ip_ospf_retransmit_set(sys_enable_t bNoFlag, char *ifname, ulong_t value, sys_ipv4_t addr)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_param_set(bNoFlag, ifname, OSPF_CONFIG_RETRANSMIT, value, addr));

	 return SYS_ERR_OK;
 }

 int32 sal_ip_ospf_network_set(sys_enable_t bNoFlag, char *ifname, int type)
 {
	 SYS_ERR_CHK(ipc_ip_ospf_network_set(bNoFlag, ifname, type));

	 return SYS_ERR_OK;
 }

int32 sal_ip_ospf_mtu_ignore_set(sys_enable_t bNoFlag, char *ifname, sys_ipv4_t addr)
{
	SYS_ERR_CHK(ipc_ip_ospf_mtu_ignore_set(bNoFlag, ifname, addr));

	return SYS_ERR_OK;
}

int32 sal_ospf_Info_get(OSPF_INFO_T *pOspf)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pOspf), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_info_get(pOspf);

    return iRet;
}

int32 sal_ospf_area_info_get(DATA_OP_ENT op, OSPF_AREA_INFO_T *pAreaInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pAreaInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_area_info_get(op, pAreaInfo);

    return iRet;
}

int32 sal_ospf_process_status_get(DATA_OP_ENT op, uint16* pProcess)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pProcess), SYS_ERR_NULL_POINTER);

	iRet = ipc_process_status_get(op, pProcess);

    return iRet;
}

int32 sal_ospf_area_network_get(DATA_OP_ENT op, OSPF_AREA_NETWORK_T* network)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == network), SYS_ERR_NULL_POINTER);

	iRet = ipc_area_network_get(op, network);

    return iRet;
}

int32 sal_ospf_area_range_get(DATA_OP_ENT op, OSPF_AREA_RANGE_T* range)
{
	int					   iRet;

	SYS_PARAM_CHK((NULL == range), SYS_ERR_NULL_POINTER);

	iRet = ipc_area_range_get(op, range);

	return iRet;
}

int32 sal_ospf_route_info_get(DATA_OP_ENT op, OSPF_ROUTE_INFO_T* pRouteInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pRouteInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_route_info_get(op, pRouteInfo);

    return iRet;
}

int32 sal_ospf_route_border_info_get(DATA_OP_ENT op, OSPF_RTR_ROUTE_INFO_T* pRouteInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pRouteInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_route_border_info_get(op, pRouteInfo);

    return iRet;
}

int32 sal_ospf_passive_interface_get(DATA_OP_ENT op, OSPF_PASSIVE_INTF_INFO_T *pIntfInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pIntfInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_passive_interface_get(op, pIntfInfo);

    return iRet;
}

int32 sal_ospf_redist_nfo_get(OSPF_REDIST_INFO_T *pRedistInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pRedistInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_redist_info_get(pRedistInfo);

    return iRet;
}

int32 sal_ospf_distance_get(DATA_OP_ENT op, OSPF_DISTANCE_INFO_T *pDistInfo)
{
	int                       iRet;

	SYS_PARAM_CHK((NULL == pDistInfo), SYS_ERR_NULL_POINTER);

	iRet = ipc_ospf_distance_get(op, pDistInfo);

    return iRet;
}

