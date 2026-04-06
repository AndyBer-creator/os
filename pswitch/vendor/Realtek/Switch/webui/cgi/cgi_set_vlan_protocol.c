
#include <include/cgi.h>

static int32 cgi_vlan_protocol_group_set(void);
static int32 cgi_vlan_protocol_groupDel_set(void);
static int32 cgi_vlan_protocol_binding_set(void);
static int32 cgi_vlan_protocol_bindingDel_set(void);

const cgi_cmdmap_t cmdSetVlanProtocolTbl[] =
{
    {CGI_CMD_VLAN_PROTOCOL_GROUP,           cgi_vlan_protocol_group_set},
    {CGI_CMD_VLAN_PROTOCOL_GROUPDEL,        cgi_vlan_protocol_groupDel_set},
    {CGI_CMD_VLAN_PROTOCOL_BINDING,         cgi_vlan_protocol_binding_set},
    {CGI_CMD_VLAN_PROTOCOL_BINDINGDEL,      cgi_vlan_protocol_bindingDel_set},
    {NULL, NULL}
};

static int32 cgi_vlan_protocol_group_set(void)
{
    char *value;
    char tmp[SYS_BUF16_LEN];
    int32 ret = SYS_ERR_OK;
    uint32 gid;
    sys_vlan_protoGroup_t group;

    SYS_MEM_CLEAR(group);

    if (CGI_IS_PARAM_EXIST(CGI_PROTOCOLVLAN_GID, value))
        gid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_PROTOCOLVLAN_FT, value))
        group.frametype = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_PROTOCOLVLAN_PV, value))
    {
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "0x%s", value);
        group.framevalue = SYS_STR2UINT(tmp);
    }
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_vlan_protoGroup_set(gid - 1, &group), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_protocol_groupDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    uint32 gid = 0;
    uint8 hasFailed = FALSE;
    char **array = NULL;
    sys_logic_port_t lp;
    sys_enable_t enable;
    sys_vlan_protoGroup_t group;
    sys_vlan_protoVlanCfg_t binding;

    SYS_MEM_CLEAR(group);
    SYS_MEM_CLEAR(binding);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_PROTOCOLVLAN_GID, array, &len))
    {
        for(i = 0; i < len; i++)
        {
            gid = SYS_STR2UINT(array[i]);
            gid -= 1;
            ret = SYS_ERR_OK;
            FOR_EACH_LOGIC_PORT(lp)
            {
                sal_vlan_portProtoVlan_get(lp, gid, &binding, &enable);

                if (ENABLED == enable)
                {
                    ret = SYS_ERR_VLAN_PROVLAN_PORT_REF;
                    hasFailed = TRUE;
                    break;
                }
            }

            if (ret == SYS_ERR_OK)
            {
                SYS_MEM_CLEAR(group);
                group.frametype = FRAME_TYPE_END;
                CGI_SET_ENTRY_ERR_CONTINUE(sal_vlan_protoGroup_set(gid, &group), "Delete");
            }
        }
    }

    if (hasFailed)
        ret = SYS_ERR_VLAN_PROVLAN_PORT_REF;

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_protocol_binding_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    uint32 gid = 0;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_vlan_protoVlanCfg_t vlanCfg;

    SYS_MEM_CLEAR(vlanCfg);

    if (CGI_IS_PARAM_EXIST(CGI_PROTOCOLVLAN_GID, value))
        gid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        vlanCfg.vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    CGI_GET_LPORTMSK_PARAM_ARRAY(CGI_PROTOCOLVLAN_MBRS, lpm);
    gid -= 1;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portProtoVlan_set(lp, gid, &vlanCfg, ENABLED), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_protocol_bindingDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i;
    uint32 gid = 0;
    char **array = NULL;
    char *flag = "_";

    sys_logic_port_t lp;
    sys_vlan_protoVlanCfg_t vlanCfg;

    SYS_MEM_CLEAR(vlanCfg);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
        {
            lp = SYS_STR2UINT(osal_strtok(array[i], flag));
            gid = SYS_STR2UINT(osal_strtok(NULL, flag));

            CGI_SET_ENTRY_ERR_CONTINUE(sal_vlan_portProtoVlan_set(lp, gid -1, &vlanCfg, DISABLED), "Delete");
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

