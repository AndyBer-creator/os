
#include <include/cgi.h>

static int32 cgi_dr_prop_set(void);
static int32 cgi_dr_portEdit_set(void);
static int32 cgi_dr_ipPoolAdd_set(void);
static int32 cgi_dr_ipPoolDel(void);
static int32 cgi_dr_ipPoolEdit_set(void);
static int32 cgi_vlanif_pool_set(void);
static int32 cgi_dr_group_add_set(void);
static int32 cgi_dr_group_edit_set(void);
static int32 cgi_dr_group_del(void);
static int32 cgi_dr_client_static_add_set(void);
static int32 cgi_dr_client_static_edit_set(void);
static int32 cgi_dr_client_static_del(void);

const cgi_cmdmap_t cmdSetDrTbl[] =
{
    {CGI_CMD_DR_PROP,               cgi_dr_prop_set},
    {CGI_CMD_DR_PORTEDIT,           cgi_dr_portEdit_set},
	{CGI_CMD_DR_IPPOOLEDIT,			cgi_dr_ipPoolEdit_set},
	{CGI_CMD_DR_IPPOOLDEL, 		    cgi_dr_ipPoolDel},
	{CGI_CMD_DR_IPPOOLADD,          cgi_dr_ipPoolAdd_set},
	{CGI_CMD_DR_VLANIFIPPOOL,       cgi_vlanif_pool_set},
	{CGI_CMD_DR_GROUP_ADD,          cgi_dr_group_add_set},
	{CGI_CMD_DR_GROUP_EDIT,         cgi_dr_group_edit_set},
	{CGI_CMD_DR_GROUP_DEL,          cgi_dr_group_del},
	{CGI_CMD_DR_STATIC_BIND_ADD,    cgi_dr_client_static_add_set},
	{CGI_CMD_DR_STATIC_BIND_EDIT,   cgi_dr_client_static_edit_set},
	{CGI_CMD_DR_STATIC_BIND_DEL,    cgi_dr_client_static_del},
    {NULL, NULL}
};

static int32 cgi_dr_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable = DISABLED;
    sys_enable_t bindenable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;
    else
        enable = DISABLED;
        
    if (CGI_IS_PARAM_EXIST(CGI_DR_STATIC_BIND_ENABLE, value))
        bindenable = ENABLED;
    else
        bindenable = DISABLED;

    CGI_SET_ERR_HDL(sal_dhcp_server_enable_set(enable), ret);
    
	CGI_SET_ERR_HDL(sal_dhcp_server_IpBind_Switch_enable_set(bindenable), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t enable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DR_PORT_ENABLE, value))
        enable = ENABLED;
    else
        enable = DISABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_relay_port_enable_set(lp, enable), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_ipPoolAdd_set(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
    char *szName = NULL;
	sys_ipv4_t start = 0, tmpStart = 0;
	sys_ipv4_t end = 0, tmpEnd = 0;
	sys_ipv4_t gatweay = 0, tmpGateway = 0;
	sys_ipv4_t mask = 0;
	sys_ipv4_t dnsPrimary = 0;
	sys_ipv4_t dnsSecond = 0;
	uint32 days = 0, hours = 0, minutes = 0, leasetime = 0;
	int32 section = 0;
	sys_ippool_option43_t option43 = {0};

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_NAME, szName))
	{
		;
	}
	else
	{
		goto cgi_end;
	}

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_SECTION, value))
		section = SYS_STR2UINT(value);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_START_ADDR, value))
		STR2IPV4(value, &start);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_END_ADDR, value))
		STR2IPV4(value, &end);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_GATEWAY, value))
		STR2IPV4(value, &gatweay);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_MASK, value))
		STR2IPV4(value, &mask);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_DNSPRIMARY, value))
		STR2IPV4(value, &dnsPrimary);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_DNSSECOND, value))
		STR2IPV4(value, &dnsSecond);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEDAYS, value))
		days = SYS_STR2UINT(value);
	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEHOURS, value))
		hours = SYS_STR2UINT(value);
	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEMINUTES, value))
		minutes = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_OPTION43_FORMAT, value))
    {
        if (0 == osal_strcmp(value, "hex"))
            option43.ucFormat = IP_POOL_OPTION_FORMAT_HEX;
        else if (0 == osal_strcmp(value, "ascii"))
            option43.ucFormat = IP_POOL_OPTION_FORMAT_ASCII;
		else
			goto cgi_end;
    }

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_OPTION43, value))
	{
		if (IP_POOL_OPTION_FORMAT_ASCII == option43.ucFormat)
		{
			
			option43.ucValueLen = strlen(value);
			osal_memcpy(option43.szValue, value, option43.ucValueLen);
		}
		else if (IP_POOL_OPTION_FORMAT_HEX == option43.ucFormat)
		{
			if (1 == (strlen(value) % 2))
				goto cgi_end;

			option43.ucValueLen = strlen(value)/2;
			sys_util_hexStr2byteArray(value, option43.szValue);
		}		
	}

	if((days == IPPOOL_MAX_DAY) && (hours + minutes > 0))
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_MAX_TIME, ret); 
	}
	
	if((days > IPPOOL_MAX_DAY) || (hours > 23) || (minutes > 59))
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_TIME_ERR, ret); 
	}
	
	if(days == 0 && hours == 0 && minutes == 0)
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_TIME_EMPTY, ret); 
	}
	
	if(start > end)
    {
        CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_STARTIP_HIGHER, ret);
    }

	if(start == gatweay || end == gatweay)
    {
        CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW, ret);
    }
    
	if((gatweay > start)&&(gatweay < end))
	{
		CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW, ret);
	}

    tmpGateway  = gatweay & mask;
    tmpStart = start & mask;
    tmpEnd   = end & mask;
	if((tmpStart != tmpGateway)||(tmpEnd != tmpGateway))
	{
		CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_IPGATE_NOSAMESUBNET, ret);
	}

	CGI_SET_ERR_HDL(sal_ip_pool_create(szName), ret);
	
	CGI_SET_ERR_HDL(sal_ip_pool_gateway_set(szName, gatweay, mask), ret);

	CGI_SET_ERR_HDL(sal_ip_pool_section_set(szName, section-1, start, end), ret);

	CGI_SET_ERR_HDL(sal_dhcp_server_dns_set(szName, 1, dnsPrimary), ret);
	CGI_SET_ERR_HDL(sal_dhcp_server_dns_set(szName, 2, dnsSecond), ret);

	leasetime = ((days*24 + hours)*60 + minutes)*60;
	CGI_SET_ERR_HDL(sal_dhcp_server_lease_time_set(szName, leasetime), ret);
	CGI_SET_ERR_HDL(sal_ip_pool_option43_set(szName, &option43), ret);

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_dr_ipPoolDel(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_NAME, value))
	{
		CGI_SET_ERR_HDL(sal_ip_pool_del(value), ret);
	}

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_dr_ipPoolEdit_set(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
    char *szName = NULL;
	sys_ipv4_t start = 0, tmpStart = 0;
	sys_ipv4_t end = 0, tmpEnd = 0;
	sys_ipv4_t gatweay = 0, tmpGateway = 0;
	sys_ipv4_t mask = 0;
	sys_ipv4_t dnsPrimary = 0;
	sys_ipv4_t dnsSecond = 0;
	uint32 days = 0, hours = 0, minutes = 0, leasetime = 0;
	int32 section = 0;
	sys_ippool_option43_t option43 = {0};

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_NAME, szName))
	{
		
	}
	else
	{
		goto cgi_end;
	}

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_SECTION, value))
		section = SYS_STR2UINT(value);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_START_ADDR, value))
		STR2IPV4(value, &start);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_END_ADDR, value))
		STR2IPV4(value, &end);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_GATEWAY, value))
		STR2IPV4(value, &gatweay);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_MASK, value))
		STR2IPV4(value, &mask);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_DNSPRIMARY, value))
		STR2IPV4(value, &dnsPrimary);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_DNSSECOND, value))
		STR2IPV4(value, &dnsSecond);

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEDAYS, value))
		days = SYS_STR2UINT(value);
	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEHOURS, value))
		hours = SYS_STR2UINT(value);
	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_LEASEMINUTES, value))
		minutes = SYS_STR2UINT(value);
	
    if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_OPTION43_FORMAT, value))
    {
        if (0 == osal_strcmp(value, "hex"))
            option43.ucFormat = IP_POOL_OPTION_FORMAT_HEX;
        else if (0 == osal_strcmp(value, "ascii"))
            option43.ucFormat = IP_POOL_OPTION_FORMAT_ASCII;
		else
			goto cgi_end;
    }

	if (CGI_IS_PARAM_EXIST(CGI_DR_IPPOOL_OPTION43, value))
	{
		osal_memcpy(option43.szValue, value, option43.ucValueLen);
		if (IP_POOL_OPTION_FORMAT_ASCII == option43.ucFormat)
		{
			
			option43.ucValueLen = strlen(value);
			osal_memcpy(option43.szValue, value, option43.ucValueLen);
		}
		else if (IP_POOL_OPTION_FORMAT_HEX == option43.ucFormat)
		{
			if (1 == (strlen(value) % 2))
				goto cgi_end;

			option43.ucValueLen = strlen(value)/2;
			sys_util_hexStr2byteArray(value, option43.szValue);
		}		
	}

	if((days == IPPOOL_MAX_DAY) && (hours + minutes > 0))
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_MAX_TIME, ret); 
	}
	
	if((days > IPPOOL_MAX_DAY) || (hours > 23) || (minutes > 59))
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_TIME_ERR, ret); 
	}
	
	if(days == 0 && hours == 0 && minutes == 0)
	{
		CGI_SET_ERR_HDL(SYS_ERR_DHCP_LEASE_TIME_EMPTY, ret); 
	}
	
	if(start > end)
    {
        CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_STARTIP_HIGHER, ret);
    }

	if(start == gatweay || end == gatweay)
    {
        CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW, ret);
    }
    
	if((gatweay > start)&&(gatweay < end))
	{
		CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_NO_CONTAIN_GW, ret);
	}

    tmpGateway  = gatweay & mask;
    tmpStart = start & mask;
    tmpEnd   = end & mask;
	if((tmpStart != tmpGateway)||(tmpEnd != tmpGateway))
	{
		CGI_SET_ERR_HDL (SYS_ERR_DHCP_SECTION_IPGATE_NOSAMESUBNET, ret);
	}

	CGI_SET_ERR_HDL(sal_ip_pool_gateway_set(szName, gatweay, mask), ret);

	CGI_SET_ERR_HDL(sal_ip_pool_section_set(szName, section-1, start, end), ret);

	CGI_SET_ERR_HDL(sal_dhcp_server_dns_set(szName, 1, dnsPrimary), ret);
	CGI_SET_ERR_HDL(sal_dhcp_server_dns_set(szName, 2, dnsSecond), ret);

	leasetime = ((days*24 + hours)*60 + minutes)*60;
	CGI_SET_ERR_HDL(sal_dhcp_server_lease_time_set(szName, leasetime), ret);
	CGI_SET_ERR_HDL(sal_ip_pool_option43_set(szName, &option43), ret);

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_vlanif_pool_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
	uint32 intf = 0;
	uint32 group = 0;
	
    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_INTF, value))
    {
		intf = SYS_STR2UINT(value);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_ID, value))
	{
		group = SYS_STR2UINT(value);
	}

	CGI_SET_ERR_HDL(sal_dhcp_server_intf_group_ip_add(group, intf), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_group_add_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
	sys_ipv4_t ipv4 = 0;
	uint32 group = 0;
    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_IP, value))
    {
		STR2IPV4(value, &ipv4);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_ID, value))
	{
		group = SYS_STR2UINT(value);
	}

	CGI_SET_ERR_HDL(sal_dhcp_server_group_ip_add(group, ipv4), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_group_edit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
	sys_ipv4_t ipv4 = 0;
	uint32 group = 0;
	
	uint32 intf = 0;
	uint32 oldIntf = 0;

    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_IP, value))
    {
		STR2IPV4(value, &ipv4);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_ID, value))
	{
		group = SYS_STR2UINT(value);
	}

	CGI_SET_ERR_HDL(sal_dhcp_server_group_ip_add(group, ipv4), ret);

    if (CGI_IS_PARAM_EXIST(CGI_DR_STATUS, value))
	{
		
	}
	else
	{
		sal_dhcp_server_group_ip_get(group, &ipv4, &oldIntf);
		if(oldIntf != 0)
		{
			CGI_SET_ERR_HDL(sal_dhcp_server_intf_group_ip_del(group, oldIntf), ret);
		}
		goto cgi_end;
	}

    if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_INTF, value))
	{
		intf = SYS_STR2UINT(value);
	}

	sal_dhcp_server_group_ip_get(group, &ipv4, &oldIntf);
	if((oldIntf != intf) && (oldIntf != 0))
	{
		CGI_SET_ERR_HDL(sal_dhcp_server_intf_group_ip_del(group, oldIntf), ret);
	}

	CGI_SET_ERR_HDL(sal_dhcp_server_intf_group_ip_add(group, intf), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_group_del(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
	uint32 group = 0;
	
	uint32 intf = 0;
	uint32 used = 0;

	if (CGI_IS_PARAM_EXIST(CGI_DR_GROUP_ID, value))
	{
		group = SYS_STR2UINT(value);
		CGI_SET_ERR_HDL(sal_dhcp_server_check_group_used(group, &used, &intf), ret);
		if(used)
		{
			CGI_SET_ERR_HDL(sal_dhcp_server_intf_group_ip_del(group, intf), ret);
		}
		CGI_SET_ERR_HDL(sal_dhcp_server_group_ip_del(group), ret);
	}

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_dr_client_static_add_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
	sys_ipv4_t ipv4 = 0;
	sys_mac_t mac;
	char userName[32];
	sys_vid_t vid = 0;

	SYS_MEM_CLEAR(mac);
	SYS_MEM_CLEAR(ipv4);
	SYS_MEM_CLEAR(userName);
	
    if (CGI_IS_PARAM_EXIST(CGI_MAC_ADDR, value))
    {
		sys_util_str2Mac(value, mac.octet);
		
    }

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
	{
		vid = SYS_STR2UINT(value);
		
	}

	if (CGI_IS_PARAM_EXIST(CGI_DR_CLIENT_IP, value))
	{
		STR2IPV4(value, &ipv4);
		
	}

    if (CGI_IS_PARAM_EXIST(CGI_DR_CLIENT_USERNAME, value))
	{
		osal_sprintf(userName, "%s", value);
		
	}

	CGI_SET_ERR_HDL(sal_dhcp_client_host_mac_ip_add(&mac, ipv4, vid, userName), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_client_static_edit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
	sys_ipv4_t ipv4 = 0;
	sys_mac_t mac;
	char userName[32];
	sys_vid_t vid = 0;

	SYS_MEM_CLEAR(mac);
	SYS_MEM_CLEAR(ipv4);
	SYS_MEM_CLEAR(userName);
	
    if (CGI_IS_PARAM_EXIST(CGI_MAC_ADDR, value))
    {
		sys_util_str2Mac(value, mac.octet);
		
    }

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
	{
		vid = SYS_STR2UINT(value);
		
	}

	if (CGI_IS_PARAM_EXIST(CGI_DR_CLIENT_IP, value))
	{
		STR2IPV4(value, &ipv4);
		
	}

    if (CGI_IS_PARAM_EXIST(CGI_DR_CLIENT_USERNAME, value))
	{
		osal_sprintf(userName, "%s", value);
		
	}

	CGI_SET_ERR_HDL(sal_dhcp_client_host_mac_ip_add(&mac, ipv4, vid, userName), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_dr_client_static_del(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
	char **array=NULL;
	uint32 len=0;
	sys_mac_t mac;
	sys_vid_t vid = 0;
	uint32 i=0;
	char *flag = "_";

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array ,&len))
    {
    	CGI_DBG("static=%s", *array);
        for(i=0; i<len; i++)
        {
            SYS_MEM_CLEAR(mac);

            value = osal_strtok(array[i], flag);
            
            sys_util_str2Mac(value, mac.octet);

            value = osal_strtok(NULL, flag);
            
            vid = SYS_STR2UINT(value);

            CGI_SET_ERR_HDL(sal_dhcp_client_host_mac_ip_del(&mac, vid), ret);
        }
    }

cgi_end:
	cgi_set_respGeneral_send(ret);
	if (NULL != array)
        osal_free(array);

	return SYS_ERR_OK;
}

