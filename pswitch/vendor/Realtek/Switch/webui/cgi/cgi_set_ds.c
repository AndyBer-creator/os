
#include <include/cgi.h>

static int32 cgi_ds_prop_set(void);
static int32 cgi_ds_portEdit_set(void);
static int32 cgi_ds_statClear_set(void);
static int32 cgi_ds_option82Prop_set(void);
static int32 cgi_ds_option82PortEdit_set(void);
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
static int32 cgi_ds_option82CID_set(void);
static int32 cgi_ds_option82CIDDel_set(void);
#endif

const cgi_cmdmap_t cmdSetDsTbl[] =
{
    {CGI_CMD_DS_PROP,               cgi_ds_prop_set},
    {CGI_CMD_DS_PORTEDIT,           cgi_ds_portEdit_set},
    {CGI_CMD_DS_STATISTIC,          cgi_ds_statClear_set},
    {CGI_CMD_DS_82_PROP,            cgi_ds_option82Prop_set},
    {CGI_CMD_DS_82_PORTEDIT,        cgi_ds_option82PortEdit_set},
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    {CGI_CMD_DS_82_CID,             cgi_ds_option82CID_set},
    {CGI_CMD_DS_82_CID_DEL,         cgi_ds_option82CIDDel_set},
#endif
    {NULL, NULL}
};

static int32 cgi_ds_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    sys_vid_t vid;
    sys_vlanmask_t existVM;
    sys_vlanmask_t newVM;
    sys_enable_t enable = DISABLED;

    SYS_MEM_CLEAR(existVM);
    SYS_MEM_CLEAR(newVM);
    sal_dhcp_snooping_vlanmask_enable_get(&existVM);

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;
    else
        enable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VLANLIST, value))
        STR2VMSK(value, &newVM);
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_dhcp_snooping_enable_set(enable), ret);

    FOR_EACH_VID_IN_VLANMASK(vid, newVM)
    {
	    CGI_SET_VID_ERR_CONTINUE(sal_dhcp_snooping_vlan_enable_set(vid, ENABLED), vid);
        VLANMASK_CLEAR_VID(existVM, vid);
    }

    FOR_EACH_VID_IN_VLANMASK(vid, existVM)
    {
        CGI_SET_VID_ERR_CONTINUE(sal_dhcp_snooping_vlan_enable_set(vid, DISABLED), vid);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ds_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    uint32 rate = BOND_RATE_UNLIMITED;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t trust = DISABLED;
    sys_enable_t chaddr = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_DS_TRUSTED, value))
        trust = ENABLED;
    else
        trust = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DS_VERIMAC, value))
        chaddr = ENABLED;
    else
        chaddr = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DS_RATE, value))
    {
    	rate = SYS_STR2UINT(value);

    	if (0 == rate)
    	    rate = BOND_RATE_UNLIMITED;
    }
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_snooping_port_trust_set(lp, trust), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_snooping_portChaddrChk_set(lp, chaddr), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_snooping_rateLimit_set(lp, rate), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ds_statClear_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_snooping_portMib_clear(lp), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ds_option82Prop_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_dhcp_option82_id_t rid;

    SYS_MEM_CLEAR(rid);

    if (CGI_IS_PARAM_EXIST(CGI_DS_RID_STATIC, value))
        osal_memcpy(&rid, value, strlen(value));

    CGI_SET_ERR_HDL(sal_dhcp_option82_remoteID_set(&rid), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ds_option82PortEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_dhcp_option82_act_t action = 0;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        enable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_DS_ALLOWUNTRUSTED, value))
	    action = SYS_STR2UINT(value);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_option82_action_set(lp, action), lp);
        CGI_SET_PORT_ERR_CONTINUE(sal_dhcp_option82_enable_set(lp, enable), lp);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
static int32 cgi_ds_option82CID_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *value = NULL;
    sys_logic_port_t lp = 0;
    sys_dhcp_option82_circuit_id_t cid;

    SYS_MEM_CLEAR(cid);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, value))
        STR2LP(value, &lp);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VLAN, value))
        cid.vid = SYS_STR2UINT(value);

    if (CGI_IS_PARAM_EXIST(CGI_DS_CID, value))
        osal_memcpy(cid.str, value, osal_strlen(value));
    else
        goto cgi_end;

    CGI_SET_ERR_HDL(sal_dhcp_option82_circuitID_set(lp, &cid), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_ds_option82CIDDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;
    char *flag = "?";
    char *value =  NULL;
    sys_logic_port_t lp;
    sys_dhcp_option82_circuit_id_t cid;

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array ,&len))
    {
        for(i=0; i<len; i++)
        {
            SYS_MEM_CLEAR(cid);

            value = osal_strtok(array[i], flag);
            lp = SYS_STR2UINT(value);
            value = osal_strtok(NULL, flag);
            cid.vid = SYS_STR2UINT(value);

            if (DFLT_VLAN_RESERVED_VLAN == cid.vid)
                cid.vid = 0;

            CGI_SET_ENTRY_ERR_CONTINUE(sal_dhcp_option82_circuitID_set(lp, &cid), "Cid");
        }
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}
#endif

