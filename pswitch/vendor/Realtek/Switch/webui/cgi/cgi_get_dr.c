
#include <include/cgi.h>

static int32 cgi_dr_prop_get(void);
static int32 cgi_dr_portEdit_get(void);
static int32 cgi_ip_pool_get(void);
static int32 cgi_ip_pool_edit_get(void);
static int32 cgi_ip_pool_add_get(void);
static int32 cgi_vlanif_pool_get(void);
static int32 cgi_dr_group_edit_get(void);
static int32 cgi_dr_clients_get(void);
static int32 cgi_dr_client_static_bind_get(void);
static int32 cgi_dr_group_add_get(void);

extern int32 sal_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer);
extern int32 sal_ip_pool_get_by_name(char *name, ippool_t *pPool);

const cgi_cmdmap_t cmdGetDrTbl[] =
{
    {CGI_CMD_DR_PROP,               cgi_dr_prop_get},
    {CGI_CMD_DR_PORTEDIT,           cgi_dr_portEdit_get},
    {CGI_CMD_DR_IPPOOL,	            cgi_ip_pool_get},
    {CGI_CMD_DR_IPPOOLEDIT,	        cgi_ip_pool_edit_get},
    {CGI_CMD_DR_IPPOOLADD,          cgi_ip_pool_add_get},
    {CGI_CMD_DR_VLANIFIPPOOL,       cgi_vlanif_pool_get},
    {CGI_CMD_DR_GROUP_EDIT,         cgi_dr_group_edit_get},
    {CGI_CMD_DR_CLIENTS,            cgi_dr_clients_get},
    {CGI_CMD_DR_STATIC_BIND,        cgi_dr_client_static_bind_get},
    {CGI_CMD_DR_GROUP_ADD,          cgi_dr_group_add_get},
    {NULL, NULL}
};

int32 cgi_second_to_day_hour_min(uint32 second, uint32* pDay, uint32* pHour, uint32* pMinute)
{
	uint32 tmp = 0;
	*pDay = second / (60 * 60 * 24);
	tmp = second % (60 * 60 * 24);
	*pHour = tmp / (60 * 60);
	tmp = tmp % (60 * 60);
	*pMinute = tmp / 60;
	
	return SYS_ERR_OK;
}
 
static int32 cgi_dr_prop_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    sys_enable_t enable = DISABLED;
    sys_enable_t bindenable = DISABLED;
    sys_enable_t unBoundAssignEnable = DISABLED;
    sys_logic_port_t lp = 0;

    sal_dhcp_server_enable_get(&enable);
    sal_dhcp_server_IpBind_get(&bindenable, &unBoundAssignEnable);

    CGI_GET_JSON_CREATE(root, data);
    ports = cJSON_CreateArray();

    cJSON_AddBoolToObject(data, CGI_STATE, enable);
    cJSON_AddBoolToObject(data, CGI_DR_STATIC_BIND_ENABLE, bindenable);
    cJSON_AddItemToObject(data, CGI_PORTS, ports);

	FOR_EACH_LOGIC_PORT(lp)
	{
		sal_dhcp_relay_port_enable_get(lp, &enable);

	    port = cJSON_CreateObject();
	    cJSON_AddItemToArray(ports, port);

	    cJSON_AddBoolToObject(port, CGI_DR_PORT_ENABLE, enable);
	}

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dr_portEdit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
	sys_logic_port_t lp = 0;
	sys_enable_t enable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
        lp = SYS_STR2UINT(value);

	sal_dhcp_relay_port_enable_get(lp, &enable);

	CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_DR_PORT_ENABLE, enable);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ip_pool_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *ippools = NULL;
    cJSON *ippool = NULL;
    uint32 i = 0;
    uint32 j = 0;
    uint32 cnt = 0;
    char ipStr[CAPA_IPV4_STR_LEN];
    char ndsPriipStr[CAPA_IPV4_STR_LEN];
    char ndsSecipStr[CAPA_IPV4_STR_LEN];
    char leaseTimeStr[24];
	cfg_dhcp_server_t dhcpServer;
    char str[IP_POOL_MAX_NAME];
	char szValue[IP_POOL_OPTION43_MAX_LEN + 1];
    uint32 days = 0, hour = 0, mins = 0;

	osal_memset(&str,0, IP_POOL_MAX_NAME);

	sal_dhcp_server_get(&dhcpServer);

    CGI_GET_JSON_CREATE(root, data);

    ippools = cJSON_CreateArray();
    for (i = 0; i < IP_POOL_MAX_NUM; i++)
    {
		if (memcmp(dhcpServer.gIpPool[i].szName, str, IP_POOL_MAX_NAME))
	    {
			
			{
				//if (dhcpServer.gIpPool[i].IpSection[j].ulStartIp
					//&& dhcpServer.gIpPool[i].IpSection[j].ulEndIp)
				{
					ippool = cJSON_CreateObject();
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_NAME, dhcpServer.gIpPool[i].szName);

					cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEN, 32);

			        SYS_MEM_CLEAR(ipStr);
					sprintf(ipStr, "%d", j+1);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_SECTION, ipStr);

					cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_SECTION_MIN, 0);
					cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_SECTION_MAX, 7);
					cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_SECTION_DFLT, 0);

			        SYS_MEM_CLEAR(ipStr);
			        sys_util_ip2Str(dhcpServer.gIpPool[i].IpSection[j].ulStartIp, ipStr);
			        cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_START_ADDR, ipStr);

			        SYS_MEM_CLEAR(ipStr);
			        sys_util_ip2Str(dhcpServer.gIpPool[i].IpSection[j].ulEndIp, ipStr);
			        cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_END_ADDR, ipStr);

					SYS_MEM_CLEAR(ipStr);
					sys_util_ip2Str(dhcpServer.gIpPool[i].ulGatewayIp, ipStr);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_GATEWAY, ipStr);

					SYS_MEM_CLEAR(ipStr);
					sys_util_ip2Str(dhcpServer.gIpPool[i].ulSubnetMask, ipStr);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_MASK, ipStr);

					SYS_MEM_CLEAR(ndsPriipStr);
					sys_util_ip2Str(dhcpServer.gIpPool[i].ulDNSIp[0], ndsPriipStr);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_DNSPRIMARY, ndsPriipStr);
					if(dhcpServer.gIpPool[i].ulDNSIp[0] != 0)
					{
						cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSPRIMARYSTAT, ENABLED);
					}
					else
					{
						cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSPRIMARYSTAT, Disabled);
					}

					SYS_MEM_CLEAR(ndsSecipStr);
					sys_util_ip2Str(dhcpServer.gIpPool[i].ulDNSIp[1], ndsSecipStr);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_DNSSECOND, ndsSecipStr);
					if(dhcpServer.gIpPool[i].ulDNSIp[1] != 0)
					{
						cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSSECONDSTAT, ENABLED);
					}
					else
					{
						cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSSECONDSTAT, Disabled);
					}

					SYS_MEM_CLEAR(szValue);
					if ( IP_POOL_OPTION_FORMAT_HEX == dhcpServer.gIpPool[i].option43.ucFormat )
					{
						cJSON_AddLangStringToObject(ippool, CGI_DR_IPPOOL_OPTION43_FORMAT, "dr", "txtHex");
						sys_util_byteArray2hexStr(dhcpServer.gIpPool[i].option43.szValue, dhcpServer.gIpPool[i].option43.ucValueLen, szValue);
					}
					else
					{
						cJSON_AddLangStringToObject(ippool, CGI_DR_IPPOOL_OPTION43_FORMAT, "dr", "txtAscii");
						osal_memcpy(szValue, dhcpServer.gIpPool[i].option43.szValue, dhcpServer.gIpPool[i].option43.ucValueLen);
					}
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_OPTION43, szValue);

					SYS_MEM_CLEAR(leaseTimeStr);
					cgi_second_to_day_hour_min(dhcpServer.gIpPool[i].ulLeaseTime, &days, &hour, &mins);
					osal_sprintf(leaseTimeStr,"%3u:%2u:%2u", days, hour, mins);
					cJSON_AddStringToObject(ippool, CGI_DR_IPPOOL_LEASE, leaseTimeStr);

					cJSON_AddItemToArray(ippools, ippool);
					cnt++;
				}
			}
	    }
	}

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_DR_IPPOOL, ippools);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_ip_pool_edit_get(void)
{
	cJSON *root = NULL;
	cJSON *data = NULL;
	char *value = NULL;
	char ipStr[CAPA_IPV4_STR_LEN];
	char ndsPriipStr[CAPA_IPV4_STR_LEN];
	char ndsSecipStr[CAPA_IPV4_STR_LEN];
	char str[IP_POOL_MAX_NAME];
	char szValue[IP_POOL_OPTION43_MAX_LEN + 1];
	ippool_t poll;
	int i;
	uint32 days = 0, hours = 0, minutes = 0;

	SYS_MEM_CLEAR(poll);
	SYS_MEM_CLEAR(str);
	
	if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
		osal_sprintf(str, "%s", value);

	sal_ip_pool_get_by_name(str, &poll);

	CGI_GET_JSON_CREATE(root, data);

	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_NAME, str);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEN, 32);

	for(i = 0; i < IP_POOL_MAX_IPSECTION_NUM; i++)
	{
		if(poll.IpSection[i].ulStartIp)
		{
			
			SYS_MEM_CLEAR(ipStr);
			sprintf(ipStr, "%d", i+1);
			cJSON_AddStringToObject(data, CGI_DR_IPPOOL_SECTION, ipStr);

			SYS_MEM_CLEAR(ipStr);
			sys_util_ip2Str(poll.IpSection[i].ulStartIp, ipStr);
			cJSON_AddStringToObject(data, CGI_DR_IPPOOL_START_ADDR, ipStr);

			SYS_MEM_CLEAR(ipStr);
			sys_util_ip2Str(poll.IpSection[i].ulEndIp, ipStr);
			cJSON_AddStringToObject(data, CGI_DR_IPPOOL_END_ADDR, ipStr);
		}
	}

	SYS_MEM_CLEAR(ipStr);
	sys_util_ip2Str(poll.ulGatewayIp, ipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_GATEWAY, ipStr);

	SYS_MEM_CLEAR(ipStr);
	sys_util_ip2Str(poll.ulSubnetMask, ipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_MASK, ipStr);

	SYS_MEM_CLEAR(ndsPriipStr);
	if(poll.ulDNSIp[0])
	{
		sys_util_ip2Str(poll.ulDNSIp[0], ndsPriipStr);
		cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSPRIMARYSTAT, ENABLED);
		cJSON_AddStringToObject(data, CGI_DR_IPPOOL_DNSPRIMARY, ndsPriipStr);
	}
	else 
	{
		cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSPRIMARYSTAT, Disabled);
	}

	if(poll.ulDNSIp[1])
	{
		SYS_MEM_CLEAR(ndsSecipStr);
		sys_util_ip2Str(poll.ulDNSIp[1], ndsSecipStr);
		cJSON_AddStringToObject(data, CGI_DR_IPPOOL_DNSSECOND, ndsSecipStr);
		cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSSECONDSTAT, ENABLED);
	}
	else
	{
		cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSSECONDSTAT, Disabled);
	}
	
	SYS_MEM_CLEAR(szValue);
	if ( IP_POOL_OPTION_FORMAT_HEX == poll.option43.ucFormat )
	{
		cJSON_AddStringToObject(data, CGI_DR_IPPOOL_OPTION43_FORMAT, "hex");
		sys_util_byteArray2hexStr(poll.option43.szValue, poll.option43.ucValueLen, szValue);
	}
	else
	{
		cJSON_AddStringToObject(data, CGI_DR_IPPOOL_OPTION43_FORMAT, "ascii");
		osal_memcpy(szValue, poll.option43.szValue, poll.option43.ucValueLen);
	}
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_OPTION43, szValue);

	cgi_second_to_day_hour_min(poll.ulLeaseTime, &days, &hours, &minutes);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEDAYS, days);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEHOURS, hours);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEMINUTES, minutes);

	CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

	return SYS_ERR_OK;
}

static int32 cgi_ip_pool_add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char ndsPriipStr[CAPA_IPV4_STR_LEN];
    char ndsSecipStr[CAPA_IPV4_STR_LEN];
    char str[IP_POOL_MAX_NAME];

	osal_memset(&str,0, IP_POOL_MAX_NAME);

    CGI_GET_JSON_CREATE(root, data);

	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEN, 32);

    SYS_MEM_CLEAR(ipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_SECTION, ipStr);

    SYS_MEM_CLEAR(ipStr);
    cJSON_AddStringToObject(data, CGI_DR_IPPOOL_START_ADDR, ipStr);

    SYS_MEM_CLEAR(ipStr);
    cJSON_AddStringToObject(data, CGI_DR_IPPOOL_END_ADDR, ipStr);

	SYS_MEM_CLEAR(ipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_GATEWAY, ipStr);

	SYS_MEM_CLEAR(ipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_MASK, ipStr);

	SYS_MEM_CLEAR(ndsPriipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_DNSPRIMARY, ndsPriipStr);
	cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSPRIMARYSTAT, Disabled);

	SYS_MEM_CLEAR(ndsSecipStr);
	cJSON_AddStringToObject(data, CGI_DR_IPPOOL_DNSSECOND, ndsSecipStr);
	cJSON_AddBoolToObject(data, CGI_DR_IPPOOL_DNSSECONDSTAT, Disabled);

	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEDAYS, 1);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEHOURS, 0);
	cJSON_AddNumberToObject(data, CGI_DR_IPPOOL_LEASEMINUTES, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlanif_pool_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    cJSON *groups = NULL;
    cJSON *groupid = NULL;
    cJSON *grouplist = NULL;
    cJSON *groupindex = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    sys_l3_ipv4Db_t *pCurr = NULL;
    int i, group;
    sys_ipv4_t ipv4;
	uint32 cnt = 0;
	uint32 intfVlanId = 0;
	#ifndef CONFIG_SYS_L3_ARCHITECTURE
    sys_l3_intfId_t intfId;
    #endif

	CGI_GET_JSON_CREATE(root, data);

	intfs = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_DRINTF, intfs);
	intf = cJSON_CreateObject();
	#ifndef CONFIG_SYS_L3_ARCHITECTURE
	
	STR2INTFID(L3_INTF_MGMTVLAN, &intfId);
	SYS_MEM_CLEAR(intfName);
	INTFID2NAME(&intfId, intfName);
	cJSON_AddNumberToObject(intf, CGI_VALUE, intfId.id);
	cJSON_AddStringToObject(intf, CGI_NAME, intfName); 
	cJSON_AddItemToArray(intfs, intf);
	#endif
    FOR_EACH_L3_IPV4(i, pCurr)
    {
        
        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&pCurr->intfId, intfName);
        intf = cJSON_CreateObject();
        cJSON_AddNumberToObject(intf, CGI_VALUE, pCurr->intfId.id);
        cJSON_AddStringToObject(intf, CGI_NAME, intfName); 
        cJSON_AddItemToArray(intfs, intf);
    }
     
	groups = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_DRGROUP, groups);
	for(i = 1; i <= 256; i++)
	{
		osal_memset(&ipv4, 0, sizeof(sys_ipv4_t));
    	SYS_ERR_CHK(sal_dhcp_server_group_ip_get(i, &ipv4, &intfVlanId));
    	if(ipv4 == 0)
    	{
			continue;
    	}
    	groupid = cJSON_CreateObject();
		cJSON_AddItemToArray(groups, groupid);
		cJSON_AddNumberToObject(groupid, CGI_VALUE, i);
		osal_sprintf(intfName, "%d", i);
		cJSON_AddStringToObject(groupid, CGI_NAME, intfName);
	}

	grouplist = cJSON_CreateArray();
	for(group = 1; group <= 256; group++)
    {
    	groupindex = cJSON_CreateObject();
    	osal_memset(&ipv4, 0, sizeof(sys_ipv4_t));
    	SYS_ERR_CHK(sal_dhcp_server_group_ip_get(group, &ipv4, &intfVlanId));
    	if(ipv4 == 0)
    	{
			continue;
    	}
    	cJSON_AddNumberToObject(groupindex, CGI_DR_GROUP_ID, group);
    	SYS_MEM_CLEAR(ipStr);
    	sys_util_ip2Str(ipv4, ipStr);
    	cJSON_AddStringToObject(groupindex, CGI_DR_GROUP_IP, ipStr);
    	if(intfVlanId)
    	{
    		SYS_MEM_CLEAR(intfName);
    		osal_sprintf(intfName, "vlan %d", intfVlanId);
    		cJSON_AddStringToObject(groupindex, CGI_DR_GROUP_INTF, intfName);
		}
		else
		{
			cJSON_AddStringToObject(groupindex, CGI_DR_GROUP_INTF, "----");
		}
		cJSON_AddItemToArray(grouplist, groupindex);
		cnt++;
    }
    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_DRGROUPLIST, grouplist);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dr_group_edit_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *intfs = NULL;
    cJSON *intf = NULL;
    char *value = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    char ipStr[CAPA_IPV4_STR_LEN];
    sys_l3_ipv4Db_t *pCurr = NULL;
    int i, group = 0;
    sys_ipv4_t ipv4;
	uint32 intfVlanId = 0;
#ifndef CONFIG_SYS_L3_ARCHITECTURE
	sys_l3_intfId_t intfId;
#endif
    if (CGI_IS_PARAM_EXIST(CGI_SEL_SRC, value))
    {
		group = SYS_STR2UINT(value);
    }

	CGI_GET_JSON_CREATE(root, data);

	intfs = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_DRINTF, intfs);
	intf = cJSON_CreateObject();
#ifndef CONFIG_SYS_L3_ARCHITECTURE
	
	STR2INTFID(L3_INTF_MGMTVLAN, &intfId);
	SYS_MEM_CLEAR(intfName);
	INTFID2NAME(&intfId, intfName);
	cJSON_AddNumberToObject(intf, CGI_VALUE, intfId.id);
	cJSON_AddStringToObject(intf, CGI_NAME, intfName); 
	cJSON_AddItemToArray(intfs, intf);
#endif
    FOR_EACH_L3_IPV4(i, pCurr)
    {
        
        SYS_MEM_CLEAR(intfName);
        INTFID2NAME(&pCurr->intfId, intfName);
        intf = cJSON_CreateObject();
        cJSON_AddNumberToObject(intf, CGI_VALUE, pCurr->intfId.id);
        cJSON_AddStringToObject(intf, CGI_NAME, intfName);  
        cJSON_AddItemToArray(intfs, intf);
    }

    osal_memset(&ipv4, 0, sizeof(sys_ipv4_t));
    SYS_ERR_CHK(sal_dhcp_server_group_ip_get(group, &ipv4, &intfVlanId));
    cJSON_AddNumberToObject(data, CGI_DR_GROUP_ID, group);
    SYS_MEM_CLEAR(ipStr);
    sys_util_ip2Str(ipv4, ipStr);
    cJSON_AddStringToObject(data, CGI_DR_GROUP_IP, ipStr);
    SYS_MEM_CLEAR(intfName);
    osal_sprintf(intfName, "vlan %d", intfVlanId);
    cJSON_AddStringToObject(data, CGI_DR_GROUP_INTF, intfName);
    if(intfVlanId)
    {
		cJSON_AddBoolToObject(data, CGI_DR_STATUS, ENABLED);
    }
    else
    {
		cJSON_AddBoolToObject(data, CGI_DR_STATUS, DISABLED);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dr_group_add_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *groups = NULL;
    cJSON *groupid = NULL;
    char intfName[CAPA_L3_INTFNAME_LEN];
    int i;

	CGI_GET_JSON_CREATE(root, data);
    
	groups = cJSON_CreateArray();
	cJSON_AddItemToObject(data, CGI_DRGROUP, groups);
	for(i = 1; i <= 256; i++)
	{
    	groupid = cJSON_CreateObject();
		cJSON_AddItemToArray(groups, groupid);
		cJSON_AddNumberToObject(groupid, CGI_VALUE, i);
		osal_sprintf(intfName, "%d", i);
		cJSON_AddStringToObject(groupid, CGI_NAME, intfName);
	}

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dr_clients_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *clientList = NULL;
    cJSON *cilent = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char ipStr[CAPA_IPV4_STR_LEN];
    char str[IP_POOL_MAX_NAME];
    ushort_t usStart, usNextStart, usNum;
    MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX];
    MACHASH_S *pHashData;
	char macstr[CAPA_MAC_STR_LEN];

	osal_memset(&str,0, IP_POOL_MAX_NAME);
	
	SYS_MEM_CLEAR(hostBuf);

    CGI_GET_JSON_CREATE(root, data);

    clientList = cJSON_CreateArray();

    usNextStart = 1;
    while(1)
    {
    	usStart = usNextStart;
        usNum = 0;
        if ( SYS_ERR_OK != sal_dhcp_client_hash_get(usStart, &usNextStart, &usNum, hostBuf) )
        {
            break;
        }

        for ( i = 0; i < usNum; i++ )
        {
        	cilent = cJSON_CreateObject();
            pHashData = &hostBuf[i];

            SYS_MEM_CLEAR(macstr);
			SYS_ERR_CHK(sys_util_mac2Str(pHashData->ucUserMac, macstr));
			cJSON_AddStringToObject(cilent, CGI_MAC_ADDR, macstr);
            
            cJSON_AddNumberToObject(cilent, CGI_VLAN, pHashData->usVlanId);
            
			SYS_MEM_CLEAR(ipStr);
			sys_util_ip2Str(pHashData->ulIpAddr, ipStr);
			cJSON_AddStringToObject(cilent, CGI_DR_CLIENT_IP, ipStr);
            
            cJSON_AddStringToObject(cilent, CGI_DR_CLIENT_HOSTNAME, pHashData->hostname);
            cJSON_AddItemToArray(clientList, cilent);
            cnt++;
        }

        if ( (0 == usNum) || (0 == usNextStart) )
        {
            break;
        }	    
	}

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_DR_CLIENT_LIST, clientList);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_dr_client_static_bind_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *clientList = NULL;
    cJSON *cilent = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    char ipStr[CAPA_IPV4_STR_LEN];
    ushort_t usStart, usNextStart, usNum;;
    dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN], *pNode;
	char macstr[CAPA_MAC_STR_LEN];
	
	SYS_MEM_CLEAR(nodeBuf);

    CGI_GET_JSON_CREATE(root, data);

    clientList = cJSON_CreateArray();

    usNextStart = 1;
    while(1)
    {
    	usStart = usNextStart;
        usNum = 0;
        if ( SYS_ERR_OK != sal_dhcp_client_host_mac_ip_get(usStart, &usNextStart, &usNum, nodeBuf) )
        {
            break;
        }

        for ( i = 0; i < usNum; i++ )
        {
        	cilent = cJSON_CreateObject();
            pNode = &nodeBuf[i];

            SYS_MEM_CLEAR(macstr);
			SYS_ERR_CHK(sys_util_mac2Str(pNode->mac, macstr));
			cJSON_AddStringToObject(cilent, CGI_MAC_ADDR, macstr);
            
            cJSON_AddNumberToObject(cilent, CGI_VLAN, pNode->usVlanID);
            
			SYS_MEM_CLEAR(ipStr);
			sys_util_ip2Str(pNode->ulIpAddress, ipStr);
			cJSON_AddStringToObject(cilent, CGI_DR_CLIENT_IP, ipStr);
            
            cJSON_AddStringToObject(cilent, CGI_DR_CLIENT_USERNAME, pNode->szUserName);
            cJSON_AddItemToArray(clientList, cilent);
            cnt++;
        }

        if ( (0 == usNum) || (0 == usNextStart) )
        {
            break;
        }	    
	}

    if (cnt > 0)
        cJSON_AddItemToObject(data, CGI_DR_CLIENT_LIST, clientList);
    else
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

