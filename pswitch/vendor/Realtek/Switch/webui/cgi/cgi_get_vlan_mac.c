
#include <include/cgi.h>
#include <libsal/sal_vlan_mac.h>

static int32 cgi_vlan_mac_group_get(void);
static int32 cgi_vlan_mac_binding_get(void);

const cgi_cmdmap_t cmdGetVlanMacTbl[] =
{
    {CGI_CMD_VLAN_MAC_GROUP,        cgi_vlan_mac_group_get},
    {CGI_CMD_VLAN_MAC_BINDING,      cgi_vlan_mac_binding_get},
    {NULL, NULL}
};

static int32 cgi_vlan_mac_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *groups = NULL;
    cJSON *group = NULL;
    char tmp[SYS_BUF64_LEN];
    uint32 i = 0;
    uint32 cnt = 0;
    sys_mac_vlan_group_t macgroup;

	CGI_GET_JSON_CREATE(root, data);
    cJSON_AddNumberToObject(data, CGI_MACVLAN_MINIDX, BOND_VLAN_MAC_GROUP_MIN);
    cJSON_AddNumberToObject(data, CGI_MACVLAN_MAXIDX, BOND_VLAN_MAC_GROUP_MAX);

    groups = cJSON_CreateArray();

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM ; i++)
    {
        sal_macvlan_group_get(i, &macgroup);

        if (macgroup.mask == 0)
            continue;

        group = cJSON_CreateObject();

        SYS_MEM_CLEAR(tmp);
        sys_util_mac2Str(macgroup.mac.octet, tmp);

        cJSON_AddNumberToObject(group, CGI_MACVLAN_GID, macgroup.groupId);
        cJSON_AddStringToObject(group, CGI_MACVLAN_MAC, tmp);
        cJSON_AddNumberToObject(group, CGI_MACVLAN_MASK, macgroup.mask);
        cJSON_AddItemToArray(groups, group);
        cnt++;
    }

    if (0 != cnt)
        cJSON_AddItemToObject(data, CGI_MACVLAN_GROUPS, groups);
    else
    {
        cJSON_Delete(groups);
        cJSON_AddNumberToObject(data, CGI_EMPTY, 0);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_mac_binding_get(void)
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
    sys_mac_vlan_group_t macgroup;
    sys_mac_vlan_mapping_t mapping[CAPA_VLAN_MAC_GROUP_NUM_MAX];
    sys_vlan_portMode_t vlanPortMode;

    SYS_MEM_CLEAR(mapping);

	CGI_GET_JSON_CREATE(root, data);

    groups = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_MACVLAN_GROUPS, groups);

    for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
    {
        sal_macvlan_group_get(i, &macgroup);

        if (macgroup.mask == 0)
            continue;

        group = cJSON_CreateObject();

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%u", macgroup.groupId);

        cJSON_AddNumberToObject(group, CGI_VALUE, macgroup.groupId);
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
        SYS_MEM_CLEAR(mapping);
        SYS_MEM_CLEAR(portStr);
        sal_macvlan_mapping_get(lp, mapping);

        for (i = 0; i < CAPA_VLAN_MAC_GROUP_NUM; i++)
        {
            if (mapping[i].groupId == 0)
                continue;

            binding = cJSON_CreateObject();
            cJSON_AddNumberToObject(binding, CGI_PORT, lp);
            cJSON_AddNumberToObject(binding, CGI_MACVLAN_GID, mapping[i].groupId);
            cJSON_AddNumberToObject(binding, CGI_VLAN, mapping[i].vid);
            cJSON_AddItemToArray(bindings, binding);
            cnt++;
        }
    }

    if (cnt != 0)
        cJSON_AddItemToObject(data, CGI_MACVLAN_BINDING, bindings);

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

