
#include <include/cgi.h>

static int32 cgi_vlan_protocol_group_get(void);
static int32 cgi_vlan_protocol_binding_get(void);

const cgi_cmdmap_t cmdGetVlanProtocolTbl[] =
{
    {CGI_CMD_VLAN_PROTOCOL_GROUP,        cgi_vlan_protocol_group_get},
    {CGI_CMD_VLAN_PROTOCOL_BINDING,      cgi_vlan_protocol_binding_get},
    {NULL, NULL}
};

static int32 cgi_vlan_protocol_group_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
	cJSON *groups = NULL;
    cJSON *group = NULL;
    cJSON *gids = NULL;
    cJSON *gid = NULL;
    char tmp[SYS_BUF64_LEN];
    uint32 i = 0;
    uint32 cnt = 0;
    uint32 cntid = 0;
    sys_vlan_protoGroup_t  protoGroup;

	CGI_GET_JSON_CREATE(root, data);

    groups = cJSON_CreateArray();
    gids = cJSON_CreateArray();

    for (i = 1; i <= CAPA_VLAN_PROTO_GROUP_NUM; i++)
    {
        SYS_ERR_CHK(sal_vlan_protoGroup_get(i-1, &protoGroup));

        if (protoGroup.frametype == FRAME_TYPE_END)
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

            SYS_MEM_CLEAR(tmp);
            osal_sprintf(tmp, "%04X", protoGroup.framevalue);

            cJSON_AddNumberToObject(group, CGI_PROTOCOLVLAN_GID, i);
            cJSON_AddNumberToObject(group, CGI_PROTOCOLVLAN_FT, protoGroup.frametype);
            cJSON_AddStringToObject(group, CGI_PROTOCOLVLAN_PV, tmp);
            cJSON_AddItemToArray(groups, group);
            cnt++;
        }
    }

    if (0 != cnt)
        cJSON_AddItemToObject(data, CGI_PROTOCOLVLAN_GROUPS, groups);
    else
        cJSON_Delete(groups);

    if (0 != cntid)
        cJSON_AddItemToObject(data, CGI_PROTOCOLVLAN_GID, gids);
    else
        cJSON_Delete(gids);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_protocol_binding_get(void)
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
    sys_enable_t enable;
    sys_vlan_protoVlanCfg_t vlanCfg;
    sys_vlan_protoGroup_t  protoGroup;
    sys_vlan_portMode_t vlanPortMode;

	CGI_GET_JSON_CREATE(root, data);

    groups = cJSON_CreateArray();
    cJSON_AddItemToObject(data, CGI_PROTOCOLVLAN_GROUPS, groups);

    for (i = 0; i < CAPA_VLAN_PROTO_GROUP_NUM; i++)
    {
        SYS_ERR_CHK(sal_vlan_protoGroup_get(i, &protoGroup));

        if (protoGroup.frametype == FRAME_TYPE_END)
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

    SYS_MEM_CLEAR(vlanCfg);
    cnt = 0;

    bindings = cJSON_CreateArray();

    FOR_EACH_LOGIC_PORT(lp)
    {
        for (i = 0; i < CAPA_VLAN_PROTO_GROUP_NUM; i++)
        {
            sal_vlan_portProtoVlan_get(lp, i, &vlanCfg, &enable);

            if (enable != ENABLED)
                continue;

            binding = cJSON_CreateObject();
            cJSON_AddNumberToObject(binding, CGI_PORT, lp);
            cJSON_AddNumberToObject(binding, CGI_PROTOCOLVLAN_GID, i+1);
            cJSON_AddNumberToObject(binding, CGI_VLAN, vlanCfg.vid);
            cJSON_AddItemToArray(bindings, binding);
            cnt++;
        }
    }

    if (cnt != 0)
        cJSON_AddItemToObject(data, CGI_PROTOCOLVLAN_BINDING, bindings);

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

