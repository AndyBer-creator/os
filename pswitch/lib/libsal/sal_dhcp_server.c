
#include <common/sys_def.h>
#include <libsal/sal_util.h>
#include <libsal/sal_dhcp_server.h>
#include <libipc/smdMsgRequest.h>
#include <libipc/vosMsg.h>
#include <libipc/vos.h>

#define GROUP_MIX 1
#define GROUP_MAX 256
#define SYS_GROUP_PARAM_CHK(groupid, errCode)               \
do {                                                        \
    if ( groupid < GROUP_MIX || groupid > GROUP_MAX) {      \
        return errCode;                                     \
    }                                                       \
} while (0)

#define SYS_VLAN_PARAM_CHK(vid, errCode)               \
do {                                                        \
    if ( vid < 1 || vid > 4094) {      \
        return errCode;                                     \
    }                                                       \
} while (0)

#define SYS_INTFNUM_PARAM_CHK(intnum, errCode)               \
do {                                                        \
    if ( intnum < 1 || intnum > 32) {      \
        return errCode;                                     \
    }                                                       \
} while (0)

static int dhcp_err_to_sys_err(int errcode)
{
	int rc = 0;
	switch(errcode)
	{
		case DHCP_ERROR_SECTION_STARTIP_INVALID:
			rc = SYS_ERR_DHCP_SECTION_STARTIP_INVALID;
			break;
		case DHCP_ERROR_SECTION_ENDIP_INVALID:
			rc = SYS_ERR_DHCP_SECTION_ENDIP_INVALID;
			break;
		case DHCP_ERROR_SECTION_STARTIP_HIGHER:
			rc = SYS_ERR_DHCP_SECTION_STARTIP_HIGHER;
			break;
		case DHCP_ERROR_SECTION_LENGTH_TOOBIG:
			rc = SYS_ERR_DHCP_SECTION_LENGTH_TOOBIG;
			break;
		case DHCP_ERROR_SECTION_NO_GATEWAY:
			rc = SYS_ERR_DHCP_SECTION_NO_GATEWAY;
			break;
		case DHCP_ERROR_SECTION_IPGATE_NOSAMESUBNET:
			rc = SYS_ERR_DHCP_SECTION_IPGATE_NOSAMESUBNET;
			break;		
		case DHCP_ERROR_SECTION_OVERLAP:
			rc = SYS_ERR_DHCP_SECTION_OVERLAP;
			break;
		case DHCP_ERROR_SECTION_NUM_FULL:
			rc = SYS_ERR_DHCP_SECTION_NUM_FULL;
			break;
			case DHCP_ERROR_SECTION_IP_INUSED:
			rc = SYS_ERR_DHCP_SECTION_IP_INUSED;
			break;
	}
	return rc;
}
int32 sal_dhcp_server_db_init(void)
{
	sys_mac_t macaddr;
    sys_boardModel_t boardModel;
	uint32 sysOid;

    SYS_MEM_CLEAR(boardModel);

    SYS_ERR_CHK(cfg_dhcp_server_db_init());

    SYS_ERR_CHK(sal_sys_macAddr_get(&macaddr));
	SYS_ERR_CHK(ipc_dhcp_system_mac_set(&macaddr));

    SYS_ERR_CHK(sal_sys_boardModel_get(&boardModel));
	sysOid = boardModel.oid.oidAry[6];
	SYS_ERR_CHK(ipc_dhcp_system_oid_set(sysOid));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer)
{
    SYS_PARAM_CHK((NULL == pDhcpServer), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_get(pDhcpServer));
    return SYS_ERR_OK;
}

int32 ipc_dhcp_server_enable_set(sys_enable_t enable)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_ENABLE,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_server_group_ip_set(uint32 uiGroupId, sys_ipv4_t ipv4)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_SERVER_IP_SET,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &uiGroupId, 0, sizeof(uint32));
    ARG_ADD(1, &ipv4, 0, sizeof(sys_ipv4_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_server_group_set(uint32 uiGroupId, uint32 vid, BOOL add)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_GROUP_SET,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &vid, 0, sizeof(uint32));
    ARG_ADD(1, &add, 0, sizeof(BOOL));
    ARG_ADD(2, &uiGroupId, 0, sizeof(uint32));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_enable_set(sys_enable_t enable)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_device_enable_set(sys_enable_t enable)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_DEV_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_format_set(uint32 format)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_FORMAT_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &format, 0, sizeof(uint32));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_info_format_set(uint32 iFormat)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_INFO_FORMAT_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, &iFormat, 0, sizeof(uint32));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_format_verbose_set(char *option82VerboseNodeId)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_FORMAT_VERBOSE,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, option82VerboseNodeId, SMD_ARG_POINTER, strlen(option82VerboseNodeId) + 1);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_PORT_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &port, 0, sizeof(sys_logic_port_t));
	ARG_ADD(1, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_port_strategy_set(sys_logic_port_t port, uint32 strategy)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_PORT_STRATEGY_CFG,
        SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &port, 0, sizeof(sys_logic_port_t));
	ARG_ADD(1, &strategy, 0, sizeof(uint32));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_port_circuit_id_set(sys_logic_port_t port, char *option82CircuitId)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_PORT_CIRCUITID,
        SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &port, 0, sizeof(sys_logic_port_t));
    ARG_ADD(1, option82CircuitId, SMD_ARG_POINTER, strlen(option82CircuitId) + 1);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_dhcp_relay_option82_port_remote_id_set(sys_logic_port_t port, char *option82RemoteId)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_OPTION82_PORT_REMOTEID,
        SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &port, 0, sizeof(sys_logic_port_t));
    ARG_ADD(1, option82RemoteId, SMD_ARG_POINTER, strlen(option82RemoteId) + 1);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_ip_pool_set(char *name, BOOL add)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IP_POOL_SET,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
    ARG_ADD(1, &add, 0, sizeof(BOOL));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_ip_pool_gateway_set(char *name, sys_ipv4_t addr, sys_ipv4_t mask)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_GATEWAY_SET,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
    ARG_ADD(1, &addr, 0, sizeof(sys_ipv4_t));
    ARG_ADD(2, &mask, 0, sizeof(sys_ipv4_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }
    return iRet;
}

int32 ipc_ip_pool_section_set(char *name, uint32 id, sys_ipv4_t start_ip, sys_ipv4_t end_ip)
{
	int                      iRet;
	SMD_MSG 				 msg;
	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_SECTION_SET,
        SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
    ARG_ADD(1, &id, 0, sizeof(uint32));
    ARG_ADD(2, &start_ip, 0, sizeof(sys_ipv4_t));
    ARG_ADD(3, &end_ip, 0, sizeof(sys_ipv4_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        return dhcp_err_to_sys_err(iRet);
    }
    return iRet;
}

int32 ipc_dhcp_relay_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_PORT_RELAY_ENABLE,
		SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &port, 0, sizeof(sys_logic_port_t));
	ARG_ADD(1, &enable, 0, sizeof(sys_enable_t));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_server_IpBind_Switch_enable_set(sys_enable_t enable)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(1);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IP_BIND_SWITCH_SET,
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}
int32 ipc_dhcp_client_host_mac_ip_add(sys_mac_t* pMac, sys_ipv4_t ipaddr, uint16 vid, char* pUserName)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_ADDTO_MACLIST,
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, pMac, SMD_ARG_POINTER, 6);
    ARG_ADD(1, &ipaddr, 0,  sizeof(sys_ipv4_t));
    ARG_ADD(2, &vid, 0, sizeof(uint16));
    ARG_ADD(3, pUserName, SMD_ARG_POINTER, strlen(pUserName) + 1);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {
        if(iRet == 2){
        	iRet = SYS_ERR_DUPLICATE_ENTRY;
        } else if(iRet == 3){
			iRet = SYS_ERR_ENTRY_FULL;
        }       
    }

    return iRet;
}

int32 ipc_dhcp_client_host_mac_ip_del(sys_mac_t* pMac, uint16 vid)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_DELFROM_MACLIST,
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, pMac, SMD_ARG_POINTER, 6);
    ARG_ADD(1, &vid, 0, sizeof(uint16));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_dhcp_client_host_mac_ip_get(uint16 usStart, uint16 *pusNextStart, uint16 *pusBufNum,
		dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN])
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_MACLIST_GET, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &usStart, 0, sizeof(uint16));
    ARG_ADD(1, pusNextStart, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint16));
    ARG_ADD(2, pusBufNum, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint16));
    ARG_ADD(3, nodeBuf, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(dhcp_snooping_mac_lst_t) * IPPOOL_MAC_BUF_LEN);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_dhcp_client_unknown_client_assign(sys_enable_t enable)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(1);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_UNBIND_ASSIGN_SET,
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &enable, 0, sizeof(sys_enable_t));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_dhcp_client_hash_get(uint16 usStart, uint16 *pusNextStart,
	uint16 *pusBufNum, MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX])
{
	int 					 iRet;
	SMD_MSG 				 msg;

	ARG_DEF(4);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_HASH_DATA_GET, SMD_GLOBAL, SMD_MSG_GET);

	ARG_ADD(0, &usStart, 0, sizeof(uint16));
	ARG_ADD(1, pusNextStart, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint16));
	ARG_ADD(2, pusBufNum, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint16));
	ARG_ADD(3, hostBuf, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(MACHASH_S) * DHCPRS_HOST_BUF_MAX);

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int ipc_dhcp_client_ip_check_get(uint32 ipaddr, uint32* pUserd)
{
	int 					 iRet;
	SMD_MSG 				 msg;

	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_STATIC_IP_CHECK_GET, SMD_GLOBAL, SMD_MSG_GET);

	ARG_ADD(0, &ipaddr, 0, sizeof(uint32));
	ARG_ADD(1, pUserd, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}

	return iRet;
}

int32 ipc_dhcp_server_dns_set(char *name, uint32 dnsNum, sys_ipv4_t ip)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_DNS_IP_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
	ARG_ADD(1, &dnsNum, 0, sizeof(uint32));
    ARG_ADD(2, &ip, 0,  sizeof(sys_ipv4_t));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_server_dns_suffix_set(char *name, char *dnsSuffixName)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_DNS_SUFFIX_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
    ARG_ADD(1, dnsSuffixName, SMD_ARG_POINTER, strlen(dnsSuffixName) + 1);

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_server_wins_set(char *name, uint32 winsNum, sys_ipv4_t ip)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(3);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_WINS_IP_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
	ARG_ADD(1, &winsNum, 0, sizeof(uint32));
    ARG_ADD(2, &ip, 0,  sizeof(sys_ipv4_t));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_server_lease_time_set(char *name, uint32 lease_time)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_LEASETIME_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
	ARG_ADD(1, &lease_time, 0, sizeof(uint32));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_ip_pool_option43_set(char *name, sys_ippool_option43_t *option43)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(2);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_OPTION43_SET_BY_INDEX,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, name, SMD_ARG_POINTER, strlen(name) + 1);
    ARG_ADD(1, option43, SMD_ARG_POINTER, sizeof(sys_ippool_option43_t));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_system_mac_set(sys_mac_t *pMac)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_SYSTEM_MAC_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, pMac, SMD_ARG_POINTER, 6);

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_system_oid_set(uint32 oid)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_SYSTEM_OID_SET,
		SMD_GLOBAL, SMD_MSG_SET);
	ARG_ADD(0, &oid, 0, sizeof(uint32));

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int32 ipc_dhcp_hostname_set(char *hostname)
{
	int 					 iRet;
	SMD_MSG 				 msg;
	ARG_DEF(1);

	smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_HOSTNAME_SET,
		SMD_GLOBAL, SMD_MSG_SET);
    ARG_ADD(0, hostname, SMD_ARG_POINTER, strlen(hostname) + 1);

	iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 1, VOS_TIMEOUT_DEFAULT);
	if ( VOS_ERR_NONE != iRet )
	{

	}
	return iRet;
}

int ipc_ipPoolDisableIp(BOOL bDisable, char *szPoolName, sys_ipv4_t ulIp, int *piErrNo)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(4);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_DISABLE_IP, 
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &bDisable, 0, sizeof(BOOL));
    ARG_ADD(1, szPoolName, SMD_ARG_POINTER, strlen(szPoolName) + 1);
    ARG_ADD(2, &ulIp, 0, sizeof(sys_ipv4_t));
    ARG_ADD(3, piErrNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(int));
    
    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_ipPoolDisableIp_get(char *szPoolName, sys_ipv4_t ulIp[GET_NUM_ONE])
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(2);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IPPOOL_DISABLE_IP_GET, 
        SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, szPoolName, SMD_ARG_POINTER, strlen(szPoolName) + 1);
    ARG_ADD(1, ulIp, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(sys_ipv4_t)*GET_NUM_ONE);

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 2, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int ipc_dhcprelayIfGroupOpGet(DATA_OP_ENT op, uint32 *pulSw, uint32 *pVid, uint32 groupNo[DHCPINTERFACEMAXNUM])
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(4);
    
    if ( (DATA_OP_GET != op) && (DATA_OP_GETNEXT != op) )
    {
        return VOS_ERR_FAIL;
    }

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_IF_GROUP_OP, SMD_GLOBAL, SMD_MSG_GET);

    ARG_ADD(0, &op, 0, sizeof(DATA_OP_ENT));
    ARG_ADD(1, pulSw, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32));
    ARG_ADD(2, pVid, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32));
    ARG_ADD(3, groupNo, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32) * DHCPINTERFACEMAXNUM);
    
    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 4, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 ipc_dhcp_server_check_group_used(uint32 group, uint32* pUsed, uint32* vid)
{
    int                      iRet;
    SMD_MSG                  msg;

    ARG_DEF(3);

    smdMsgInit(&msg, SMD_MSG_TYPE_APP, DHCPS_FUNC_CHECK_GROUPNUM_USED, SMD_GLOBAL, SMD_MSG_SET);

    ARG_ADD(0, &group, 0, sizeof(uint32));
    ARG_ADD(1, pUsed, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32));
	ARG_ADD(2, vid, (SMD_ARG_POINTER | SMD_ARG_OUTPUT), sizeof(uint32));

    iRet = smdMsgRequestByName(DHCP_SERVER_PATH, &msg, ARG_GET(), 3, VOS_TIMEOUT_DEFAULT);
    if ( VOS_ERR_NONE != iRet )
    {

    }

    return iRet;
}

int32 sal_dhcp_server_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_dhcp_server_enable_set(enable));

	SYS_ERR_CHK(ipc_dhcp_server_enable_set(enable));

    SYS_ERR_CHK(cfg_dhcp_server_enable_set(enable));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_server_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_enable_get(pEnable));

    return SYS_ERR_OK;
} 

int32 sal_dhcp_server_group_ip_add(uint32 uiGroupId, sys_ipv4_t ipv4)
{
    SYS_GROUP_PARAM_CHK(uiGroupId, SYS_ERR_GROUP_NOT_EXIST);

	SYS_ERR_CHK(ipc_dhcp_server_group_ip_set(uiGroupId, ipv4));

    SYS_ERR_CHK(cfg_dhcp_server_group_ip_set(uiGroupId, ipv4));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_group_ip_del(uint32 uiGroupId)
{
	sys_ipv4_t ipv4 = 0;
    SYS_GROUP_PARAM_CHK(uiGroupId, SYS_ERR_GROUP_NOT_EXIST);

	SYS_ERR_CHK(ipc_dhcp_server_group_ip_set(uiGroupId, ipv4));

    SYS_ERR_CHK(cfg_dhcp_server_group_ip_set(uiGroupId, ipv4));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_group_ip_get(uint32 uiGroupId, sys_ipv4_t *ipv4, uint32 *pIntf)
{
    SYS_GROUP_PARAM_CHK(uiGroupId, SYS_ERR_GROUP_NOT_EXIST);
    SYS_PARAM_CHK((NULL == ipv4), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_group_ip_get(uiGroupId, ipv4, pIntf));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_group_ip_clear(void)
{
	int group;
	for(group = GROUP_MIX; group <= GROUP_MAX; group++)
	{
   	 	SYS_ERR_CHK(sal_dhcp_server_group_ip_del(group));
    }

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_intf_group_ip_add(uint32 uiGroupId, uint32 vid)
{
    SYS_GROUP_PARAM_CHK(uiGroupId, SYS_ERR_GROUP_NOT_EXIST);
    SYS_VLAN_PARAM_CHK(vid, SYS_ERR_VLAN_NOT_EXIST);

	SYS_ERR_CHK(ipc_dhcp_server_group_set(uiGroupId, vid, TRUE));

    SYS_ERR_CHK(cfg_dhcp_server_intf_group_ip_add(uiGroupId, vid));
	SYS_ERR_CHK(cfg_dhcp_server_group_use_stat_set(uiGroupId, vid));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_intf_group_ip_del(uint32 uiGroupId, uint32 vid)
{
    SYS_GROUP_PARAM_CHK(uiGroupId, SYS_ERR_GROUP_NOT_EXIST);

	SYS_ERR_CHK(ipc_dhcp_server_group_set(uiGroupId, vid, FALSE));

    SYS_ERR_CHK(cfg_dhcp_server_intf_group_ip_del(uiGroupId, vid));
    SYS_ERR_CHK(cfg_dhcp_server_group_use_stat_set(uiGroupId, 0));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_intf_group_ip_get(DATA_OP_ENT op, uint32* intfId, uint32* pVid, uint32 groupNo[DHCP_INTERFACE_MAX_NUM])
{
	SYS_PARAM_CHK((NULL == intfId), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(ipc_dhcprelayIfGroupOpGet(op, intfId, pVid, groupNo));
    return SYS_ERR_OK;
}

int32 sal_dhcp_server_intf_group_by_vid_get(uint32 vid, uint32 groupNo[DHCP_INTERFACE_MAX_NUM])
{
	sys_intf_dhcp_t data;
	int intf;

	SYS_VLAN_PARAM_CHK(vid, SYS_ERR_FAILED);

    for(intf = 0; intf < L3INTFNUM; intf++)
    {
    	SYS_ERR_CHK(cfg_dhcp_server_intf_group_ip_get(intf, &data));
    	if(data.vid == vid)
    	{
			memcpy(groupNo, data.groupNo, sizeof(uint32) * DHCP_INTERFACE_MAX_NUM);
			break;
    	}
    }
    return SYS_ERR_OK;
}

int32 sal_dhcp_server_intf_group_ip_init(void)
{
	uint32 intfid;
	for(intfid = 0; intfid < L3INTFNUM; intfid++)
	{
		SYS_ERR_CHK(cfg_dhcp_server_intf_group_ip_init(intfid));
	}
    return SYS_ERR_OK;
}

int32 sal_dhcp_server_check_group_used(uint32 group, uint32* pUsed, uint32* vid)
{
    SYS_PARAM_CHK((NULL == pUsed), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == vid), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(ipc_dhcp_server_check_group_used(group, pUsed, vid));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	ipc_dhcp_relay_option82_enable_set(enable);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_device_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	ipc_dhcp_relay_option82_device_enable_set(enable);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_device_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_device_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_device_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_format_set(uint32 format)
{
	ipc_dhcp_relay_option82_format_set(format);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_format_set(format));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_format_get(uint32 *format)
{
    SYS_PARAM_CHK((NULL == format), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_format_get(format));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_info_format_set(uint32 iFormat)
{
	ipc_dhcp_relay_option82_info_format_set(iFormat);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_info_format_set(iFormat));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_info_format_get(uint32 *iFormat)
{
    SYS_PARAM_CHK((NULL == iFormat), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_info_format_get(iFormat));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_format_verbose_set(char *option82VerboseNodeId)
{
    SYS_PARAM_CHK((NULL == option82VerboseNodeId), SYS_ERR_NULL_POINTER);

	ipc_dhcp_relay_option82_format_verbose_set(option82VerboseNodeId);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_format_verbose_set(option82VerboseNodeId));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_format_verbose_get(char *option82VerboseNodeId)
{
    SYS_PARAM_CHK((NULL == option82VerboseNodeId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_format_verbose_get(option82VerboseNodeId));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	ipc_dhcp_relay_option82_port_enable_set(port, enable);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_enable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_strategy_set(sys_logic_port_t port, uint32 strategy)
{
	ipc_dhcp_relay_option82_port_strategy_set(port, strategy);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_strategy_set(port, strategy));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_strategy_get(sys_logic_port_t port, uint32 *pStrategy)
{
    SYS_PARAM_CHK((NULL == pStrategy), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_strategy_get(port, pStrategy));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_circuit_id_set(sys_logic_port_t port, char *option82CircuitId)
{
	ipc_dhcp_relay_option82_port_circuit_id_set(port, option82CircuitId);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_circuit_id_set(port, option82CircuitId));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_circuit_id_get(sys_logic_port_t port, char *option82CircuitId)
{
    SYS_PARAM_CHK((NULL == option82CircuitId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_relay_option82_port_circuit_id_get(port, option82CircuitId));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_remote_id_set(sys_logic_port_t port, char *option82RemoteId)
{
	ipc_dhcp_relay_option82_port_remote_id_set(port, option82RemoteId);

	SYS_ERR_CHK(cfg_dhcp_relay_option82_port_remote_id_set(port, option82RemoteId));

	return SYS_ERR_OK;
}

int32 sal_dhcp_relay_option82_port_remote_id_get(sys_logic_port_t port, char *option82RemoteId)
{
	SYS_PARAM_CHK((NULL == option82RemoteId), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(cfg_dhcp_relay_option82_port_remote_id_get(port, option82RemoteId));

	return SYS_ERR_OK;
}

int32 sal_ip_pool_create(char *name)
{
	SYS_ERR_CHK(ipc_ip_pool_set(name, TRUE));
    SYS_ERR_CHK(cfg_ip_pool_create(name));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_del(char *name)
{
	SYS_ERR_CHK(ipc_ip_pool_set(name, FALSE));
    SYS_ERR_CHK(cfg_ip_pool_del(name));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_get_by_name(char *name, ippool_t *pPool)
{
    SYS_ERR_CHK(cfg_ip_pool_get(name, pPool));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_gateway_set(char *name, sys_ipv4_t addr, sys_ipv4_t mask)
{
	SYS_ERR_CHK(ipc_ip_pool_gateway_set(name, addr, mask));
    SYS_ERR_CHK(cfg_ip_pool_gateway_set(name, addr, mask));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_section_set(char *name, uint32 id, sys_ipv4_t start_ip, sys_ipv4_t end_ip)
{
	SYS_ERR_CHK(ipc_ip_pool_section_set(name, id, start_ip, end_ip));
    SYS_ERR_CHK(cfg_ip_pool_section_set(name, id, start_ip, end_ip));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_disable_ip_set(char *name, uint32 enable, sys_ipv4_t ipv4)
{
	int err = 0;
	SYS_ERR_CHK(ipc_ipPoolDisableIp( enable, name, ipv4, &err));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_disable_ip_get(char *name, sys_ipv4_t ipv4[GET_NUM_ONE])
{
   	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
	SYS_PARAM_CHK((NULL == ipv4), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(ipc_ipPoolDisableIp_get(name, ipv4));

    return SYS_ERR_OK;
}

int32 sal_ip_pool_option43_set(char *name, sys_ippool_option43_t *option43)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(ipc_ip_pool_option43_set(name, option43));
    SYS_ERR_CHK(cfg_ip_pool_option43_set(name, option43));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_ERR_CHK(ipc_dhcp_relay_port_enable_set(port, enable));
    SYS_ERR_CHK(cfg_dhcp_relay_port_enable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_relay_port_enable_get(sys_logic_port_t port, sys_enable_t *enable)
{
    SYS_PARAM_CHK((NULL == enable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(cfg_dhcp_relay_port_enable_get(port, enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_IpBind_Switch_enable_set(sys_enable_t enable)
{
   	SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	SYS_ERR_CHK(ipc_dhcp_server_IpBind_Switch_enable_set(enable));

    SYS_ERR_CHK(cfg_dhcp_server_IpBind_Switch_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_client_host_mac_ip_add(sys_mac_t* pMac, sys_ipv4_t ipaddr, sys_vid_t vid, char* pUserName)
{
	uint32 userd = FALSE;

   	SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);
   	SYS_PARAM_CHK((NULL == pUserName), SYS_ERR_NULL_POINTER);
    SYS_VLAN_PARAM_CHK(vid, SYS_ERR_VLAN_NOT_EXIST);    

	SYS_ERR_CHK(ipc_dhcp_client_ip_check_get(ipaddr, &userd));
	if(userd == FALSE)
	{
		return SYS_ERR_DHCP_STATIC_IP_INVALID;
	}
	SYS_ERR_CHK(ipc_dhcp_client_host_mac_ip_add(pMac, ipaddr, (uint16)vid, pUserName));

    return SYS_ERR_OK;
}

int32 sal_dhcp_client_host_mac_ip_del(sys_mac_t* pMac, sys_vid_t vid)
{
   	SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);
    SYS_VLAN_PARAM_CHK(vid, SYS_ERR_VLAN_NOT_EXIST);

	SYS_ERR_CHK(ipc_dhcp_client_host_mac_ip_del(pMac, (uint16)vid));

    return SYS_ERR_OK;
}

int32 sal_dhcp_client_host_mac_ip_get(uint16 usStart, uint16 *pusNextStart, uint16 *pusBufNum,
		dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN])
{
   	SYS_PARAM_CHK((NULL == pusNextStart), SYS_ERR_NULL_POINTER);
   	SYS_PARAM_CHK((NULL == pusBufNum), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(ipc_dhcp_client_host_mac_ip_get(usStart, pusNextStart, pusBufNum, nodeBuf));

    return SYS_ERR_OK;
}

int32 sal_dhcp_client_unknown_client_assign(sys_enable_t enable)
{
   	SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

	SYS_ERR_CHK(ipc_dhcp_client_unknown_client_assign(enable));

    SYS_ERR_CHK(cfg_dhcp_client_unknown_client_assign(enable));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_IpBind_get(sys_enable_t* enable, sys_enable_t* unBoundAssign)
{
   	SYS_PARAM_CHK((NULL == enable), SYS_ERR_NULL_POINTER);
   	SYS_PARAM_CHK((NULL == unBoundAssign), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_IpBind_get(enable, unBoundAssign));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_dns_set(char *name, uint32 dnsNum, sys_ipv4_t ip)
{
	SYS_ERR_CHK(ipc_dhcp_server_dns_set(name, dnsNum, ip));
    SYS_ERR_CHK(cfg_dhcp_server_dns_set(name, dnsNum, ip));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_dns_get(char *name, uint32 dnsNum, sys_ipv4_t *ip)
{
    SYS_PARAM_CHK((NULL == ip), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_dns_get(name, dnsNum, ip));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_dns_suffix_set(char *name, char *dnsSuffixName)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == dnsSuffixName), SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(ipc_dhcp_server_dns_suffix_set(name, dnsSuffixName));
    SYS_ERR_CHK(cfg_dhcp_server_dns_suffix_set(name, dnsSuffixName));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_dns_suffix_get(char *name, char *dnsSuffixName)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == dnsSuffixName), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(cfg_dhcp_server_dns_suffix_get(name, dnsSuffixName));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_wins_set(char *name, uint32 winsNum, sys_ipv4_t ip)
{
	SYS_ERR_CHK(ipc_dhcp_server_wins_set(name, winsNum, ip));
    SYS_ERR_CHK(cfg_dhcp_server_wins_set(name, winsNum, ip));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_wins_get(char *name, uint32 winsNum, sys_ipv4_t *ip)
{
    SYS_PARAM_CHK((NULL == ip), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_wins_get(name, winsNum, ip));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_lease_time_set(char *name, uint32 lease_time)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
	SYS_ERR_CHK(ipc_dhcp_server_lease_time_set(name, lease_time));
    SYS_ERR_CHK(cfg_dhcp_server_lease_time_set(name, lease_time));

    return SYS_ERR_OK;
}

int32 sal_dhcp_server_lease_time_get(char *name, uint32 *lease_time)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == lease_time), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_dhcp_server_lease_time_get(name, lease_time));

    return SYS_ERR_OK;
}

int32 sal_dhcp_client_hash_get(uint16 usStart, uint16 *pusNextStart,
    uint16 *pusBufNum, MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX])
{
   	SYS_PARAM_CHK((NULL == pusNextStart), SYS_ERR_NULL_POINTER);
   	SYS_PARAM_CHK((NULL == pusBufNum), SYS_ERR_NULL_POINTER);

	SYS_ERR_CHK(ipc_dhcp_client_hash_get(usStart, pusNextStart, pusBufNum, hostBuf));

    return SYS_ERR_OK;
}

