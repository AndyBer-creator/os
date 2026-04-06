
#include <include/cgi.h>
#include <libsal/sal_vlan_mac.h>

static int32 cgi_vlan_mac_group_set(void);
static int32 cgi_vlan_mac_group_add(void);
static int32 cgi_vlan_mac_groupDel_set(void);
static int32 cgi_vlan_mac_binding_set(void);
static int32 cgi_vlan_mac_bindingDel_set(void);

const cgi_cmdmap_t cmdSetVlanMacTbl[] =
{
    {CGI_CMD_VLAN_MAC_GROUP,           cgi_vlan_mac_group_set},
	{CGI_CMD_VLAN_MAC_GROUP_ADD,       cgi_vlan_mac_group_add},
    {CGI_CMD_VLAN_MAC_GROUPDEL,        cgi_vlan_mac_groupDel_set},
    {CGI_CMD_VLAN_MAC_BINDING,         cgi_vlan_mac_binding_set},
    {CGI_CMD_VLAN_MAC_BINDINGDEL,      cgi_vlan_mac_bindingDel_set},
    {NULL, NULL}
};

static int32 cgi_vlan_mac_group_set(void)
{
    char *value;
    char *tmp;
    char *flag = "_";
    int32 ret = SYS_ERR_OK;
    sys_mac_vlan_group_t group;
	sys_mac_t zeroMac;
    
    SYS_MEM_CLEAR(group);
    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_MAC, value))
    {
    	SYS_MEM_CLEAR(zeroMac);
        sys_util_str2Mac(value, group.mac.octet);
        if (0 == osal_memcmp(&group.mac,&zeroMac,sizeof(zeroMac)))
        {
        	CGI_SET_ERR_HDL(SYS_ERR_MAC_ADDRESS_INVALID, ret);
        }
    }
    else
        goto cgi_end;
    SYS_MEM_CLEAR(group);
    
    if(CGI_IS_PARAM_EXIST(CGI_IDX, value))
    {
        
        tmp = osal_strtok(value, flag);
        group.groupId= SYS_STR2UINT(tmp);

        tmp = osal_strtok(NULL, flag);
        sys_util_str2Mac(tmp, group.mac.octet);

        tmp = osal_strtok(NULL, flag);
        group.mask = SYS_STR2UINT(tmp);

        CGI_SET_ERR_HDL(sal_macvlan_group_del(&group), ret);
    }

    SYS_MEM_CLEAR(group);

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_GID, value))
        group.groupId= SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_MAC, value))
    {
        sys_util_str2Mac(value, group.mac.octet);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_MASK, value))
        group.mask = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_macvlan_group_set(&group), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}
static int32 cgi_vlan_mac_group_add(void)
{
    char *value;
	int32 i;
	int32 tmpId;
    int32 ret = SYS_ERR_OK;
    sys_mac_vlan_group_t group;	
    sys_mac_t zeroMac;

	SYS_MEM_CLEAR(group);
    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_GID, value))
        tmpId= SYS_STR2UINT(value);
    else
        goto cgi_end;	
    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
    {
        sal_macvlan_group_get(i, &group);

        if ((group.mask != 0)&&(group.groupId == tmpId))
        {
        	CGI_SET_ERR_HDL(SYS_ERR_MAC_VLAN_GROUP_ID, ret);
        }
  
    }
    SYS_MEM_CLEAR(group);
    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_GID, value))
        group.groupId= SYS_STR2UINT(value);
    else
        goto cgi_end; 	

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_MAC, value))
    {
		SYS_MEM_CLEAR(zeroMac);
        sys_util_str2Mac(value, group.mac.octet);
        if (0 == osal_memcmp(&group.mac,&zeroMac,sizeof(zeroMac)))
        {
        	CGI_SET_ERR_HDL(SYS_ERR_MAC_ADDRESS_INVALID, ret);
        }
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_MASK, value))
        group.mask = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_macvlan_group_set(&group), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_mac_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    char **array = NULL;
    char *flag = "_";
    char *value =  NULL;
    sys_mac_vlan_group_t group;

    SYS_MEM_CLEAR(group);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
        {
            
            value = osal_strtok(array[i], flag);
            group.groupId= SYS_STR2UINT(value);

            value = osal_strtok(NULL, flag);
            sys_util_str2Mac(value, group.mac.octet);

            value = osal_strtok(NULL, flag);
            group.mask = SYS_STR2UINT(value);

            CGI_SET_ENTRY_ERR_CONTINUE(sal_macvlan_group_del(&group), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_mac_binding_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_mac_vlan_mapping_t mapping;

    SYS_MEM_CLEAR(mapping);

    if (CGI_IS_PARAM_EXIST(CGI_MACVLAN_GID, value))
        mapping.groupId = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        mapping.vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_MACVLAN_MBRS, lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_macvlan_mapping_set(lp, &mapping), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_mac_bindingDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    char **array = NULL;
    char *flag = "_";
    sys_logic_port_t lp;
    sys_mac_vlan_mapping_t mapping;

    SYS_MEM_CLEAR(mapping);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
    	{
            lp = SYS_STR2UINT(osal_strtok(array[i], flag));
            mapping.groupId= SYS_STR2UINT(osal_strtok(NULL, flag));
            CGI_SET_ENTRY_ERR_CONTINUE(sal_macvlan_mapping_del(lp, &mapping), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

