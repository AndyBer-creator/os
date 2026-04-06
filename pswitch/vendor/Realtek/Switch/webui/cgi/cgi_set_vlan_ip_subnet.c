
#include <include/cgi.h>

static int32 cgi_vlan_ip_subnet_group_set(void);
static int32 cgi_vlan_ip_subnet_groupDel_set(void);
static int32 cgi_vlan_ip_subnet_binding_set(void);
static int32 cgi_vlan_ip_subnet_bindingDel_set(void);

const cgi_cmdmap_t cmdSetVlanIpSubnetTbl[] =
{
    {CGI_CMD_VLAN_IPSUBNET_GROUP,           cgi_vlan_ip_subnet_group_set},
    {CGI_CMD_VLAN_IPSUBNET_GROUPDEL,        cgi_vlan_ip_subnet_groupDel_set},
    {CGI_CMD_VLAN_IPSUBNET_BINDING,         cgi_vlan_ip_subnet_binding_set},
    {CGI_CMD_VLAN_IPSUBNET_BINDINGDEL,      cgi_vlan_ip_subnet_bindingDel_set},
    {NULL, NULL}
};

static int32 cgi_vlan_ip_subnet_group_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    uint32 gid;
    sys_ipSubnet_vlan_group_t group;
	sys_ipv4_t mask =0;

    SYS_MEM_CLEAR(group);

    if (CGI_IS_PARAM_EXIST(CGI_IPSUBNETVLAN_GID, value))
        gid = SYS_STR2UINT(value);
    else
        goto cgi_end;

	if (CGI_IS_PARAM_EXIST(CGI_MAC_VID, value))
	{
		group.vid = SYS_STR2UINT(value);
	}
    else
        goto cgi_end;

	if (CGI_IS_PARAM_EXIST(CGI_L3_IPADDR, value))
	{
		STR2IPV4(value, &group.ipaddr.addr);
	}
    else
        goto cgi_end;
        
	if (CGI_IS_PARAM_EXIST(CGI_L3_MASKTYPE, value))
	{
		if (0 == osal_strcmp(value, "network"))
		{
			if (CGI_IS_PARAM_EXIST(CGI_L3_IPMASK, value))
			{
				STR2IPV4(value, &mask);
				group.ipaddr.plen = IPV4_MASK2PFX(mask);
			}
		}
		else
		{
			if (CGI_IS_PARAM_EXIST(CGI_L3_PLEN, value))
			{
				group.ipaddr.plen = SYS_STR2UINT(value);
			}
		}
	}

    if (CGI_IS_PARAM_EXIST(CGI_IPSUBNETVLAN_PRI, value))
        group.priority = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_vlan_ip_subnet_group_set(gid - 1, &group), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_ip_subnet_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    uint32 gid = 0;
    char **array = NULL;
    sys_ipSubnet_vlan_group_t group;

    SYS_MEM_CLEAR(group);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IPSUBNETVLAN_GID, array, &len))
    {
        for(i = 0; i < len; i++)
        {
            gid = SYS_STR2UINT(array[i]);
            gid -= 1;
            ret = SYS_ERR_OK;

            SYS_MEM_CLEAR(group);  
            CGI_SET_ENTRY_ERR_CONTINUE(sal_vlan_ip_subnet_group_del(gid), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_ip_subnet_binding_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    uint32 gid = 0;
    sys_logic_port_t lp;
	sys_logic_portmask_t lpm;
	sys_enable_t oldEnable;
	
    if (CGI_IS_PARAM_EXIST(CGI_IPSUBNETVLAN_GID, value))
        gid = SYS_STR2UINT(value);
    else
        goto cgi_end;

	CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_PROTOCOLVLAN_MBRS, lpm);

	FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
	{
		CGI_SET_ERR_HDL(sal_vlan_port_ipsubnet_vlan_status(lp, &oldEnable), ret);
		if(oldEnable == DISABLED)
			CGI_SET_ERR_HDL(sal_vlan_port_ipsubnet_vlan_enable(lp, ENABLED), ret);
			
		CGI_SET_ERR_HDL(sal_vlan_port_ipsubnet_vlan_group_bind_set(lp, gid-1, ENABLED), ret);
	}

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_ip_subnet_bindingDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    uint32 gid = 0;
    char **array = NULL;
    char *flag = "_";
	uint32 groupIdx = 0;
    sys_logic_port_t lp;
	uint32  rc = SYS_ERR_OK;
	
    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
        {	
            lp = SYS_STR2UINT(osal_strtok(array[i], flag));
            gid = SYS_STR2UINT(osal_strtok(NULL, flag));

            CGI_SET_ENTRY_ERR_CONTINUE(sal_vlan_port_ipsubnet_vlan_group_bind_set(lp, gid -1, DISABLED), "Delete");

            for (groupIdx = 0; groupIdx < CAPA_VLAN_IP_SUBNET_GROUP_NUM; groupIdx++)
			{
				rc = sal_vlan_port_ipsubnet_vlan_group_bind_get(lp, &groupIdx);
				if(rc == SYS_ERR_ENTRY_NOT_FOUND)
				{
					sal_vlan_port_ipsubnet_vlan_enable(lp, DISABLED);
					break;
				}
			}
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

