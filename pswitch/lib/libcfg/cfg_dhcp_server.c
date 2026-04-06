
#include <libcfg/cfg.h>

int32 cfg_dhcp_server_db_init(void)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
    sys_logic_port_t lp;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memset(dhcp_server, 0, sizeof(cfg_dhcp_server_t));

    FOR_EACH_LOGIC_PORT(lp)
	{
		dhcp_server->portOption82[lp].option82Enabled = ENABLED;
	}

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer)
{
	cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == pDhcpServer), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	osal_memcpy(pDhcpServer, dhcp_server, sizeof(cfg_dhcp_server_t));

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_sync_clean(void)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	dhcp_server->dhcpConfigSync = 0;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_enable_set(sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->enable = enable;
	dhcp_server->dhcpConfigSync |= DHCP_GLOBAL_CHANGE_FLAG;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_enable_get(sys_enable_t *pEnable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pEnable = dhcp_server->enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_group_ip_set(uint32 group, sys_ipv4_t ipv4)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    if((group < 1) || (group > DHCP_GROUP_MAX_NUM))
    {
		return SYS_ERR_INPUT;
    }

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    
	dhcp_server->dhcpGroup[group - 1].serverIp = ipv4;
	dhcp_server->dhcpConfigSync |= DHCP_GROUP_CHANGE_FLAG;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_group_use_stat_set(uint32 group, uint32 vid)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    if((group < 1) || (group > DHCP_GROUP_MAX_NUM))
    {
		return SYS_ERR_INPUT;
    }

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
	dhcp_server->dhcpGroup[group - 1].intf = vid;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_group_ip_get(uint32 group, sys_ipv4_t* ipv4, uint32* pIntf)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == ipv4), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIntf), SYS_ERR_NULL_POINTER);

    if((group < 1) || (group > DHCP_GROUP_MAX_NUM))
    {
		return SYS_ERR_INPUT;
    }

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    memcpy(ipv4, &dhcp_server->dhcpGroup[group - 1].serverIp, sizeof(sys_ipv4_t));
    *pIntf =  dhcp_server->dhcpGroup[group - 1].intf;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_intf_group_ip_add(uint32 group, uint32 vid)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
    int i, j;

    if(group < 1 || group > DHCP_GROUP_MAX_NUM)
    {
		return SYS_ERR_INPUT;
    }

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    
    for(i = 0; i < L3INTFNUM && (dhcp_server->intfDhcp[i].vid == vid); i++)
    {
    #if 1
    	
    	for(j = 0; j < DHCP_SERVER_GROUP_MAX_NUM; j++)
    	{
			if(dhcp_server->intfDhcp[i].groupNo[j] == group)
				CFG_CLOSE(CFG_ID_DHCP_SERVER);
    	}
		
    	for(j = 0; j < DHCP_SERVER_GROUP_MAX_NUM; j++)
    	{
			if(dhcp_server->intfDhcp[i].groupNo[j] == 0)
			{
				dhcp_server->intfDhcp[i].groupNo[j] = group;
				dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
				CFG_CLOSE(CFG_ID_DHCP_SERVER);
			}
    	}
    	
		if(j == DHCP_SERVER_GROUP_MAX_NUM)
		{
			CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_FAILED);
		}
	#else
		dhcp_server->intfDhcp[i].dhcp.group = group;
		dhcp_server->intfDhcp[i].dhcp.flag = TRUE;
		dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
		
		break;
	#endif
	}

	for(i = 0; i < L3INTFNUM && (dhcp_server->intfDhcp[i].vid == 0); i++)
	{
	#if 1
		
		for(j = 0; j < DHCP_SERVER_GROUP_MAX_NUM; j++)
		{
			if(dhcp_server->intfDhcp[i].groupNo[j] == 0)
			{
				dhcp_server->intfDhcp[i].vid = vid;
				dhcp_server->intfDhcp[i].groupNo[j] = group;
				dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
				CFG_CLOSE(CFG_ID_DHCP_SERVER);
			}
		}
		
		if(j == DHCP_SERVER_GROUP_MAX_NUM)
		{
			CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_FAILED);
		}
	#else

		dhcp_server->intfDhcp[i].dhcp.group = group;
		dhcp_server->intfDhcp[i].dhcp.flag = TRUE;
		dhcp_server->intfDhcp[i].vid = vid;
		dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
		
		break;
	#endif
	}

	if (i == L3INTFNUM)
	{
		CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_FAILED);
	}

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_intf_group_ip_del(uint32 group, uint32 vid)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
    int i, j;

    if(group < 1 || group > (DHCP_GROUP_MAX_NUM))
    {
		return SYS_ERR_INPUT;
    }

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    
    for(i = 0; i < L3INTFNUM && (dhcp_server->intfDhcp[i].vid == vid); i++)
    {
#if 1
    	
    	for(j = 0; j < DHCP_SERVER_GROUP_MAX_NUM; j++)
    	{
			if(dhcp_server->intfDhcp[i].groupNo[j] == group)
			{
				dhcp_server->intfDhcp[i].groupNo[j] = 0;
				dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
			}
    	}

    	for(j = 0; j < DHCP_SERVER_GROUP_MAX_NUM; j++)
    	{
			if(dhcp_server->intfDhcp[i].groupNo[j] == 0)
			{
				break;
			}
    	}
    	
		if(j == DHCP_SERVER_GROUP_MAX_NUM)
		{
			dhcp_server->intfDhcp[i].vid = 0;
			CFG_CLOSE(CFG_ID_DHCP_SERVER);
		}
#else
		dhcp_server->intfDhcp[i].dhcp.group = 0;
		dhcp_server->intfDhcp[i].dhcp.flag = FALSE;
		dhcp_server->intfDhcp[i].vid = 0;
		dhcp_server->dhcpConfigSync |= DHCP_INTF_CHANGE_FLAG;
		break;
#endif
	}

	if (i == L3INTFNUM)
	{
		CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_FAILED);
	}

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_intf_group_ip_get(uint32 intf, sys_intf_dhcp_t* data)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	SYS_PARAM_CHK((NULL == data), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
	osal_memcpy(data, &dhcp_server->intfDhcp[intf], sizeof(sys_intf_dhcp_t));
	
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_intf_group_ip_init(uint32 intf)
{
#if 0
    cfg_dhcp_server_t *dhcp_server = NULL;
	if(intf < 0 || intf > (L3INTFNUM-1))
	{
		return SYS_ERR_INPUT;
	}

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
	dhcp_server->intfDhcp[intf].flag = 0;
	memset(&dhcp_server->intfDhcp[intf].groupId, 0, (sizeof(uint32)*DHCP_SERVER_GROUP_MAX_NUM));
	dhcp_server->intfDhcp[intf].vid = 0;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
#endif
	return SYS_ERR_OK;
}

int32 cfg_dhcp_relay_option82_enable_set(sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->option82Enable = enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_enable_get(sys_enable_t *pEnable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pEnable = dhcp_server->option82Enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_device_enable_set(sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->option82DevEnabled = enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_device_enable_get(sys_enable_t *pEnable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pEnable = dhcp_server->option82DevEnabled;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_format_set(uint32 format)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->option82Format = format;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_format_get(uint32 *format)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == format), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *format = dhcp_server->option82Format;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_info_format_set(uint32 iFormat)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->opion82InfoFormat = iFormat;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_info_format_get(uint32 *iFormat)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == iFormat), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *iFormat = dhcp_server->opion82InfoFormat;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_format_verbose_set(char *option82VerboseNodeId)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == option82VerboseNodeId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(dhcp_server->option82VerboseNodeId, option82VerboseNodeId, OPTION82_VERBOS_NODEID_LEN + 1);

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_format_verbose_get(char *option82VerboseNodeId)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == option82VerboseNodeId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(option82VerboseNodeId, dhcp_server->option82VerboseNodeId, OPTION82_VERBOS_NODEID_LEN + 1);

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->portOption82[port].option82Enabled = enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pEnable = dhcp_server->portOption82[port].option82Enabled;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_strategy_set(sys_logic_port_t port, uint32 strategy)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->portOption82[port].opion82Strategy = strategy;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_strategy_get(sys_logic_port_t port, sys_enable_t *pStrategy)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStrategy), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pStrategy = dhcp_server->portOption82[port].opion82Strategy;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_circuit_id_set(sys_logic_port_t port, char *option82CircuitId)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == option82CircuitId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(dhcp_server->portOption82[port].option82CircuitId, option82CircuitId, OPTION82_CIRCUITID_LEN + 1);

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_circuit_id_get(sys_logic_port_t port, char *option82CircuitId)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == option82CircuitId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(option82CircuitId, dhcp_server->portOption82[port].option82CircuitId, OPTION82_CIRCUITID_LEN + 1);

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_remote_id_set(sys_logic_port_t port, char *option82RemoteId)
{
	cfg_dhcp_server_t *dhcp_server = NULL;

	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK((NULL == option82RemoteId), SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	osal_memcpy(dhcp_server->portOption82[port].option82RemoteId, option82RemoteId, OPTION82_REMOTEID_LEN + 1);

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_option82_port_remote_id_get(sys_logic_port_t port, char *option82RemoteId)
{
	cfg_dhcp_server_t *dhcp_server = NULL;

	SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
	SYS_PARAM_CHK((NULL == option82RemoteId), SYS_ERR_NULL_POINTER);

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	osal_memcpy(option82RemoteId, dhcp_server->portOption82[port].option82RemoteId, OPTION82_REMOTEID_LEN + 1);

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_lookup(char *name, int32 *id)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int i = 0;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	for(i=0; i<IP_POOL_MAX_NUM; i++)
	{
		if (!osal_strcmp(dhcp_server->gIpPool[i].szName, name))
		{
			*id = i;
			CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_ENTRY_IS_EXIST);
		}
	}

    CFG_CLOSE(CFG_ID_DHCP_SERVER);

	return SYS_ERR_OK;
}

int32 cfg_ip_pool_new(int32 *id)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
    char str[IP_POOL_MAX_NAME];
	int i = 0;

	osal_memset(&str,0, IP_POOL_MAX_NAME);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	for(i=0; i<IP_POOL_MAX_NUM; i++)
	{
		if (!memcmp(dhcp_server->gIpPool[i].szName, str, IP_POOL_MAX_NAME))
		{
			*id = i;
			CFG_CLOSE(CFG_ID_DHCP_SERVER);
		}
	}

	CFG_ERR_CLOSE(CFG_ID_DHCP_SERVER, SYS_ERR_ENTRY_FULL);
}

int32 cfg_ip_pool_create(char *name)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret == SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_OK;

	ret = cfg_ip_pool_new(&id);
	if (ret == SYS_ERR_ENTRY_FULL)
		return SYS_ERR_ENTRY_FULL;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    memcpy(dhcp_server->gIpPool[id].szName, name, IP_POOL_MAX_NAME);
    dhcp_server->gIpPool[id].ulSubnetMask = IPPOOL_DEFAULT_SUBNET_MASK;
    dhcp_server->gIpPool[id].ulLeaseTime = IPPOOL_DEFAULT_LEASETIME;
	dhcp_server->dhcpConfigSync |= DHCP_IPPOOL_CHANGE_FLAG;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_del(char *name)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    osal_memset(&dhcp_server->gIpPool[id], 0, sizeof(ippool_t));
	dhcp_server->dhcpConfigSync |= DHCP_IPPOOL_CHANGE_FLAG;
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_get(char *name, ippool_t *pPool)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
    osal_memcpy(pPool, &dhcp_server->gIpPool[id], sizeof(ippool_t));
    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_gateway_set(char *name, sys_ipv4_t addr, sys_ipv4_t mask)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
	dhcp_server->gIpPool[id].ulGatewayIp = addr;
	dhcp_server->gIpPool[id].ulSubnetMask = mask;
	dhcp_server->dhcpConfigSync |= DHCP_IPPOOL_CHANGE_FLAG;
	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_section_set(char *name, uint32 section, sys_ipv4_t start_ip, sys_ipv4_t end_ip)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);
	dhcp_server->gIpPool[id].IpSection[section].ulStartIp = start_ip;
	dhcp_server->gIpPool[id].IpSection[section].ulEndIp = end_ip;
	dhcp_server->dhcpConfigSync |= DHCP_IPPOOL_CHANGE_FLAG;
	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_port_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
	cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	if (dhcp_server->portRelayEnable[port] != enable)
	{
		dhcp_server->portRelayEnable[port] = enable;
		dhcp_server->dhcpConfigSync |= DHCP_PORT_CHANGE_FLAG;
	}

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_relay_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *pEnable = dhcp_server->portRelayEnable[port];

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_IpBind_Switch_enable_set(sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->ipbindEnable = enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_client_unknown_client_assign(sys_enable_t enable)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    dhcp_server->ipUnbindAssignEnable = enable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_IpBind_get(sys_enable_t* enable, sys_enable_t* unBoundAssign)
{
    cfg_dhcp_server_t *dhcp_server = NULL;

    SYS_PARAM_CHK((NULL == enable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == unBoundAssign), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    *enable = dhcp_server->ipbindEnable;
    *unBoundAssign = dhcp_server->ipUnbindAssignEnable;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_dns_set(char *name, uint32 dnsNum, sys_ipv4_t ip)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	if ((dnsNum < IP_POOL_PRIMARY_DNS_NUM) || (dnsNum > IP_POOL_MAX_DNS_NUM))
		return SYS_ERR_FAILED;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	dhcp_server->gIpPool[id].ulDNSIp[dnsNum - 1] = ip;

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_dns_get(char *name, uint32 dnsNum, sys_ipv4_t *pIp)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

	if ((dnsNum < IP_POOL_PRIMARY_DNS_NUM) || (dnsNum > IP_POOL_MAX_DNS_NUM))
		return SYS_ERR_FAILED;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	*pIp = dhcp_server->gIpPool[id].ulDNSIp[dnsNum - 1];

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_dns_suffix_set(char *name, char *dnsSuffixName)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == dnsSuffixName), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(dhcp_server->gIpPool[id].szDnsSuffixName, dnsSuffixName, IP_POOL_MAX_DNS_SUFFIX_NAME);

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_dns_suffix_get(char *name, char *dnsSuffixName)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == dnsSuffixName), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    osal_memcpy(dnsSuffixName, dhcp_server->gIpPool[id].szDnsSuffixName, IP_POOL_MAX_DNS_SUFFIX_NAME);

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_wins_set(char *name, uint32 winsNum, sys_ipv4_t ip)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	if ((winsNum < IP_POOL_PRIMARY_WINS_NUM) || (winsNum > IP_POOL_SECOND_WINS_NUM))
		return SYS_ERR_FAILED;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	if (winsNum == IP_POOL_PRIMARY_WINS_NUM)
		dhcp_server->gIpPool[id].ulPrimaryNBNSIp = ip;
	else
		dhcp_server->gIpPool[id].ulSecondNBNSIp = ip;

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_wins_get(char *name, uint32 winsNum, sys_ipv4_t *pIp)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIp), SYS_ERR_NULL_POINTER);

	if ((winsNum < IP_POOL_PRIMARY_WINS_NUM) || (winsNum > IP_POOL_SECOND_WINS_NUM))
		return SYS_ERR_FAILED;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	if (winsNum == IP_POOL_PRIMARY_WINS_NUM)
		*pIp = dhcp_server->gIpPool[id].ulPrimaryNBNSIp;
	else
		*pIp = dhcp_server->gIpPool[id].ulSecondNBNSIp;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_lease_time_set(char *name, uint32 lease_time)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

		dhcp_server->gIpPool[id].ulLeaseTime = lease_time;

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_lease_time_get(char *name, uint32 *pLease_time)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pLease_time), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	*pLease_time = dhcp_server->gIpPool[id].ulLeaseTime;

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_option43_set(char *name, sys_ippool_option43_t *option43)
{
	cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	osal_memcpy(&dhcp_server->gIpPool[id].option43, option43, sizeof(sys_ippool_option43_t));

	CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_ip_pool_option43_get(char *name, sys_ippool_option43_t *option43)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
	int32 id = -1;
	int32 ret;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

	ret = cfg_ip_pool_lookup(name, &id);
	if (ret != SYS_ERR_ENTRY_IS_EXIST)
		return SYS_ERR_FAILED;

	if ((id <0) || (id >= IP_POOL_MAX_NUM))
		return SYS_ERR_FAILED;

    CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

	osal_memcpy(option43, &dhcp_server->gIpPool[id].option43, sizeof(sys_ippool_option43_t));

    CFG_CLOSE(CFG_ID_DHCP_SERVER);
}

int32 cfg_dhcp_server_init(void)
{
    cfg_dhcp_server_t *dhcp_server = NULL;
    sys_logic_port_t lp;

	CFG_OPEN(CFG_ID_DHCP_SERVER, (cfg_addr)&dhcp_server);

    FOR_EACH_LOGIC_PORT(lp)
	{
		dhcp_server->portOption82[lp].option82Enabled = ENABLED;
	}

	CFG_CLOSE(CFG_ID_DHCP_SERVER);

	return SYS_ERR_OK;
}

