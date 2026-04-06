
#include <include/cgi.h>
#include <libipc/quaggaTask.h>
#include <libipc/vos.h>
#include <libsal/sal_l3_route_rip.h>

static int32 cgi_rip_prop_set(void);
static int32 cgi_rip_network_add(void);
static int32 cgi_rip_network_del(void);

const cgi_cmdmap_t cmdSetRipTbl[] =
{
    {CGI_CMD_RIP_CFG,               cgi_rip_prop_set},
    {CGI_CMD_RIP_NETWORK_ADD,       cgi_rip_network_add},
    {CGI_CMD_RIP_NETWORK_DEL,       cgi_rip_network_del},
    {NULL, NULL}
};

static int32 cgi_rip_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable = DISABLED;
    int32 oldEnable = 0;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;
    else
        enable = DISABLED;
        
	sal_rip_status_get(&oldEnable);
	if(enable != oldEnable)
	{
		if(enable)
		{
			
			CGI_SET_ERR_HDL(sal_l3_routeV4_rip_open(), ret);
		}
		else 
		{
			CGI_SET_ERR_HDL(sal_l3_routeV4_rip_cloce(), ret);
		}
	}
    
cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rip_network_add(void)
{
	int32 ret = SYS_ERR_OK;
	char *value = NULL;
	sys_ipv4_t network = 0;
	sys_ipv4_t mask = 0;

	if (CGI_IS_PARAM_EXIST(CGI_RIP_NETWORKADDR, value))
		STR2IPV4(value, &network);

	if (CGI_IS_PARAM_EXIST(CGI_RIP_NETWORKMASK, value))
		STR2IPV4(value, &mask);

	if(network && mask)
	{
		CGI_SET_ERR_HDL(sal_l3_rip_network_set(ENABLED, network, mask, NULL), ret);
	}
	else if(network == 0)
	{
		CGI_SET_ERR_HDL(sal_l3_rip_network_set(ENABLED, network, mask, NULL), ret);
	}

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

static int32 cgi_rip_network_del(void)
{
	int32 ret = SYS_ERR_OK;
	sys_ipv4_t network = 0;
	sys_ipv4_t mask = 0;
	uint32 len = 0;
	uint32 i = 0;
    char **array = NULL;
    char *token = NULL;

	if (CGI_GET_PARAM_ARRAY_DATA(CGI_RIP_NETWORKADDR, array, &len))
	{
		for (i = 0; i < len; i++)
        {
            SYS_MEM_CLEAR(network);
    		SYS_MEM_CLEAR(mask);

			token = strtok(array[i], "_");
			if( token != NULL ) 
            {
			    STR2IPV4(token, &network);
			}
			
			token = strtok(NULL, "_");
			if( token != NULL ) 
            {
				STR2IPV4(token, &mask);
			}

			if(network && mask)
			{
				CGI_SET_ERR_HDL(sal_l3_rip_network_set(DISABLED, network, mask, NULL), ret);
			}	
			else if(network == 0)
			{
				CGI_SET_ERR_HDL(sal_l3_rip_network_set(DISABLED, network, mask, NULL), ret);
			}
        }

	}

cgi_end:
	cgi_set_respGeneral_send(ret);

	return SYS_ERR_OK;
}

#if 0
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
#endif
