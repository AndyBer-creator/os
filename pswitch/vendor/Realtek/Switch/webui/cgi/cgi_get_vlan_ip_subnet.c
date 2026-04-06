
#include <include/cgi.h>

static int32 cgi_vlan_ip_subnet_group_get(void);
static int32 cgi_vlan_ip_subnet_binding_get(void);

const cgi_cmdmap_t cmdGetVlanIpSubNetTbl[] =
{
    {CGI_CMD_VLAN_IPSUBNET_GROUP,        cgi_vlan_ip_subnet_group_get},
    {CGI_CMD_VLAN_IPSUBNET_BINDING,      cgi_vlan_ip_subnet_binding_get},
    {NULL, NULL}
};

static int32 cgi_vlan_ip_subnet_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *groups = NULL;
    cJSON *group = NULL;
    cJSON *gids = NULL;
    cJSON *gid = NULL;
    uint32 i = 0;
    uint32 cnt = 0;
    uint32 cntid = 0;
    sys_ipSubnet_vlan_group_t  protoGroup;
	char ipStr[CAPA_IPV4_STR_LEN];
	char tmp[SYS_BUF64_LEN];

	CGI_GET_JSON_CREATE(root, data);

    groups = cJSON_CreateArray();
    gids = cJSON_CreateArray();

    for (i = 1; i <= CAPA_VLAN_IP_SUBNET_GROUP_NUM; i++)
    {
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(i-1, &protoGroup));

        if((protoGroup.ipaddr.addr == 0) && (protoGroup.vid == 0))
        {
            gid = cJSON_CreateObject();

            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "%u", i);

            cJSON_AddStringToObject(gid, CGI_NAME, tmp);
            cJSON_AddNumberToObject(gid, CGI_VALUE, i);
            cJSON_AddItemToArray(gids, gid);
            cntid++;
        }
		else
		{
	        group = cJSON_CreateObject();

	        cJSON_AddNumberToObject(group, CGI_IPSUBNETVLAN_GID, i);
	        
	        SYS_MEM_CLEAR(ipStr);
	        sys_util_ip2Str(protoGroup.ipaddr.addr, ipStr);
	        cJSON_AddStringToObject(group, CGI_L3_IPADDR, ipStr);
	        
	        SYS_MEM_CLEAR(ipStr);
	        sys_util_ip2Str(IPV4_PFX2MASK(protoGroup.ipaddr.plen), ipStr);
	        cJSON_AddStringToObject(group, CGI_L3_IPMASK, ipStr);

			cJSON_AddNumberToObject(group, CGI_MAC_VID, protoGroup.vid);
			cJSON_AddNumberToObject(group, CGI_IPSUBNETVLAN_PRI, protoGroup.priority);
			SYS_MEM_CLEAR(ipStr);
			if(protoGroup.valid)
			{
				LP2WEBSTR(protoGroup.lport, ipStr);
				cJSON_AddStringToObject(group, CGI_IPSUBNETVLAN_PORTS, ipStr);
			}
			else
			{
				cJSON_AddStringToObject(group, CGI_IPSUBNETVLAN_PORTS, "NA");
			}

	        cJSON_AddItemToArray(groups, group);
	        cnt++;  
        }
    }

    if (0 != cnt)
        cJSON_AddItemToObject(data, CGI_IPSUBNETVLAN_GROUPS, groups);
    else
        cJSON_Delete(groups);

    if (0 != cntid)
        cJSON_AddItemToObject(data, CGI_IPSUBNETVLAN_GID, gids);
    else
        cJSON_Delete(gids);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_ip_subnet_binding_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *groups = NULL;
    cJSON *group = NULL;
	cJSON *bindings = NULL;
    cJSON *binding = NULL;
    cJSON *ports = NULL;
    cJSON *port = NULL;
    char tmp[SYS_BUF64_LEN];
    char portStr[CAPA_PORT_STR_LEN + 1];
    uint32 i = 0;
    uint32 cnt = 0;
    sys_logic_port_t lp = 0;
    sys_ipSubnet_vlan_group_t  protoGroup;
    sys_vlan_portMode_t vlanPortMode;
	uint32  rc = SYS_ERR_OK;

	CGI_GET_JSON_CREATE(root, data);

    groups = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_IPSUBNETVLAN_GROUPS, groups);

    for (i = 0; i < CAPA_VLAN_IP_SUBNET_GROUP_NUM; i++)
    {
        SYS_ERR_CHK(sal_vlan_ip_subnet_group_get(i, &protoGroup));

        if((protoGroup.ipaddr.addr == 0) && (protoGroup.vid == 0))
            continue;

        group = cJSON_CreateObject();

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%u", i+1);

        cJSON_AddNumberToObject(group, CGI_VALUE, i+1);
        cJSON_AddStringToObject(group, CGI_NAME, tmp);
        cJSON_AddItemToArray(groups, group);
        cnt++;
    }

    if (0 == cnt)
    {
        group = cJSON_CreateObject();
        cJSON_AddNumberToObject(group, CGI_VALUE, 0);
        cJSON_AddStringToObject(group, CGI_NAME, "None");
        cJSON_AddItemToArray(groups, group);
    }

    cnt = 0;

    bindings = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
        for (i = 0; i < CAPA_VLAN_IP_SUBNET_GROUP_NUM; i++)
        {
			rc = sal_vlan_port_ipsubnet_vlan_group_bind_get(lp, &i);
			if(rc == SYS_ERR_ENTRY_NOT_FOUND)
			{
				break;
			}

            binding = cJSON_CreateObject();
            cJSON_AddNumberToObject(binding, CGI_PORT, lp);
            cJSON_AddNumberToObject(binding, CGI_IPSUBNETVLAN_GID, i+1);
            cJSON_AddItemToArray(bindings, binding);
            cnt++;
        }
    }

    if (cnt != 0)
        cJSON_AddItemToObject(data, CGI_IPSUBNETVLAN_BINDING, bindings);

    cnt = 0;
    ports = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_vlan_portAdminMode_get(lp, &vlanPortMode);

        if (vlanPortMode != SYS_VLAN_PORT_MODE_GENERAL)
            continue;

        SYS_MEM_CLEAR(portStr);
        LP2WEBSTR(lp, portStr);

        port = cJSON_CreateObject();
        cJSON_AddStringToObject(port, CGI_PORT, portStr);
        cJSON_AddItemToArray(ports, port);
        cnt++;
    }

    if (cnt != 0)
        cJSON_AddItemToObject(data, CGI_LAG_NOT_MBRS, ports);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);
    return SYS_ERR_OK;
}

